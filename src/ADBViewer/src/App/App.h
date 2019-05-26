#pragma once

#include "../ADBViewer.h"

class App : public guiMain, public guiBase
{
public:
    //
    guiStaticText m_appinfo;
    guiBoxTextInput m_appinput;
    //
    AppVideo      m_appvideo;
    AppMenuBar    m_appmenubar;
    AppMenuPopUp  m_appmenupop;

    App();
    ~App();

    void loop();
    bool event(SDL_Event*, const void*) override;

private:
    //
};
