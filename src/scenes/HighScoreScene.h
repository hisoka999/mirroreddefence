#pragma once

#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <engine/ui/Container.h>
#include <vector>
#include <memory>
#include <ui/windows/SettingsWindow.h>

namespace scenes
{
    struct HighScore
    {
        std::string name;
        int points;
    };

    class HighScoreScene : public core::Scene
    {
    private:
        std::vector<HighScore> highScores;
        core::SceneManager *sceneManager;
        std::shared_ptr<graphics::Text> font;
        std::shared_ptr<graphics::Text> titleFont;

    public:
        HighScoreScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager);
        virtual ~HighScoreScene();
        void load() override;
        void render() override;
        bool handleEvents(core::Input *pInput) override;
    };
}
