#include "EnterNameWindow.h"
#include "engine/ui/Button.h"
#include "engine/ui/TextItem.h"

namespace windows
{
    EnterNameWindow::EnterNameWindow() : UI::Window(100, 100, 400, 100)
    {
        setTitle("ENTER YOUR NAME");
        auto nameInput = std::make_shared<UI::TextItem>(this, 200, 30);
        nameInput->connect("textChanged", [this](std::string text)
                           { this->playerName = text; });
        nameInput->setFont("fonts/PressStart2P-Regular.ttf", 14);
        addObject(nameInput);
        auto okButton = std::make_shared<UI::Button>(this);
        okButton->setFont("fonts/PressStart2P-Regular.ttf", 14);
        okButton->setLabel("OK");
        okButton->setPos(210, 0);
        okButton->connect("buttonClick", [this]()
                          { this->setVisible(false); });
        addObject(okButton);
    }

    EnterNameWindow::~EnterNameWindow()
    {
    }

    std::string &EnterNameWindow::getName()
    {
        return playerName;
    }
} // namespace windows
