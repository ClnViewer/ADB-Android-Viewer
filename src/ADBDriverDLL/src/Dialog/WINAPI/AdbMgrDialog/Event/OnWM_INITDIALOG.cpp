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

#include "../AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::_OnWM_INITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        ::SetDlgItemTextW(hWnd, DLG_ADBPATH, static_cast<LPCWSTR>(_adb->GetAdbBin()));

        if (::_waccess(_adb->GetAdbBin(), F_OK) < 0)
        {
            string_from_res<std::wstring> wres{};
            ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(
                                    wres.go(hmodule_get(), IDS_ERR3).c_str()
                                )
                            );
        }

  return 0;
}

}
