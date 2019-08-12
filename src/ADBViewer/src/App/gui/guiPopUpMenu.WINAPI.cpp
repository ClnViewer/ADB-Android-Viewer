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
#include  "guiPopUpMenu.WINAPI.h"
HBRUSH guiPopUp::m_hbrush = nullptr;void guiPopUp::clean()    {        if (m_hbrush)            ::DeleteObject(m_hbrush);        m_hbrush = nullptr;    }int32_t guiPopUp::show(HMENU hmenu, SDL_Window *swin)    {        int32_t idx = -1;        do        {            if (!hmenu)                break;            int32_t x, y;            HWND l_hWnd = nullptr;            if (!(l_hWnd = AppSysDialog::gethwnd(swin)))                break;            ::SDL_GetGlobalMouseState(&x, &y);            idx = static_cast<int32_t>(::TrackPopupMenu(
                        hmenu,
                        TPM_RETURNCMD | TPM_NONOTIFY |
                        TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,
                        x, y,
                        0,
                        l_hWnd,
                        nullptr
                    ));
        }        while (0);        guiPopUp::clean();        return idx;    }void guiPopUp::style(HMENU hmenu)    {        if (!hmenu)            return;        m_hbrush = ((m_hbrush) ?                m_hbrush :                ::CreateSolidBrush(RGB(151,192,36))        );        if (!m_hbrush)            return;        MENUINFO mim{};        mim.cbSize = sizeof(mim);        mim.hbrBack = (HBRUSH) m_hbrush;        mim.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE | MIM_BACKGROUND;        mim.dwStyle = MNS_AUTODISMISS | MNS_NOCHECK;        ::SetMenuInfo(hmenu, &mim);    }