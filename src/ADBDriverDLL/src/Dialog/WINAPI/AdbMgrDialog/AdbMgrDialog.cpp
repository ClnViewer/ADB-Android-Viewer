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

#include "AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::__DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CHAR:         return _OnWM_CHAR(hWnd, uMsg, wParam, lParam);
    case WM_COMMAND:      return _OnWM_COMMAND(hWnd, uMsg, wParam, lParam);
    case WM_CLOSE:        return _OnWM_CLOSE(hWnd, uMsg, wParam, lParam);
    case WM_INITDIALOG:   return _OnWM_INITDIALOG(hWnd, uMsg, wParam, lParam);
    //case WM_CTLCOLORBTN:
    case WM_CTLCOLORSCROLLBAR:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORDLG:  return _OnWM_CTLCOLORSTATIC(hWnd, uMsg, wParam, lParam);
    case WM_DRAWITEM:     return _OnWM_DRAWITEM(hWnd, uMsg, wParam, lParam);
    case WM_NCHITTEST:    return _OnWM_NCHITTEST(hWnd, uMsg, wParam, lParam);
    default:
        break;
    }
    return false;
}

INT_PTR CALLBACK AdbMgrDialog::DialogRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG)
        SetWindowLongPtr(hWnd, DWLP_USER, lParam);

    AdbMgrDialog *pThis =
        reinterpret_cast<AdbMgrDialog*>(GetWindowLongPtr(hWnd, DWLP_USER));

    if (pThis)
        return pThis->__DialogProc(hWnd, uMsg, wParam, lParam);
    return FALSE;
}

INT_PTR AdbMgrDialog::Show()
{
    ADBDriver adb;
    INT_PTR ret = Show(adb);
    adb.~ADBDriver();
    return ret;
}

INT_PTR AdbMgrDialog::Show(GameDev::ADBDriver *padb)
{
    if (!padb)
        return 0;
    return Show(*padb);
}

INT_PTR AdbMgrDialog::Show(GameDev::ADBDriver & adb)
{
    m_adb = &adb;

    if (!m_hinst)
        m_hinst = GameDev::gethmodule();

    return DialogBoxParamW(
               m_hinst,
               MAKEINTRESOURCEW(m_rid),
               NULL,
               &AdbMgrDialog::DialogRedirect,
               reinterpret_cast<LPARAM>(this)
           );
}

AdbMgrDialog::AdbMgrDialog(DWORD rid)
    : m_hwnd(nullptr), m_hinst(nullptr), m_rid(rid),
      m_IsFuture(false), m_IsAdbStatus(false), m_adb(nullptr) {}

AdbMgrDialog::AdbMgrDialog(HINSTANCE hinst, DWORD rid)
    : m_hwnd(nullptr), m_hinst(hinst), m_rid(rid),
      m_IsFuture(false), m_IsAdbStatus(false), m_adb(nullptr) {}

AdbMgrDialog::AdbMgrDialog(HINSTANCE hinst, DWORD rid, COLORREF creft, COLORREF crefb)
    : m_hwnd(nullptr), m_hinst(hinst), m_rid(rid),
      m_IsFuture(false), m_IsAdbStatus(false), m_adb(nullptr)
    {
        m_style.Set(creft, crefb);
    }

AdbMgrDialog::~AdbMgrDialog()
{
    m_style.~StyleDialog();
}

}


