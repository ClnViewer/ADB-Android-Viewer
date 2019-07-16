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

#include "../ADBViewer.h"

bool AppTerminalPageNuber::init(App *app, AppTerminalPage *atp, std::function<void()> f)
    {
        if ((!app) || (!atp))
            return false;

        m_app = app;
        m_page = atp;
        m_func = f;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = nullptr;

        bool ret = guiBase::initgui(app);
        do
        {
            if (!ret)
                break;

            ret = ((m_font = ResManager::fontload(
                        ResManager::IndexFontResource::RES_FONT_CONSOLAS
                )));
            if (!ret)
                break;

            ret = ((m_color = ResManager::colorload(
                        ResManager::IndexColorResource::RES_COLOR_TERMINAL_NUM
                )));
        }
        while (0);

        guiBase::ActiveOff();
        return ret;
    }

bool AppTerminalPageNuber::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        GuiLock(
            SDL_DestroyTexture(*texture);
        );
        *texture = nullptr;
        return false;
    }

void AppTerminalPageNuber::stop()
    {
        guiBase::ActiveOff();
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        if (!gr->texture)
            return;

        GuiLock(
            SDL_DestroyTexture(gr->texture);
        );
        gr->texture = nullptr;

    }

void AppTerminalPageNuber::draw()
    {
        SDL_Surface *l_surface = nullptr;
        SDL_Texture *l_texture = nullptr;
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        do
        {
            if (!m_page->out_tpage)
                break;

            ///
            {
            std::stringstream ss;
            ss << " " << (m_page->out_npage + 1) << " ";

            if (ss.str().empty())
                break;

            if (!(l_surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                ss.str().c_str(),
                                m_color[0],
                                m_color[1]
                )))
                break;
            }
            ///

            SDL_Rect *r = guiBase::GetGui<SDL_Rect>();
            r->w = l_surface->w;
            r->h = l_surface->h;
            r->x = m_page->c_p_pnum.x;
            r->y = m_page->c_p_pnum.y;

            l_texture = SDL_CreateTextureFromSurface(
                guiBase::GetGui<SDL_Renderer>(),
                l_surface
            );
            SDL_FreeSurface(l_surface); l_surface = nullptr;

            if (!l_texture)
            {
                SDLErrorMessageQueue();
                break;
            }

            GuiLock(
                std::swap(gr->texture, l_texture);
            );
            if (l_texture)
                SDL_DestroyTexture(l_texture);

            guiBase::ActiveOn();
            return;
        }
        while (0);

        guiBase::ActiveOff();

        if (l_surface)
            SDL_FreeSurface(l_surface);

        if (l_texture)
            SDL_DestroyTexture(l_texture);

        if (gr->texture)
        {
            GuiLock(
                SDL_DestroyTexture(gr->texture);
                gr->texture = nullptr;
            );
        }
    }

    /*
bool AppTerminalPageNuber::event(SDL_Event *ev, const void *instance)
    {
        AppTerminalPageNuber *atpn = static_cast<AppTerminalPageNuber*>(
                const_cast<void*>(instance)
            );

        if ((!atpn) || (!m_func))
            return false;

        if (!guiBase::IsActive())
            return false;

        if (ev->type == SDL_MOUSEMOTION)
        {
            if (!atpn->guiBase::IsRegion(ev, atpn->guiBase::GetGui<SDL_Rect>()))
                return false;
            atpn->guiBase::PushEvent(ID_CMD_POP_MENU27);
            return true;
        }
        if ((ev->type == SDL_MOUSEBUTTONDOWN) && (ev->button.button == SDL_BUTTON_RIGHT))
        {
            if (!atpn->guiBase::IsRegion(ev, atpn->guiBase::GetGui<SDL_Rect>()))
                return false;

            atpn->m_func();
            return true;
        }
        return false;
    }
*/
