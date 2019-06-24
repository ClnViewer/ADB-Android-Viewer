#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class guiStaticText : public guiBase
{
public:
    //
    bool init(
            guiMain *gm,
            ResManager::IndexFontResource ifx,
            ResManager::IndexColorResource icx
        ) noexcept
    {
        SDL_RWops *rwops;
        if (!(rwops = ResManager::fontload(ifx)))
            return false;

        if (!(m_font = TTF_OpenFontRW(rwops, 1, 16)))
            return false;

        if (!(m_color = ResManager::colorload(icx)))
            return false;

        return initgui(gm);
    }

    void draw(std::string const & s, SDL_Point *offset, int32_t id)
    {
        if ((!getgui()) || (!m_font) || (!m_color))
            return;

        if ((id > 0) && (m_id == id))
            return;

        while (m_iswrite.load())
            std::this_thread::yield();

        m_iswrite = true;
        m_id = id;
        guiBase::ActiveOff();
        SDL_Texture *tmp_texture = nullptr;

        GuiLock(
            std::swap(guiBase::gui.texture, tmp_texture);
        );

        if (tmp_texture)
            SDL_DestroyTexture(tmp_texture);

        SDL_Surface *surface = nullptr;

        do
        {
            if (!(surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                s.c_str(),
                                m_color[0],
                                m_color[1]
                            )))
                break;

            gui.rect.w = surface->w;
            gui.rect.h = surface->h;
            gui.rect.x = offset->x;
            gui.rect.y = offset->y;

            if ((tmp_texture = SDL_CreateTextureFromSurface(getgui()->m_renderer, surface)))
            {
                GuiLock(
                    std::swap(guiBase::gui.texture, tmp_texture);
                );
                guiBase::ActiveOn();
            }
        }
        while (0);

        if (surface)
            SDL_FreeSurface(surface);

        m_iswrite = false;
    }

    void clear()
    {
        if (!gui.texture)
            return;

        m_id = -1;
        guiBase::ActiveOff();
        SDL_Texture *tmp_texture = nullptr;

        GuiLock(
            std::swap(guiBase::gui.texture, tmp_texture);
        );

        if (tmp_texture)
            SDL_DestroyTexture(tmp_texture);
    }

    guiStaticText()
     : m_id(-1), m_font(nullptr), m_color(nullptr), m_iswrite(false) {}
    ~guiStaticText()
    {
        if (m_font)
            TTF_CloseFont(m_font);
    }

private:
    //
    int32_t     m_id;
    TTF_Font   *m_font;
    SDL_Color  *m_color;
    std::atomic<bool> m_iswrite;
    //
};
