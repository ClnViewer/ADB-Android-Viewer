#pragma once

#include "../ADBViewer.h"

class App;

class AppTerminal : public guiBase
{
public:
    //
    bool init(App*);
    bool tinit(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;
    bool evresize(SDL_Texture**) override;
    bool isenabled() const;


private:
    //
    App              *m_app = nullptr;
    AppTerminalInput  m_tinput;
    AppTerminalOutput m_toutput;
    guiIcon           m_icon_close;
    SDL_Rect          m_rect{};
    std::atomic<bool> m_enable = false;
    std::mutex        m_lock;
    //
    bool getwcord();
    void runselect();
    void gui_icon_on();
    void gui_icon_off();
};

