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
    MainWin::MainWin()
    {
        Base::m_data = data();
    }

    MDIWin::WinData MainWin::data()
    {
        MDIWin::WinData d(
                    MDIWin::BaseData::MDIWinType::MWTYPE_MAIN,   // MDIWinType group
                    MDIWin::BaseData::MDIWinType::MWTYPE_MAIN,   // MDIWinType type
                    MDIWin::BaseData::MDIWinStyle::MWSTYLE_SYS_CAPTION, // MDIWinStyle
                    std::string(),                               // Class name
                    MDIWin::Config::instance().appname,          // Title
                    (WS_OVERLAPPED | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_BORDER),
                    (WS_EX_COMPOSITED | WS_EX_WINDOWEDGE)
               );
        d.irdefault.set<int32_t>(5, 5, 90, 90);                  // % from desktop screen
        return d;
    }

    std::tuple<bool, bool> MainWin::event_initBegin()
    {
        try
        {
            do
            {
                Base::m_data.irsize = {};
                ImageLite::IRECT<int32_t> irm{};

                irm.set<int32_t>(
                    0, 0,
                    ::GetSystemMetrics(SM_CXSCREEN),
                    ::GetSystemMetrics(SM_CYSCREEN)
                );
                Base::setsizedefault(irm);
                RECT rc = Base::m_data.irsize.get<RECT, int32_t>();
                if (!::AdjustWindowRectEx(&rc, Base::m_data.wflags, TRUE, Base::m_data.wflagex))
                    break;
                Base::m_data.irsize.convert(rc);

                if (!Base::create(Base::m_data.irsize))
                    break;
                return { true, true };
            }
            while (0);
        }
        catch (...) {}
        return { true, false };
    }

};
