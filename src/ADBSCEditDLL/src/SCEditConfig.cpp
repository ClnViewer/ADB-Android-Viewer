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

#include "SCEditInternal.h"

namespace Editor
{
    static inline const char *l_cnf_font = "Segoe UI";

    Config::Config()
    {
    }

    Config::~Config()
    {
        if (m_hfont)
            ::DeleteObject(m_hfont);
        m_hfont = nullptr;
    }


    Config& Config::instance()
    {
        static Config m_instance{};
        return m_instance;
    }

    void Config::init(HINSTANCE hinst_, HWND hwnd_)
    {
        if (hinst_)
            m_hinst = hinst_;
        if (hwnd_)
            m_hwnd = hwnd_;
    }

    void Config::init(HWND hwnd_)
    {
        if (hwnd_)
            m_hwnd = hwnd_;
    }

    void Config::clear()
    {
        m_hwnd = nullptr;
    }

    HWND Config::gethandle()
    {
        return m_hwnd;
    }

    std::string Config::getfontname()
    {
        return l_cnf_font;
    }

    HFONT Config::getfont()
    {
        if (!m_hfont)
            m_hfont = ::CreateFont(
                            20, 0, 0, 0,
                            FW_DONTCARE,
                            0, 0, 0,
                            ANSI_CHARSET,
                            OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY,
                            DEFAULT_PITCH | FF_SWISS,
                            l_cnf_font
                    );
        return m_hfont;
    }

};
