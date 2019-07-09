#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class App;

class guiCursor : public guiBase
{
public:
    //
    enum IndexTextType
    {
        TEXT_ANSI = 0,
        TEXT_UTF8,
        TEXT_UNIC
    };
    //
    ~guiCursor();

    bool init(App*, SDL_Point, ResManager::IndexFontResource, ResManager::IndexColorResource);
    int32_t getpos();
    void setpos(int32_t);
    void setstring(guiCursor::IndexTextType, std::string const &);
    void setstring(guiCursor::IndexTextType, std::string const &, int32_t);
    void setcord(SDL_Point&);
    void On();
    void Off();

protected:
    //
    App                  *m_app = nullptr;

private:
    //
    TTF_Font             *m_font;
    SDL_Color            *m_color;
    SDL_Point             m_point;
    std::atomic<uint32_t> m_pos = 0;
    AppTimer              m_timer;
    //
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;

};
