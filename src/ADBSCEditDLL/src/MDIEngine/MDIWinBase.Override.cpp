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

namespace MDIWin
{
    ///
    /// Base virtual method override (default version)
    ///

    WinData Base::data()
    {
        return WinData();
    }
    //
    std::tuple<bool, bool> Base::event_initBegin()
    {
        return { false, false };
    }
    //
    std::tuple<bool, bool> Base::event_initEnd()
    {
        return { false, false };
    }
    //
    bool Base::event_sizeinitBegin(ImageLite::IRECT<int32_t> const&)
    {
        return false;
    }
    bool Base::event_sizeinitEnd()
    {
        return true;
    }
    bool Base::event_size()
    {
        return false;
    }
    void Base::event_size(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&)
    {
    }
    void Base::event_resize()
    {
    }
    std::string Base::event()
    {
        return std::string();
    }
    void Base::event(void*) // EditBox (notify)
    {
    }
    void Base::event(int32_t, bool = false) // ToolBar
    {
    }
    void Base::event(int32_t, int32_t, int32_t, int32_t) // ToolBar (changebutton)
    {
    }
    bool Base::event(HWND, LPARAM, std::string & s) // ToolBox (drag & drop select source)
    {
        s = "";
        return false;
    }
    void Base::event(uint32_t, uint32_t, std::vector<MDIWin::BaseData::dataMap>&) // ToolBar set Plugins menu
    {
    }
    void Base::show()
    {
        if (!m_data.window)
            return;
        ::ShowWindow(m_data.window, SW_SHOWNORMAL);
    }
    void Base::show(uint32_t)
    {
    }
    void Base::show(std::vector<std::string> const&)
    {
    }

    ///
    /// Base virtual end
    ///

};

