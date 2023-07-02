#include "HighScoreScene.h"
#include "engine/utils/json/parser.h"
#include <fstream>

namespace scenes
{
    HighScoreScene::HighScoreScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager)
        : core::Scene(pRenderer), sceneManager(pSceneManager)
    {
        font = graphics::TextureManager::Instance().loadFont("fonts/PressStart2P-Regular.ttf", 20);
        titleFont = graphics::TextureManager::Instance().loadFont("fonts/PressStart2P-Regular.ttf", 34);
    }

    bool HighScoreScene::handleEvents([[maybe_unused]] core::Input *pInput)
    {
        if (pInput->isKeyDown("MENU"))
            sceneManager->setCurrentScene("main");

        return false;
    }

    HighScoreScene::~HighScoreScene()
    {
    }

    void HighScoreScene::load()
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

        highScores.clear();
        auto array = parser.parseArray(buffer);
        for (auto &data : array)
        {
            auto score = std::get<std::shared_ptr<utils::JSON::Object>>(data);
            highScores.push_back({score->getStringValue("name"), score->getIntValue("score")});
        }
    }

    void HighScoreScene::render()
    {
        SDL_Color titleColor{255, 255, 0, 255};
        titleFont->render(renderer, "HIGHSCORE", titleColor, 500, 100);
        int y = 200;
        for (auto &highScore : highScores)
        {
            font->render(renderer, highScore.name, {255, 255, 255, 255}, 200, y);
            font->render(renderer, std::to_string(highScore.points), {255, 255, 255, 255}, 500, y);
            y += 30;
        }
    }

} // namespace scenes