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

namespace Editor
{
    const inline char l_string_default[] = "Used only in step debugger mode";
    //
    HWND ToolBox::setup_monitor_()
    {
        HWND hwnd = ::CreateWindowEx(
                        WS_EX_COMPOSITED | WS_EX_CLIENTEDGE,
                        WC_LISTBOXA, nullptr,
                        WS_CHILD          |
                        WS_CLIPCHILDREN   |
                        WS_VSCROLL        |
                        LBS_NOSEL         |
                        LBS_NOTIFY        |
                        LBS_USETABSTOPS,
                        0, 30, 1, 1,
                        m_hwnd,
                        (HMENU)(ID_TAB_IDX + static_cast<uint32_t>(TabIndex::ITAB_MONITOR)),
                        MDIWin::Config::instance().gethinstance(),
                        nullptr
                    );
        if (!hwnd)
            return nullptr;

        ::SendMessage(hwnd, WM_SETFONT, (WPARAM)MDIWin::Config::instance().getfont(), (LPARAM)1);
        ::SendMessage(hwnd, LB_ADDSTRING, (WPARAM)0, reinterpret_cast<LPARAM>(l_string_default));
        ::MakeDragList(hwnd);
        return hwnd;
    }

    void ToolBox::add_monitor_(HWND hwnd, std::vector<std::string> const & v)
    {
        int32_t idx = ::SendMessage(hwnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
        if (idx == LB_ERR)
            return;
        else if (idx > 0)
            ::SendMessage(hwnd, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

        if (!v.size())
        {
            ::SendMessage(hwnd, LB_ADDSTRING, (WPARAM)0, reinterpret_cast<LPARAM>(l_string_default));
            return;
        }

        for (auto & item : v)
            ::SendMessage(hwnd, LB_ADDSTRING, (WPARAM)0, (LPARAM) item.c_str());
        //
        ::SetWindowPos(
            hwnd,
            0,
            0, 0, 0, 0,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
        );
    }

    bool ToolBox::get_monitor_selected(HWND hwnd, int32_t idx, std::string & s)
    {
        do
        {
            if (idx == LB_ERR)
                if ((idx = ::SendMessage(hwnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0)) == LB_ERR)
                    break;

            int32_t len = ::SendMessage(hwnd, LB_GETTEXTLEN, (WPARAM)idx, (LPARAM)0);
            if (len == LB_ERR)
                break;

            std::vector<char> v(len + 1);
            ::SendMessage(hwnd, LB_GETTEXT, (WPARAM)idx, reinterpret_cast<LPARAM>(&v[0]));
            s = &v[0];

            if (s.empty())
                break;
            return true;
        }
        while (0);
        return false;
    }

    bool ToolBox::event_monitor(HWND hwnd, LPARAM lp_, std::string&)
    {
        do
        {
            if (!hwnd)
                break;

            DRAGLISTINFO *dli = reinterpret_cast<DRAGLISTINFO*>(lp_);
            if (!dli)
                break;
            if (dli->hWnd != hwnd)
                break;
            if (dli->uNotification != DL_BEGINDRAG)
                break;

            int32_t idx = ::LBItemFromPt(hwnd, dli->ptCursor, 1);
            if (idx < 0)
                break;

            std::string s;
            if (!get_monitor_selected(hwnd, idx, s))
                break;

            std::stringstream ss;
            ss << " -- " << s.c_str();
            GameDev::OLEDropSource::BeginDrag(ss.str());
            /// ALERTBox(ss.str());
            return true;
        }
        while (0);
        return false;
    }

};
