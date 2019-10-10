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

#include "plugin-template.h"
#include <chrono>
#include <thread>

namespace Plugins
{
    PluginTemplate::PluginTemplate(const char *s, const void *vcb)
    {
        m_adbcmd = static_cast<Plugins::PluginCallBack_s const*>(vcb);
        m_priority = 9;
        m_isready = true;
        m_name.assign(s);

        /// test part
        m_test_swap_click = false;
    }

    void PluginTemplate::go(std::vector<uint8_t> const & v, uint32_t w, uint32_t h)
    {
        m_isready = false;

        //...
        std::this_thread::sleep_for(std::chrono::seconds(10));

        GameDev::ADBDriver::Tap_s t{};

        if (!m_test_swap_click)
            t = { 14, 416 };
        else
            t = { 412, 416 };

        m_adbcmd->click(&t);
        m_test_swap_click = !(m_test_swap_click);

        //
#       if 0
        {
            std::stringstream ss;
            ss << m_name.c_str() << "_" << m_test_count << "_image.raw";
            FILE *fp = fopen(ss.str().c_str(), "wb");
            if (fp)
            {
                fwrite(&v[0], 1U, v.size(), fp);
                fclose(fp);
            }
        }
#       endif
        //
        m_isready = true;
    }
}



