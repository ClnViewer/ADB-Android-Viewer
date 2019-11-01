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

#include "SCEditInternal.h"
#include <windowsx.h>

#define break_error() { ERRORBox(); break; }

namespace Editor
{
    LRESULT SCEApp::mdiFrameProc(HWND hwnd, HWND fhwnd, UINT msg, WPARAM wParam, LPARAM lParam, bool b)
    {
        try
        {
            switch(msg)
            {
                HANDLE_MSG (hwnd, WM_NCHITTEST, _OnNCHitTest);
                case WM_COMMAND:
                    {
                        _OnCommand(hwnd, msg, wParam, lParam);
                        return 0;
                    }
                default:
                    break;
            }
            if (b)
                return 0;
            else
                return ::DefFrameProc(hwnd, fhwnd, msg, wParam, lParam);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return 0;
    }
};

