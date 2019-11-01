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
    MDIWin::Base * Manager::find(uint32_t id)
    {
        try
        {
            auto obj = m_winmap.find(id);
            if (obj != m_winmap.end())
                return obj->second.get();
            return nullptr;
        }
        catch (...) { return nullptr; }
    }

    MDIWin::Base * Manager::find(HWND hwnd)
    {
        try
        {
            for (auto const & [key, val] : m_winmap)
            {
                auto a = val.get();
                if ((a) && (a->gethandle() == hwnd))
                    return a;
            }
            return nullptr;
        }
        catch (...) { return nullptr; }
    }

    bool Manager::remove(uint32_t id, bool b)
    {
        try
        {
            auto obj = m_winmap.find(id);
            if (obj == m_winmap.end())
                return false;

            auto a = obj->second.get();
            if (a)
                a->close(b);
            m_winmap.erase(obj);
            return true;
        }
        catch (...) { return false; }
    }

    bool Manager::remove(HWND hwnd)
    {
        uint32_t id = getwinid(hwnd);
        if (id)
            return remove(id, false);
        return false;
    }
};

