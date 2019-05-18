#pragma once

#include <SDL2/SDL.h>

class App
{
public:
    //
    SDL_Event     m_window_event;
    SDL_Texture  *m_texture;
    SDL_Rect      m_main_rect;
    SDL_Window   *m_window;
    SDL_Renderer *m_window_renderer;
    //
    ADBDriver m_adb;
    ADBDriver::Swipe_s m_adb_rect;

    App();
    ~App();

    void run();
    void loop();
    void draw();
    void screen();
    void jointh();
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t) noexcept;

private:
    //
    AppMenu       m_appmenu;
    std::thread   m_thu;
    std::atomic<bool> m_adbinit;
};
