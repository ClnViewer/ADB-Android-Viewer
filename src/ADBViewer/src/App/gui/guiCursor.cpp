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

guiCursor::~guiCursor()
    {
        guiBase::ActiveOff();
        if (m_timer.isactive())
            m_timer.stop();
    }

bool guiCursor::init(
            App *app,
            SDL_Point point,
            ResManager::IndexFontResource ifx,
            ResManager::IndexColorResource icx
        )
    {
        if (!app)
            return false;

        bool ret  = false;
        m_app = ((!m_app) ? app : m_app);
        m_point = point;

        do
        {
            if (!(m_font = ResManager::fontload(ifx)))
                break;

            if (!(m_color = ResManager::colorload(icx)))
                break;

            ret = guiBase::initgui(app);
        }
        while (0);

        guiBase::ActiveOff();
        return ret;
    }

bool guiCursor::tinit(SDL_Texture **texture)
    {
        bool ret = false;
        SDL_Surface *surface = nullptr;

        do
        {
            if (!(surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                static_cast<const char*>("|"),
                                m_color[0],
                                m_color[1]
                            )))
                break;

            SDL_Rect *r = guiBase::GetGui<SDL_Rect>();

            r->w = surface->w;
            r->h = surface->h;
            r->x = m_point.x + surface->w;
            r->y = m_point.y;

            if (!(*texture = SDL_CreateTextureFromSurface(
                                guiBase::GetGui<SDL_Renderer>(),
                                surface)
                  ))
                break;

            ret = true;
        }
        while (0);

        if (surface)
            SDL_FreeSurface(surface);

        return ret;
    }

bool guiCursor::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        return true;
    }

void guiCursor::setcord(SDL_Point & p)
    {
        m_point = p;
    }

void guiCursor::Off()
    {
        guiBase::ActiveOff();

        if (m_timer.isactive())
            m_timer.stop();
        m_pos = 0;
    }

void guiCursor::On()
    {
        if ((!m_font) || (m_timer.isactive()))
            return;

        m_timer.loop(
            0U,
            0U,
            5U,
            [=]()
            {
                if (guiBase::IsActive())
                    guiBase::ActiveOff();
                else
                    guiBase::ActiveOn();
            }
        );
    }

void guiCursor::setpos(int32_t p)
    {
        m_pos = p;
    }

int32_t guiCursor::getpos()
    {
        return m_pos.load();
    }

void guiCursor::setstring(guiCursor::IndexTextType t, std::string const & s, int32_t pos)
    {
        m_pos = pos;
        setstring(t, s);
    }

void guiCursor::setstring(guiCursor::IndexTextType t, std::string const & s)
    {
        SDL_Rect *r = guiBase::GetGui<SDL_Rect>();

        do
        {
            int32_t w, h, ret = -1;
            std::string sdraw;

            if (s.empty())
                break;

            if (m_pos > s.length())
                m_pos = s.length();

            sdraw = s.substr(0, m_pos);

            if (sdraw.empty())
                break;

            switch (t)
            {
                case guiCursor::IndexTextType::TEXT_ANSI:
                    {
                        ret = TTF_SizeText(m_font, sdraw.c_str(), &w, &h);
                        break;
                    }
                case guiCursor::IndexTextType::TEXT_UTF8:
                    {
                        ret = TTF_SizeUTF8(m_font, sdraw.c_str(), &w, &h);
                        break;
                    }
                case guiCursor::IndexTextType::TEXT_UNIC:
                    {
                        /// TODO
                        //ret = TTF_SizeUNICODE(m_font, sdraw.c_str(), &w, &h);
                        break;
                    }
            }
            if (ret)
                break;

            r->x = m_point.x + w;
            r->y = m_point.y;
            return;
        }
        while (0);

        r->x = m_point.x;
        r->y = m_point.y;
        return;
    }


