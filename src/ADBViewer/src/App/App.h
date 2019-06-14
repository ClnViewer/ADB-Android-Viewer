#pragma once

#include "../ADBViewer.h"

class App : public guiMain, public guiBase
{
public:
    //
    guiBoxTextInput m_appinput;
    //
    AppVideo          m_appvideo;
    AppEditor         m_appeditor;
    AppMenuBar        m_appmenubar;
    AppMenuPopUp      m_appmenupop;
    AppMessageBar     m_appmsgbar;
    AppVideoAnimation m_appani;

    App();
    ~App();

    void loop();
    bool event(SDL_Event*, const void*) override;

private:
    //
};
