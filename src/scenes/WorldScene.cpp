#include "WorldScene.h"
#include "engine/graphics/TextureManager.h"
#include "engine/utils/string.h"
#include "engine/core/ecs/Component.h"
#include "engine/ui/Container.h"
#include "engine/ui/ImageButton.h"
#include "engine/ui/Button.h"
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
    }

    WorldScene::~WorldScene()
    {
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        if (container.handleEvents(pInput))
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
            world = worldLoader.loadWorld("maps/level" + std::to_string(nextLevel) + ".tmj", nextLevel);
            world->setPlayer(player);
        }
    }

    void WorldScene::drawUI()
    {
        graphics::Rect background{0, 0, renderer->getMainCamera()->getWidth(), 50};

        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer->setDrawColor(22, 22, 22, 100);
        renderer->fillRect(background);
        SDL_Color fontColor{0, 255, 0, 255};
        font->render(renderer, utils::string_format("Cash: %d €", world->getPlayer().cash), fontColor, 150, 10);
        font->render(renderer, utils::string_format("Points: %d", world->getPlayer().points), fontColor, 400, 10);
        font->render(renderer, utils::string_format("Wave: %d Next in %d seconds", world->getCurrentWave(), world->getWaveTimerInSeconds()), fontColor, 600, 10);

        graphics::Rect buildBG{0, 0, 150, renderer->getMainCamera()->getHeight()};
        renderer->fillRect(buildBG);
    }

} // namespace scenes
