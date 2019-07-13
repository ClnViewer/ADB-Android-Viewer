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

bool guiIcon::init(
        App *app,
        SDL_Rect & rect,
        ResManager::IndexImageResource resid,
        std::function<bool(SDL_Event*, SDL_Rect*)> f
    )
    {
        if (!app)
            return false;

        m_app = app;
        m_resid = resid;
        m_func = f;

        guiBase::gui.rdst = rect;
        guiBase::gui.rsrc = nullptr;
        guiBase::gui.texture = nullptr;

        bool ret = guiBase::initgui(app);
        guiBase::ActiveOff();
        return ret;
    }

bool guiIcon::tinit(SDL_Texture **texture)
    {
        GuiLock(
            if ((texture) && (*texture))
                SDL_DestroyTexture(*texture);
            *texture = nullptr;
        );
        return true;
    }

bool guiIcon::evresize(SDL_Texture **texture)
    {
        Off();
        On();
        return true;
    }

void guiIcon::On()
    {
        On(nullptr);
    }

void guiIcon::On(SDL_Point *p)
    {
        SDL_Surface *l_image_surface = nullptr;
        SDL_Texture *l_texture = nullptr;

        do
        {
            if (!(l_image_surface = ResManager::imageload(m_resid)))
                break;

            SDL_Rect *r = guiBase::GetGui<SDL_Rect>();
            r->w = l_image_surface->w;
            r->h = l_image_surface->h;

            l_texture = SDL_CreateTexture(
                guiBase::GetGui<SDL_Renderer>(),
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                r->w,
                r->h
            );

            if (!l_texture)
                break;

            if (p)
            {
                r->x = p->x;
                r->y = p->y;
            }

            SDL_UpdateTexture(l_texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
            SDL_FreeSurface(l_image_surface);

            GuiLock(
                std::swap(guiBase::gui.texture, l_texture);
            );
            if (l_texture)
                SDL_DestroyTexture(l_texture);

            guiBase::ActiveOn();
            return;
        }
        while (0);

        if (l_image_surface)
            SDL_FreeSurface(l_image_surface);
        if (l_texture)
            SDL_DestroyTexture(l_texture);

        guiBase::ActiveOff();
    }

void guiIcon::Off()
    {
        guiBase::ActiveOff();
        SDL_Texture *l_texture = nullptr;
        GuiLock(
            std::swap(guiBase::gui.texture, l_texture);
        );
        if (l_texture)
            SDL_DestroyTexture(l_texture);
    }

bool guiIcon::event(SDL_Event *ev, const void *instance)
{
    bool isreg = false;
    guiIcon *gi = static_cast<guiIcon*>(
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
