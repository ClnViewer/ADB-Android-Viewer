#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class App;

class guiStaticText : public guiBase
{
public:
    //
    guiStaticText();
    guiStaticText& operator=(guiStaticText const&) = delete;
    //
    bool init(App*, ResManager::IndexFontResource, ResManager::IndexColorResource);
    void draw(std::string const &, SDL_Point*, int32_t);
    void clear();

private:
    //
    int32_t     m_id = -1;
    TTF_Font   *m_font = nullptr;  // auto remove from cache in DLL
    SDL_Color  *m_color = nullptr;
    std::atomic<bool> m_iswrite;
    //
};
