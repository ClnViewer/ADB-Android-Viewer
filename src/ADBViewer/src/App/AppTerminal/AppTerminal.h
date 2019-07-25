#pragma once

#include "../../ADBViewer.h"

class App;

class AppTerminal : public guiBase
{
public:
    //
    AppTerminal();

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
    AppTerminalPage   m_page;
    guiIcon           m_icon_close;
    std::mutex        m_lock;
    std::atomic<bool> m_enable;
    //
    void runselect(bool);
    void adbsend(std::string&);
    void resizewin(int32_t);
};

