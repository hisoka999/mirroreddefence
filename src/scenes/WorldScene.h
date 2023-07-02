#pragma once
#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <engine/ui/Container.h>
#include <vector>
#include <memory>
#include "world/World.h"
#include "world/Sprite.h"
#include <optional>
#include "world/WorldLoader.h"
#include "ui/windows/EnterNameWindow.h"

namespace UI
{
    class Container;
};

namespace scenes
{

    class WorldScene : public core::Scene
    {
    public:
        WorldScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, std::shared_ptr<utils::IniBase> settings, core::Input *input);
        virtual ~WorldScene();
        void render() override;
        void update() override;
        void fixedUpdate(uint32_t delta) override;
        bool handleEvents(core::Input *pInput) override;

        virtual void load();

    private:
        void drawUI();
        void initUI();
        graphics::Rect buildTowerRect();
        void saveHighScore(world::Player player, std::string name);
        core::SceneManager *sceneManager;
        UI::Container container;
        std::unique_ptr<world::World> world;
        utils::Vector2 mousePos;
        std::shared_ptr<graphics::Texture> towerBase;
        std::shared_ptr<graphics::Text> font;
        int cash = 20;
        std::optional<world::TowerType> towerToBuild;
        bool upgrade = false;
        world::WorldLoader worldLoader;
        std::shared_ptr<windows::EnterNameWindow> enterNameWindow;
        bool fadeOutTimer = 0;
    };

}