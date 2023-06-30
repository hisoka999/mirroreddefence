#pragma once
#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <engine/ui/Container.h>
#include <vector>
#include <memory>
#include "world/World.h"

namespace scenes
{

    class WorldScene : public core::Scene
    {
    public:
        WorldScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, std::shared_ptr<utils::IniBase> settings, core::Input *input);
        virtual ~WorldScene();
        void render();
        bool handleEvents(core::Input *pInput);

        virtual void load();

    private:
        void drawUI();
        graphics::Rect buildTowerRect();
        core::SceneManager *sceneManager;
        UI::Container container;
        std::unique_ptr<world::World> world;
        utils::Vector2 mousePos;
        std::shared_ptr<graphics::Texture> towerBase;
        std::shared_ptr<graphics::Text> font;
        int cash = 20;
    };

}