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
#include <ctime>
#include <richedit.h>

namespace MDIWin
{
    static const COLORREF l_color[] =
    {
        RGB(210, 0, 0),
        RGB(130, 18, 0),
        RGB(0, 50, 90),
        RGB(40, 40, 40)
    };

    static void f_time_print(std::stringstream & ss)
    {
        auto tt = std::time(nullptr);
        auto ttm = *std::localtime(&tt);
        ss << std::put_time(&ttm, "%H:%M:%S");
    }

    static void f_highlight(HWND hwnd, COLORREF const & color)
    {
        CHARFORMAT cf{};
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_COLOR;
        cf.dwEffects = 0;
        cf.crTextColor = color;
        ::SendMessage(hwnd, EM_SETSEL, -2, -1);
        ::SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    void f_settext(HWND hwnd, std::string const & s, bool isclear)
    {
        std::vector<char> v(s.length() + 3);
        v.assign(s.begin(), s.end());
        v[s.length()] = '\r';
        v[s.length() + 1] = '\n';
        v[s.length() + 2] = '\0';

        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ((isclear) ? ST_DEFAULT : ST_SELECTION);
        ::SendMessage(hwnd, EM_SETTEXTEX, (WPARAM)&se,      (LPARAM)&v[0]);
        ::SendMessage(hwnd, WM_VSCROLL,   (WPARAM)SB_BOTTOM, (LPARAM)0);
    }

    void f_cleartext(HWND hwnd)
    {
        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ST_DEFAULT;
        ::SendMessage(
                hwnd,
                EM_SETTEXTEX,
                (WPARAM)&se,
                (LPARAM)&""
        );
    }

    ///

    void Config::text()
    {
        f_cleartext(gethandle<HandleType::HWND_INFOBOX>());
    }

    void Config::text(std::string const & s)
    {
        f_highlight(gethandle<HandleType::HWND_INFOBOX>(), l_color[TextMsgType::IB_TYPE_DEFAULT]);
        f_settext(gethandle<HandleType::HWND_INFOBOX>(), s, false);
    }

    void Config::text(std::string const & s, COLORREF const & color)
    {
        f_highlight(gethandle<HandleType::HWND_INFOBOX>(), color);
        f_settext(gethandle<HandleType::HWND_INFOBOX>(), s, false);
    }

    void Config::text(std::string const & s, TextMsgType t)
    {
        f_highlight(gethandle<HandleType::HWND_INFOBOX>(), l_color[t]);
        f_settext(gethandle<HandleType::HWND_INFOBOX>(), s, false);
    }

    ///

    void Config::debugBox(
            std::string const & msg,
            COLORREF const & color,
            std::string const & func,
            int32_t line)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        if (!func.empty())
        {
            ss << func.c_str();
            if (line != -1)
                ss << ":" << line;
            ss << " - ";
        }
        ss << msg.c_str();
        BoxT<COLORREF>(ss.str(), color);
    }

    void Config::errorBox(std::string const & func, int32_t line, uint32_t err)
    {
        if (!err)
            err = ::GetLastError();

        if (err)
        {
            auto dbuf = std::make_unique<char[]>(1024);
            DWORD sz = ::FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                0, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                dbuf.get(), 1024, 0
            );
            std::stringstream ss;
            f_time_print(ss);
            ss << " ";
            ss << func.c_str() << ":" << line;
            if (sz)
                ss << " - " << dbuf.get();
            BoxT<TextMsgType>(ss.str(), TextMsgType::IB_TYPE_ERROR);
        }
    }

    void Config::alertBox(std::string const & msg, std::string const & func, int32_t line)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        ss << func.c_str() << ":" << line << " - " << msg.c_str();
        BoxT<TextMsgType>(ss.str(), TextMsgType::IB_TYPE_ALERT);
    }

    void Config::infoBox(std::string const & msg)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        ss << msg.c_str();
        BoxT<TextMsgType>(msg, TextMsgType::IB_TYPE_INFO);
    }

    void Config::defaultBox(std::string const & msg)
    {
        BoxT<TextMsgType>(msg, TextMsgType::IB_TYPE_DEFAULT);
    }

    void Config::colorBox(std::string const & msg, COLORREF const & color)
    {
        BoxT<COLORREF>(msg, color);
    }

    void Config::clearBox()
    {
        BoxT<TextMsgType>(std::string(), TextMsgType::IB_TYPE_DEFAULT);
    }

    ///

    template <typename T>
    void Config::BoxT(std::string const & msg, T const & val)
    {
        if (gethandle<HandleType::HWND_INFOBOX>())
        {
            if (msg.empty())
                text();
            else
                text(msg, val);
        }
        else
            ::MessageBoxA(
                    gethandle<HandleType::HWND_MAIN>(),
                    static_cast<LPCSTR>(msg.c_str()),
                    static_cast<LPCSTR>(appname.c_str()),
                    MB_OK | MB_ICONERROR
                );
    }
    template void Config::BoxT<Config::TextMsgType>(std::string const&, Config::TextMsgType const&);
    template void Config::BoxT<COLORREF>(std::string const&, COLORREF const&);

};
