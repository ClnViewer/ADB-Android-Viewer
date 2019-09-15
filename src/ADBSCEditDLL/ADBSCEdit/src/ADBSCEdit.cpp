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

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define IDI_ICON 1001
#include "../../src/SCEditDll.h"
#if defined(_DEBUG)
#  include <exchndl.h>
#endif

static void msgbox(std::string const & s)
    {
        ::MessageBoxA(
                nullptr,
                static_cast<LPCSTR>(s.c_str()),
                "SCEdit", MB_OK | MB_ICONERROR
            );
    }

static std::string parseargs(LPSTR str)
    {
        try
        {
            std::string s(str);

            do
            {
                if (s.empty())
                    break;

                std::size_t k = 0, m = 0, n = 0, i = 0;

                for (; i < s.length(); i++)
                    if      ((i)   && (s.at(i) == '"'))             { k = i; break; }
                    else if ((i)   && (!m) && (s.at(i) != ' '))     { m = i; }
                    else if (((!n) || (m > n)) && (s.at(i) == ' ')) { n = i; }

                if (s.at(0) == '"')
                {
                    if (k)
                        --k;
                    else if (n)
                        k = (n - m);
                    else
                        k = (s.length() - 1);
                }
                else if (n)
                    m = 0, k = n;
                else
                    break;

                s = s.substr(m, k);
            }
            while (0);
            return s;
        }
        catch (std::exception const & ex_)
        {
            msgbox(ex_.what());
            return std::string();

        }
        catch (...)
        {
            msgbox("parse arguments fatal error");
            return std::string();
        }
    }

int32_t WINAPI WinMain (HINSTANCE hinst, HINSTANCE, LPSTR args, int32_t)
    {

#       if defined(_DEBUG)
        static const char *l_dlog = "SCEdit.rpt";
        ExcHndlInit();
        ExcHndlSetLogFileNameA(l_dlog);
#       endif

        try
        {
            int32_t ret;
            HICON hicon = ::LoadIconA(hinst, MAKEINTRESOURCE(IDI_ICON));
            std::string p = parseargs(args);
            if (!p.empty())
                ret = RunEditOpen(p, true, hicon);
            else
                ret = RunEditEmpty(true, hicon);

            return ((ret == 1) ?
                    0 : ((!ret) ? 128 : ret)
                );
        }
        catch (std::exception const & ex_)
        {
            msgbox(ex_.what());
            return 127;
        }
        catch (...)
        {
            msgbox("main fatal error");
            return 127;
        }
    }
