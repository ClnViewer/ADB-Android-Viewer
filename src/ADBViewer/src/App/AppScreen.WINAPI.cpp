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

#if defined(OS_WIN)
#include  "SDL2/SDL_syswm.h"

void * AppScreen::getsysscreen(SDL_Surface *s)
    {
        HWND    l_hWnd = nullptr;
        HDC     l_hDC  = nullptr,
                l_wDC  = nullptr;
        HBITMAP l_hBmp = nullptr;

        do
        {
            if (!s)
                break;

            if (!(l_hWnd = AppSysDialog::gethwnd(m_app->guiMain::m_window)))
                break;

            if (!(l_wDC = ::GetDC(l_hWnd)))
                break;

            if (!(l_hDC = ::CreateCompatibleDC(l_wDC)))
                break;

            if (!(l_hBmp = ::CreateCompatibleBitmap(l_wDC, s->w, s->h)))
                break;

            {
                HGDIOBJ l_hold = ::SelectObject(l_hDC, l_hBmp);
                for (int32_t x = 0U; x < s->w; x++)
                    for (int32_t y = 0U; y < s->h; y++)
                    {
                        uint32_t pix = getpixel(s, x, y);
                        ::SetPixel(l_hDC, x, y, pix);
                    }
                ::SelectObject(l_hDC, l_hold);
            }
        }
        while (0);

        if (l_hDC)
            ::DeleteDC(l_hDC);
        if (l_wDC)
            ::ReleaseDC(l_hWnd, l_wDC);

        return l_hBmp;
    }

bool AppScreen::setclipboard(void *vsrc)
    {
        bool ret = false;
        HWND l_hWnd = nullptr;

        do
        {
            if (!vsrc)
                break;

            if (!(l_hWnd = AppSysDialog::gethwnd(m_app->guiMain::m_window)))
                break;

            if (
                (!::OpenClipboard(l_hWnd)) ||
                (!::EmptyClipboard())
               )
                break;

            ::SetClipboardData(CF_BITMAP, static_cast<HBITMAP>(vsrc));
            ::CloseClipboard();
            ret = true;
        }
        while (0);

        return ret;
    }

void AppScreen::freesysscreen(void *vsrc)
    {
        if (vsrc)
            DeleteObject(
                static_cast<HBITMAP>(vsrc)
            );
    }

#else

bool   AppScreen::setclipboard(void*) { return false; }
void * AppScreen::getsysscreen(SDL_Surface*) { return nullptr; }
void   AppScreen::freesysscreen(void*) {}

#endif
