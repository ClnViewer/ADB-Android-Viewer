#pragma once

#include "../ADBViewer.h"

class App : public guiMain, public guiBase
{
public:
    //
    AppInputText       m_appinput;
    AppVideo           m_appvideo;
    AppEditor          m_appeditor;
    AppMenuBar         m_appmenubar;
    AppMenuPopUp       m_appmenupop;
    AppMessageBar      m_appmsgbar;
    AppAnimationBender m_appabender;
    AppScreen          m_screen;
    AppTerminal        m_terminal;

    App();
    ~App();

    void loop();
    bool event(SDL_Event*, const void*) override;

private:
    //
};
