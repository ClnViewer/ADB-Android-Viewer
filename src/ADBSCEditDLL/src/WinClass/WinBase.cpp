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

#include "../SCEditInternal.h"

namespace Editor
{

    /// end static

    WinBase::WinBase() {}
    WinBase::~WinBase()
    {
        if (m_hwnd)
            ::DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    HWND WinBase::gethandle()
    {
        return m_hwnd;
    }

    bool WinBase::create(
            HWND parent,
            std::string const & s,
            uint32_t flags,
            HMENU hid,
            void * obj
        )
    {
        do
        {
            m_hwnd = ::CreateWindowExA(
                    WS_EX_COMPOSITED,
                    static_cast<LPCSTR>(s.c_str()), "",
                    flags,
                    0, 0, 1, 1,
                    parent,
                    hid,
                    MDIWin::Config::instance().gethinstance(),
                    obj
                );
            if (!m_hwnd)
                break;

            HFONT hfont = MDIWin::Config::instance().getfont();
            if (hfont)
                ::SendMessage(m_hwnd, WM_SETFONT, (LPARAM)hfont, 1);

            styleclear(m_hwnd);
            return true;
        }
        while (0);
        return false;
    }

    void WinBase::show()
    {
        if (m_hwnd)
            ::ShowWindow(m_hwnd, SW_SHOWNORMAL);
    }

    void WinBase::styleclear(HWND hwnd)
    {
        MDIWin::BaseData::setwinstyle(hwnd, MDIWin::BaseData::MDIWinStyle::MWSTYLE_SYS_CLEAR);
    }
};

