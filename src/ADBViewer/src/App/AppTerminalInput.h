#pragma once

#include "../ADBViewer.h"

class App;

class AppTerminalInput : public guiTextInputBox
{
public:
    //
    bool init(App*, SDL_Rect&);
    bool isenabled();
    void start(SDL_Point*);
    void start();
    void stop();

private:
    //
    App        *m_app;
    guiIcon     m_icon_editmenu;
    //
};

