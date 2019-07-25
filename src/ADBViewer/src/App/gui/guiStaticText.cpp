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

guiStaticText::guiStaticText() :
    m_iswrite(false) {}

bool guiStaticText::init(
            App *app,
            ResManager::IndexFontResource ifx,
            ResManager::IndexColorResource icx
        )
    {
        if (!(m_font = ResManager::fontload(ifx)))
            return false;

        if (!(m_color = ResManager::colorload(icx)))
            return false;

        return guiBase::initgui(app);
    }

void guiStaticText::draw(std::string const & s, SDL_Point *offset, int32_t id)
    {
        if ((!guiBase::GetGui<guiMain>()) || (!m_font) || (!m_color))
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
        SDL_Rect    *r = guiBase::GetGui<SDL_Rect>();

        do
        {
            if (!(surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                s.c_str(),
                                m_color[0],
                                m_color[1]
                            )))
                break;

            r->w = surface->w;
            r->h = surface->h;
            r->x = offset->x;
            r->y = offset->y;

            if ((tmp_texture = SDL_CreateTextureFromSurface(
                                    guiBase::GetGui<SDL_Renderer>(),
                                    surface)
                 ))
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

void guiStaticText::clear()
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

        guiBase::ActiveOn();
    }
