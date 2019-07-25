/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../../ADBViewer.h"

guiIconTrigger::~guiIconTrigger()
    {
        cleanup();
    }

bool guiIconTrigger::init(
        App *app,
        SDL_Rect & rect,
        SDL_Color *bcolor,
        ResManager::IndexSpriteResource resid,
        std::function<bool(SDL_Event*, SDL_Rect*)> f
    )
    {
        if (!app)
            return false;

        m_app = app;
        m_func = f;

        guiBase::gui.rdst = rect;
        guiBase::gui.rsrc = nullptr;
        guiBase::gui.texture = nullptr;

        bool ret = guiBase::initgui(app);
        guiBase::ActiveOff();

        do
        {
            if (!ret)
                break;

            SDL_Surface **l_sprites  = nullptr;
            if (
                (!(l_sprites = ResManager::spriteload(resid, bcolor, &m_textures_sz))) ||
                (!m_textures_sz)
                )
                break;

            SDL_Rect *r = guiBase::GetGui<SDL_Rect>();
            r->w = l_sprites[0]->w;
            r->h = l_sprites[0]->h;

            m_textures = new SDL_Texture*[m_textures_sz]{};

            for (uint32_t i = 0U; i < m_textures_sz; i++)
            {
                if (l_sprites[i])
                {
                    if (ret)
                        if (!(m_textures[i] = SDL_CreateTextureFromSurface(
                                guiBase::GetGui<SDL_Renderer>(),
                                l_sprites[i]
                            )))
                            ret = false;

                    SDL_FreeSurface(l_sprites[i]);
                    l_sprites[i] = nullptr;
                }
            }
            delete [] l_sprites;

            if (!ret)
                break;
            return true;
        }
        while (0);

        cleanup();
        return false;
    }

void guiIconTrigger::cleanup()
    {
        guiBase::gui.texture = nullptr;

        if ((!m_textures) || (!m_textures_sz))
            return;

        for (uint32_t i = 0U; i < m_textures_sz; i++)
            if (m_textures[i])
                SDL_DestroyTexture(m_textures[i]);

        delete [] m_textures;
        m_textures = nullptr;
    }

bool guiIconTrigger::tinit(SDL_Texture **texture)
    {
        GuiLock(
            if ((guiBase::IsActive()) && (m_textures) && (m_textures_sz))
                *texture = m_textures[((m_lastid < m_textures_sz) ? m_lastid : 0U)];
            else
                *texture = nullptr;
        );
        return true;
    }

bool guiIconTrigger::evresize(SDL_Texture **texture)
    {
        return tinit(texture);
    }

void guiIconTrigger::On()
    {
        On(nullptr, m_lastid);
    }

void guiIconTrigger::On(SDL_Point *p)
    {
        On(p, m_lastid);
    }

void guiIconTrigger::On(uint32_t idx)
    {
        On(nullptr, idx);
    }

void guiIconTrigger::On(SDL_Point *p, uint32_t idx)
    {
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        do
        {
            if ((!m_textures) || (!m_textures_sz))
                break;

            if (idx >= m_textures_sz)
                break;

            m_lastid = ((m_lastid != idx) ? idx : m_lastid);

            if (p)
            {
                gr->rdst.x = p->x;
                gr->rdst.y = p->y;
            }

            GuiLock(
                gr->texture = m_textures[m_lastid];
            );
            guiBase::ActiveOn();
            return;
        }
        while (0);

        gr->texture = nullptr;
        guiBase::ActiveOff();
    }

void guiIconTrigger::Off()
    {
        if (!guiBase::IsActive())
            return;

        guiBase::ActiveOff();
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();
        m_lastid = 0U;

        GuiLock(
            gr->texture = nullptr;
        );
    }

bool guiIconTrigger::event(SDL_Event *ev, const void *instance)
{
    bool isreg = false;
    guiIconTrigger *gi = static_cast<guiIconTrigger*>(
                const_cast<void*>(instance)
            );

    if ((!gi) || (!gi->m_func))
         return false;

    switch (ev->type)
    {
        case SDL_MOUSEMOTION: isreg = true; break;
        case SDL_MOUSEBUTTONDOWN: if (ev->button.button == SDL_BUTTON_LEFT) { isreg = true; } break;
        default: break;
    }
    if (isreg)
    {
        SDL_Rect *r = gi->guiBase::GetGui<SDL_Rect>();
        if (gi->guiBase::IsRegion(ev, r))
            return gi->m_func(ev, r);
    }
    return false;
}
