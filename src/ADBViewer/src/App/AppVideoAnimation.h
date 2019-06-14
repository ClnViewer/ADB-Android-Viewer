#pragma once

#include "../ADBViewer.h"

class App;

class AppVideoAnimation : public guiBase
{
public:
    //
    AppVideoAnimation();
    ~AppVideoAnimation();

    bool init(App*);
    void run();
    void stop();
    bool event(SDL_Event*, const void*) override;

private:
    //
    App              *m_app = nullptr;
    AppTimer          m_timer;
    SDL_Color         m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Texture     **m_textures = nullptr;
    uint32_t          m_textures_sz = 0U;
    int32_t           m_key_y = 0U;
    //
    void clean();
};
