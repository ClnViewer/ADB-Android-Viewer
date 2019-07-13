#pragma once

#include "../ADBViewer.h"

class App;

class AppMenuBar : public guiBase
{
public:
    //
    ~AppMenuBar();

    bool init(App*);
    bool uevent(SDL_Event*, const void*) override;
    bool isenabled();

    // CMD
    int32_t mb_cmd_QUIT(SDL_Event*);
    int32_t mb_cmd_START(SDL_Event*);
    int32_t mb_cmd_STOP(SDL_Event*);
    int32_t mb_cmd_ADBSET(SDL_Event*);
    int32_t mb_cmd_SCALE(SDL_Event*);
    int32_t mb_cmd_POSINFO(SDL_Event*);
    int32_t mb_cmd_CAPTURE_D(SDL_Event*);
    int32_t mb_cmd_FULLSCREEN(SDL_Event*);
    int32_t mb_cmd_APK(SDL_Event*);

private:
    //
    App        *m_app = nullptr;
    guiIcon     m_icon_menu[2];
    uint32_t    m_icon_idx = 0;
    SDL_Color   m_color[2]{};
    //
    bool iconstate();
    bool eventcb(SDL_Event*);

};
