#pragma once

#include "engine/ui/Window.h"

namespace windows
{
    class EnterNameWindow : public UI::Window
    {
    private:
        std::string playerName;

    public:
        EnterNameWindow();
        ~EnterNameWindow();
        std::string &getName();
    };

} // namespace windows
