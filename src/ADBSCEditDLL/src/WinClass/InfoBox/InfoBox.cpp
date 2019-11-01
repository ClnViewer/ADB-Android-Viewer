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

#include "../../SCEditInternal.h"
#include <richedit.h>

namespace Editor
{
    InfoBox::InfoBox()
    {
        Base::m_data = data();
        m_libed.init("msftedit.dll");
    }

    InfoBox::~InfoBox()
    {
        MDIWin::Config::instance().sethandle<MDIWin::Config::HandleType::HWND_INFOBOX>(nullptr);
        if (m_hwnd)
            ::DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    MDIWin::WinData InfoBox::data()
    {
        MDIWin::WinData d(
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD,      // MDIWinType group
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_INFO, // MDIWinType type
                    MDIWin::BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE,  // MDIWinStyle
                    std::string(),                                   // Class name
                    "Debug Output:",                                 // Title
                    (WS_POPUP | WS_SYSMENU),
                    WS_EX_COMPOSITED
                );
        d.irdefault.set<int32_t>(0, 70, 100, 30);                    // % from main widow
        return d;
    }

    void InfoBox::sizeinit_()
    {
        do
        {
            if (!m_hwnd)
                break;

            ImageLite::IRECT<int32_t> ir = m_titlebar.getsize();
            ir.x = 0;
            ir.y = ir.h;
            ir.w = Base::m_data.irsize.w;
            ir.h = (Base::m_data.irsize.h - ir.h);

            (void) ::SetWindowPos(
                    m_hwnd,
                    0,
                    ir.x,
                    ir.y,
                    ir.w,
                    ir.h,
                    SWP_NOACTIVATE | SWP_SHOWWINDOW
            );
        }
        while (0);
    }

    void InfoBox::event_resize()
    {
        sizeinit_();
    }

    void InfoBox::event_size(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const & ire)
    {
        Base::m_data.irsize.set<int32_t>(
                ire.x,
                (irc.y + ire.h),
                irc.w,
                (irc.h - ire.h)
            );
        if (Base::m_data.irsize.empty())
            return;

        Base::m_titlebar.setsize(Base::m_data.irsize);
        Base::resize();
        sizeinit_();
    }

    std::tuple<bool, bool> InfoBox::event_initEnd()
    {
        do
        {
            if (!Base::m_data.window)
                break;

            m_hwnd = ::CreateWindowExW(
                            WS_EX_COMPOSITED,
                            MSFTEDIT_CLASS, L"",
                            WS_CHILD | WS_VSCROLL | WS_CLIPCHILDREN |
                            ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                            0, Base::m_titlebar.getsize().h, 1, 1,
                            Base::m_data.window,
                            (HMENU)ID_INFO_RTBOX,
                            MDIWin::Config::instance().gethinstance(),
                            nullptr
                    );
            if (!m_hwnd)
                break;

            ::SendMessage(m_hwnd, EM_FMTLINES,      (WPARAM)1, (LPARAM)0);
            ::SendMessage(m_hwnd, EM_AUTOURLDETECT, (WPARAM)1, (LPARAM)0);
            ::SendMessage(m_hwnd, EM_SETMARGINS,    (WPARAM)EC_LEFTMARGIN, (LPARAM)12);
            ::ShowWindow (m_hwnd, SW_SHOW);
            //
            MDIWin::Config::instance().sethandle<MDIWin::Config::HandleType::HWND_INFOBOX>(m_hwnd);
            //
            return { true, true };
        }
        while (0);
        return { true, false };
    }
};
