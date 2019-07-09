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

AppCursor::~AppCursor()
    {
        for (uint32_t i = 0U; i < __NELE(m_cursor); i++)
            if (m_cursor[i])
            {
                SDL_FreeCursor(m_cursor[i]);
                m_cursor[i] = nullptr;
            }
    }

bool AppCursor::init(App *app)
    {
        static const SDL_SystemCursor cursor_array[] =
        {
            SDL_SYSTEM_CURSOR_HAND,
            SDL_SYSTEM_CURSOR_ARROW
        };

        if (!app)
            return false;

        m_app = app;
        guiBase::gui.rect = {};
        guiBase::gui.texture = nullptr;

        if (!guiBase::initgui(app))
            return false;

        for (uint32_t i = 0U; i < __NELE(cursor_array); i++)
            if (!(m_cursor[i] = SDL_CreateSystemCursor(cursor_array[i])))
                return false;

        Set(AppCursor::CursorType::APP_CURSOR_ARROW);
        return true;
    }

void AppCursor::Set(AppCursor::CursorType id)
    {
        if (id >= __NELE(m_cursor))
            return;

        if (id == m_idx)
            return;

        SDL_SetCursor(m_cursor[id]);
        m_idx = id;
    }

bool AppCursor::uevent(SDL_Event *ev, const void *instance)
    {
        AppCursor *ac = static_cast<AppCursor*>(
                const_cast<void*>(instance)
            );

        if (!ac)
            return false;

        switch(ev->user.code)
        {
            /// Change cursor (SDL_SYSTEM_CURSOR_ARROW)
            case ID_CMD_POP_MENU25:
                {
                    ac->Set(AppCursor::CursorType::APP_CURSOR_ARROW);
                    return true;
                }
            /// Change cursor (SDL_SYSTEM_CURSOR_HAND)
            case ID_CMD_POP_MENU26:
                {
                    ac->Set(AppCursor::CursorType::APP_CURSOR_HAND);
                    return true;
                }
        }
        return false;
    }

