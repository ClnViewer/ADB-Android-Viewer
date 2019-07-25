#pragma once

#include "../ADBViewer.h"

class App;

class AppVideo : public guiBase
{
public:
    //
    AppVideo();
    ~AppVideo();

    bool init(App*);
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
    void run();
    void stop();
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;

private:
    //
    App           *m_app;
    guiIconTrigger m_icon_keyboard;
    std::thread    m_thu;
    std::mutex     m_lockrun;
    SDL_Point      m_psize{};
    SDL_Color      m_color = { 0x0, 0x0, 0x0, 0x0 };

    //
    void jointh();
    bool defscreen();
    bool defscreen(SDL_Texture**);
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t);
    //
};
