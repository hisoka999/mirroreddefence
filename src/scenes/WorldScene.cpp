#include "WorldScene.h"
#include "engine/graphics/TextureManager.h"
#include "engine/utils/string.h"
#include "engine/core/ecs/Component.h"
#include "engine/ui/Container.h"
#include "engine/ui/ImageButton.h"
#include "engine/ui/Button.h"
#include "HighScoreScene.h"
#include "engine/utils/json/parser.h"
#include <fstream>

namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, [[maybe_unused]] std::shared_ptr<utils::IniBase> settings, [[maybe_unused]] core::Input *input)
        : core::Scene(pRenderer), sceneManager(pSceneManager)
    {

        world = worldLoader.loadWorld("maps/level1.tmj", 1);
        towerBase = graphics::TextureManager::Instance().loadTexture("images/Tower.png");
        font = graphics::TextureManager::Instance().loadFont("fonts/PressStart2P-Regular.ttf", 20);
        initUI();
    }

    void WorldScene::initUI()
    {

        auto cannonButton = std::make_shared<UI::ImageButton>(nullptr, 64, 64, 0, 0, true);
        cannonButton->setImage(graphics::TextureManager::Instance().loadTexture("images/Cannon.png"));
        cannonButton->setPos(30, 100);
        cannonButton->setHoverColor({0x33, 0x33, 0x33, 0xff});

        cannonButton->connect("buttonClick", [this]()
                              { this->towerToBuild = world::TowerType::Canon; upgrade = false; });
        container.addObject(cannonButton);

        auto mgButton = std::make_shared<UI::ImageButton>(nullptr, 64, 64, 0, 0, true);
        mgButton->setImage(graphics::TextureManager::Instance().loadTexture("images/MG.png"));
        mgButton->setPos(30, 200);
        mgButton->setHoverColor({0x33, 0x33, 0x33, 0xff});
        mgButton->connect("buttonClick", [this]()
                          { this->towerToBuild = world::TowerType::MG;upgrade = false; });
        container.addObject(mgButton);

        auto launcherButton = std::make_shared<UI::ImageButton>(nullptr, 64, 64, 0, 0, true);
        launcherButton->setImage(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher.png"));
        launcherButton->setPos(30, 300);
        launcherButton->setHoverColor({0x33, 0x33, 0x33, 0xff});

        launcherButton->connect("buttonClick", [this]()
                                { this->towerToBuild = world::TowerType::Missle_Launcher;upgrade = false; });
        container.addObject(launcherButton);

        auto upgradeButton = std::make_shared<UI::Button>(nullptr);
        upgradeButton->setPos(20, 400);
        upgradeButton->setFont("fonts/PressStart2P-Regular.ttf", 14);
        upgradeButton->setLabel("UPGRADE");
        upgradeButton->connect("buttonClick", [this]()
                               { upgrade = true; this->towerToBuild .reset(); });
        container.addObject(upgradeButton);

        enterNameWindow = std::make_shared<windows::EnterNameWindow>();
        winMgr->addWindow(enterNameWindow.get());
        enterNameWindow->connect("closed", [this]()
                                 { 
                                            world::Player player = world->getPlayer(); 
                                            saveHighScore(player,enterNameWindow->getName());
                                            this->sceneManager->setCurrentScene("highScore"); });
    }

    WorldScene::~WorldScene()
    {
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        if (container.handleEvents(pInput))
            return true;

        if (winMgr->handleInput(pInput))
            return true;

        mousePos = pInput->getMousePostion();

        graphics::Rect towerPos = buildTowerRect();
        if (upgrade && pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
        {
            world->tryUpgradeBuilding(pInput->getMousePostion());
        }
        else if (world->canBuildTower(towerPos) && pInput->isMouseButtonUp(SDL_BUTTON_LEFT) && towerToBuild && world->getPlayer().cash >= static_cast<int>(towerToBuild.value()))
        {
            world->buildTower({towerPos.x, towerPos.y}, towerToBuild.value());
            world->getPlayer().cash -= static_cast<int>(towerToBuild.value());
            towerToBuild.reset();
        }

        return core::Scene::handleEventsEntities(pInput);
    }

    void WorldScene::load()
    {
    }

    graphics::Rect WorldScene::buildTowerRect()
    {
        return {float(int((mousePos.getX() - 16) / 32) * 32), float(int((mousePos.getY() - 16) / 32) * 32), 64, 64};
    }

    void WorldScene::render()
    {
        world->render(renderer);
        graphics::Rect towerPos = buildTowerRect();

        if (towerToBuild)
        {
            towerBase->setAlphaMod(200);
            if (!world->canBuildTower(towerPos))
            {
                towerBase->setColorKey(255, 0, 0);
            }
            else
            {
                towerBase->setColorKey(255, 255, 255);
            }

            towerBase->renderResized(renderer, towerPos.x, towerPos.y, 64, 64);
            towerBase->setColorKey(255, 255, 255);
        }
        renderEntities(renderer);

        drawUI();
        container.render(renderer);
        winMgr->render(renderer);
    }

    void WorldScene::update()
    {
    }

    void WorldScene::fixedUpdate(uint32_t delta)
    {
        world->update(delta);

        if (world->isWorldFinished())
        {
            int nextLevel = world->getLevelId() + 1;
            world::Player player = world->getPlayer();
            if (player.lives > 0)
            {
                std::string levelPath = "maps/level" + std::to_string(nextLevel) + ".tmj";
                if (std::filesystem::exists(levelPath))
                {
                    world = worldLoader.loadWorld(levelPath, nextLevel);
                    world->setPlayer(player);
                }
                else
                {
                    fadeOutTimer = 5000;
                    enterNameWindow->setVisible(true);
                }
            }
            else
            {
                fadeOutTimer = 5000;
                enterNameWindow->setVisible(true);
            }
        }

        if (fadeOutTimer > 0)
        {
            fadeOutTimer -= delta;
        }
    }

    void WorldScene::drawUI()
    {
        graphics::Rect background{0, 0, renderer->getMainCamera()->getWidth(), 50};

        graphics::Rect fadeRect{0, 0, renderer->getMainCamera()->getWidth(), renderer->getMainCamera()->getHeight()};

        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer->setDrawColor(22, 22, 22, 100);
        renderer->fillRect(background);
        SDL_Color fontColor{0, 255, 0, 255};
        font->render(renderer, utils::string_format("Life:%d", world->getPlayer().lives), fontColor, 5, 10);
        font->render(renderer, utils::string_format("Cash:%d€", world->getPlayer().cash), fontColor, 150, 10);
        font->render(renderer, utils::string_format("Points:%d", world->getPlayer().points), fontColor, 400, 10);
        font->render(renderer, utils::string_format("Wave:%d Next in %d sec", world->getCurrentWave(), world->getWaveTimerInSeconds()), fontColor, 650, 10);

        graphics::Rect buildBG{0, 0, 150, renderer->getMainCamera()->getHeight()};
        renderer->fillRect(buildBG);

        if (world->isWorldFinished())
        {
            renderer->setDrawColor(0, 0, 0, 255 * 1 / (fadeOutTimer + 1));
            renderer->fillRect(fadeRect);
        }
    }

    void WorldScene::saveHighScore(world::Player player, std::string name)
    {

        utils::JSON::Parser parser;

        std::ifstream file;

        std::string s;
        std::string group;

        file.open("data/highscore.json", std::ios::in);
        if (!file.is_open())
        {
            throw IOException("data/highscore.json", "file does not exists");
        }
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);

        std::vector<HighScore> highScores;
        auto array = parser.parseArray(buffer);
        for (auto &data : array)
        {
            auto score = std::get<std::shared_ptr<utils::JSON::Object>>(data);
            highScores.push_back({score->getStringValue("name"), score->getIntValue("score")});
        }

        highScores.push_back({name, player.points});

        std::sort(highScores.begin(), highScores.end(), [](HighScore &h1, HighScore &h2)
                  { return h1.points > h2.points; });

        array.clear();
        for (auto &highScore : highScores)
        {
            std::shared_ptr<utils::JSON::Object> object = std::make_shared<utils::JSON::Object>();
            object->setAttribute("name", highScore.name);
            object->setAttribute("score", highScore.points);
            array.push_back(object);
        }
        std::string result = convertJsonArrayToString(array);
        std::ofstream ostream("data/highscore.json", std::ios::trunc | std::ios::out);
        ostream << result;
        ostream.flush();
        ostream.close();
    }

} // namespace scenes
