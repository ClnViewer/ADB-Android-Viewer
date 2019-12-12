/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

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

#include "SCEditInternal.h"
#include <commctrl.h>
// #include <shlwapi.h>

namespace Editor
{
    ///
    static inline const wchar_t l_search_banner[] = L"Search text";

    LRESULT CALLBACK SearchBar::SearchWndProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, UINT_PTR, DWORD_PTR ptr)
    {
        switch (umsg_)
        {
            case WM_KEYUP:
                {
                    HWND hwnd = reinterpret_cast<HWND>(ptr);
                    if (hwnd)
                        (void) ::SendMessage(hwnd, WM_COMMAND, (WPARAM)IDM_EVENT_EDIT_FINDTEXT, (LPARAM)0);
                    return 0;
                }
        }
        return ::DefSubclassProc(hwnd_, umsg_, wp_, lp_);
    }

    ///

    SearchBar::~SearchBar()
    {
        if (WinBase::m_hwnd)
        {
            GameDev::OLEDropTarget::UnregisterDropWindow(WinBase::m_hwnd, m_dropt);
            ::RemoveWindowSubclass(WinBase::m_hwnd, &SearchBar::SearchWndProc, 0);
        }
    }

    bool SearchBar::init(HWND hwnd_, HWND phwnd_)
    {
        do
        {
            if (!WinBase::create(
                    hwnd_,
                    "EDIT",
                    WS_VISIBLE          |
                    WS_CHILD            |
                    WS_CLIPCHILDREN     |
                    WS_CLIPSIBLINGS     |
                    WS_BORDER           |
                    ES_LEFT             |
                    ES_AUTOVSCROLL,
                    (HMENU)ID_REBAR_SEARCH
                ))
                break;

            ::SendMessage(WinBase::m_hwnd, EM_SETMARGINS, EC_LEFTMARGIN,  (LPARAM)5);
            ::SendMessage(WinBase::m_hwnd, EM_SETMARGINS, EC_RIGHTMARGIN, (LPARAM)10);
            ::SendMessage(WinBase::m_hwnd, EM_SETCUEBANNER, 0, reinterpret_cast<LPARAM>(l_search_banner));
            //
            ::SetWindowSubclass(
                    WinBase::m_hwnd,
                    &SearchBar::SearchWndProc,
                    0,
                    reinterpret_cast<DWORD_PTR>(phwnd_)
                );
            // ::SHAutoComplete(WinBase::m_hwnd, 0x00000040 | 0x00000008 | 0x40000000);
            GameDev::OLEDropTarget::RegisterDropWindow(WinBase::m_hwnd, &m_dropt);
            return true;
        }
        while (0);
        return false;
    }

    std::string SearchBar::gettext()
    {
        do
        {
            if (!WinBase::m_hwnd)
                break;

            int32_t sz = ::GetWindowTextLength(WinBase::m_hwnd);
            if (!sz)
                break;

            auto dbuf = std::make_unique<char[]>(++sz);
            if (!::GetWindowText(WinBase::m_hwnd, dbuf.get(), sz))
                break;

            return std::string(dbuf.get());
        }
        while (0);
        return std::string();
    }
};
