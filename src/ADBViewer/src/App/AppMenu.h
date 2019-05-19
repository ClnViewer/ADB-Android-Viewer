#pragma once

#include <SDL2/SDL.h>

class App;

class AppMenu
{
public:
    SDL_Texture  *m_menu_texture;
    SDL_Rect      m_menu_rect;
    //
    std::atomic<bool> m_isrun;
    std::atomic<bool> m_isstop;
    std::atomic<bool> m_ispos;
    std::atomic<bool> m_isfullscreen;
    std::atomic<uint32_t> m_scale;
    std::atomic<uint32_t> m_compress;

    AppMenu();
    ~AppMenu();

    bool init(App*);
    bool mouse();
    bool screenshot();
    void setcursor(uint32_t);
#   if !defined (_BUILD_FRAME_NO_TITLE)
    void titleset(MgrType, std::string const&);
#   endif

private:
    //
    App *m_app;
    SDL_Cursor *m_cursor[2];
    //
    ResManager::IndexStringResource clickpos(int32_t, int32_t, int32_t);
    void mousemove();
    void mousebutton();

    bool savefile(std::string&);
    bool openfile(std::string&);
};
