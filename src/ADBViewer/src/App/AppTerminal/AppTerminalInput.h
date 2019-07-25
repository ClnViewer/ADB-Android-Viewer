#pragma once

#include "../../ADBViewer.h"

class App;

class AppTerminalInput : public guiTextInputBox
{
public:
    //
    bool init(App*, AppTerminalPage*, std::function<void(std::string&)>);
    bool isenabled();
    void start();
    void stop();

private:
    //
    App             *m_app;
    AppTerminalPage *m_page = nullptr;
    guiIcon          m_icon_editmenu;
    std::function<void(std::string&)> m_func;
    //
    bool mevent(SDL_Event*, SDL_Rect*);
};

