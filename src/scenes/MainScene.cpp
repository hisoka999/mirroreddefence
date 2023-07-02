/*
 * MainScene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "MainScene.h"
#include <engine/ui/Button.h>
#include <functional>
#include <iostream>
#include <vector>
#include "translate.h"
#include <engine/utils/os.h>
#include <engine/core/gamewindow.h>
#include <engine/utils/os.h>
#include <engine/utils/json/parser.h>
#include <fstream>

namespace scenes
{

        MainScene::MainScene(core::Renderer *pRenderer,
                             core::SceneManager *pSceneManager, std::shared_ptr<utils::IniBase> settings, core::Input *input)
            : core::Scene(pRenderer), running(true), sceneManager(pSceneManager), settingsWindow(settings, input)
        {

                bgTexture.loadTexture(renderer, "images/title_background.png");

                // this->music->loadMusic("music/Juhani Junkala [Retro Game Music Pack] Title Screen.wav");
                int volume = settings->getValueI("Volume", "Music");
                this->music->setVolume(volume);
                winMgr->addContainer(&container);

                SDL_Color white =
                    {255, 255, 255, 0};
                auto btnStart = std::make_shared<UI::Button>();
                btnStart->setFont("fonts/PressStart2P-Regular.ttf", 14);
                btnStart->setColor(white);
                btnStart->setLabel(_("New Game"));
                btnStart->setPos(450, 350);
                btnStart->setStaticWidth(150);
                container.addObject(btnStart);

                btnStart->connect(UI::Button::buttonClickCallback(), [&]()
                                  { startGame(); });

                auto btnHighScore = std::make_shared<UI::Button>();
                btnHighScore->setFont("fonts/PressStart2P-Regular.ttf", 14);
                btnHighScore->setColor(white);
                btnHighScore->setLabel(_("Highscore"));
                btnHighScore->setPos(450, 400);
                btnHighScore->setStaticWidth(150);

                btnHighScore->connect(UI::Button::buttonClickCallback(), [&]()
                                      { showHighScore(); });

                container.addObject(btnHighScore);

                auto btnSettings = std::make_shared<UI::Button>();
                btnSettings->setFont("fonts/PressStart2P-Regular.ttf", 14);
                btnSettings->setColor(white);
                btnSettings->setLabel(_("Settings"));
                btnSettings->setPos(450, 450);
                btnSettings->setStaticWidth(150);

                btnSettings->connect(UI::Button::buttonClickCallback(), [&]()
                                     { settingsWindow.setVisible(true); });

                container.addObject(btnSettings);

                auto btnExit = std::make_shared<UI::Button>();
                btnExit->setFont("fonts/PressStart2P-Regular.ttf", 14);
                btnExit->setColor(white);
                btnExit->setLabel(_("Exit Game"));
                btnExit->setPos(450, 500);
                btnExit->setStaticWidth(150);

                btnExit->connect(UI::Button::buttonClickCallback(), [&]()
                                 { exitGame(); });

                container.addObject(btnExit);
                winMgr->addWindow(&settingsWindow);
        }
        void MainScene::render()
        {
                bgTexture.renderResized(renderer, 0, 0,
                                        renderer->getMainCamera()->getWidth(),
                                        renderer->getMainCamera()->getHeight());

                winMgr->render(renderer);
        }

        void MainScene::exitGame()
        {
                running = false;
        }

        void MainScene::startGame()
        {
                APP_LOG_INFO("start game");

                sceneManager->setCurrentScene("worldScene");
        }
        void MainScene::showHighScore()
        {
                sceneManager->setCurrentScene("highScore");
        }

        bool MainScene::handleEvents(core::Input *pInput)
        {
                return winMgr->handleInput(pInput);
        }

        void MainScene::load()
        {
                music->play(-1);
        }

        MainScene::~MainScene()
        {
        }

} /* namespace scenes */
