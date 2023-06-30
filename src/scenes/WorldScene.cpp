#include "WorldScene.h"
#include "world/WorldLoader.h"
#include "engine/graphics/TextureManager.h"
#include "engine/utils/string.h"

namespace scenes
{
    WorldScene::WorldScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, [[maybe_unused]] std::shared_ptr<utils::IniBase> settings, [[maybe_unused]] core::Input *input)
        : core::Scene(pRenderer), sceneManager(pSceneManager)
    {
        world::WorldLoader worldLoader;

        world = worldLoader.loadWorld("maps/test.json.tmj");
        towerBase = graphics::TextureManager::Instance().loadTexture("images/Tower.png");
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 16);
    }

    WorldScene::~WorldScene()
    {
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        mousePos = pInput->getMousePostion();

        graphics::Rect towerPos = buildTowerRect();
        if (world->canBuildTower(towerPos) && pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
        {
            world->buildTower({towerPos.x, towerPos.y}, world::TowerType::Canon); // TODO
            cash -= 10;
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

        drawUI();
    }

    void WorldScene::drawUI()
    {
        graphics::Rect background{0, 0, renderer->getMainCamera()->getWidth(), 50};

        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer->setDrawColor(22, 22, 22, 100);
        renderer->fillRect(background);
        SDL_Color fontColor{0, 255, 0, 255};
        font->render(renderer, utils::string_format("Cash: %d €", cash), fontColor, 50, 10);
    }

} // namespace scenes
