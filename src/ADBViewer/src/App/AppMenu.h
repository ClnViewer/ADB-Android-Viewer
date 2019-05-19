#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class App;

class AppMenu
{
public:
    SDL_Texture  *m_menu_texture;
    SDL_Texture  *m_info_texture;
    SDL_Rect      m_menu_rect;
    SDL_Rect      m_info_rect;
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
    void infoset(MgrType, std::string const&);

private:
    //
    App *m_app;
    SDL_Color   m_color[2];
    SDL_Cursor *m_cursor[2];
    TTF_Font   *m_font;
    //
    static inline SDL_Color txt_color[] =
    {
        { 191, 227, 103, 0 },
        { 2, 2, 2, 150 }
    };
    //
    ResManager::IndexStringResource clickpos(int32_t, int32_t, int32_t);
    void mousemove();
    void mousebutton();

    void drawtitle(std::string const&);
    void drawtext(std::string const&);
    void drawclear();

    bool savefile(std::string&);
    bool openfile(std::string&);
};
