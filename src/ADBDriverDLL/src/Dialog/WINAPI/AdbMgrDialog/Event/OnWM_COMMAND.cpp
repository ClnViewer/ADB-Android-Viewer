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

static inline const LPCWSTR _openFilter = L"Android Debug Bridge (adb.exe)\0adb.exe\0";
static inline const LPCWSTR _openExt = L"exe";
static inline const LPCWSTR _openDir = L".\\";

INT_PTR AdbMgrDialog::_OnWM_COMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;

        switch (LOWORD(wParam))
        {
        case ID_ADBSTART:
        {
            _tfut = std::async(std::launch::async,
                    [=]()
                    {
                        return _adb->AdbStart();
                    }
                );
            ::PostMessageA(hWnd, WM_COMMAND, (WPARAM)IDR_CMDEVENT, (LPARAM)ID_ADBSTART);
            return 1;
        }
        case ID_ADBSTOP:
        {
            _tfut = std::async(std::launch::async,
                    [=]()
                    {
                        return _adb->AdbStop();
                    }
                );
            ::PostMessageA(hWnd, WM_COMMAND, (WPARAM)IDR_CMDEVENT, (LPARAM)ID_ADBSTOP);
            return 1;
        }
        case ID_ADBPATH:
        {
            OPENFILENAMEW sfn{};
            std::wstring ws{};
            ws.resize(512);

            sfn.lStructSize = sizeof(OPENFILENAMEW);
            sfn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR |
                        OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            sfn.lpstrFilter = _openFilter;
            sfn.lpstrDefExt = _openExt;
            sfn.lpstrInitialDir = _openDir;
            sfn.hwndOwner = hWnd;
            sfn.lpstrFile = static_cast<LPWSTR>(ws.data());
            sfn.nMaxFile = ws.size();

            if (::GetOpenFileNameW(&sfn))
            {
                if (ws.length())
                {
                    _adb->SetAdbBin(ws);
                    ::SetDlgItemTextW(hWnd, DLG_ADBPATH, static_cast<LPCWSTR>(ws.c_str()));
                    ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(L""));
                }
            }
            return 1;
        }
        case IDR_CMDEVENT:
        {
                try
                {
                    std::wstring ws;
                    std::future_status st = _tfut.wait_for(std::chrono::milliseconds(10));

                    switch (st)
                    {
                        case std::future_status::ready:
                            {
                                ws = _tfut.get();
                                break;
                            }
                        case std::future_status::timeout:
                            {
                                ::PostMessageA(hWnd, WM_COMMAND, (WPARAM)IDR_CMDEVENT, (LPARAM)LOWORD(lParam));
                                return 1;
                            }
                        case std::future_status::deferred:
                            {
                                string_from_res<std::wstring> wres{};
                                throw DriverExcept(wres.go(hmodule_get(), IDS_ERR1));
                            }
                        default:
                            {
                                string_from_res<std::wstring> wres{};
                                throw DriverExcept(wres.go(hmodule_get(), IDS_ERR2));
                            }
                    }

                    if (ws.length())
                        ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(ws.c_str()));

                    (void) ::InvalidateRgn(hWnd, NULL, TRUE);

                    switch (LOWORD(lParam))
                    {
                        case ID_ADBSTART: { if (ws.length()) _IsAdbStatus = true; break;  }
                        case ID_ADBSTOP:  { _IsAdbStatus = false; break; }
                    }
                    return 1;
                }
                catch (const std::future_error & _ex)
                {
                    std::wstringstream wss;

                    if (_ex.code().value())
                        wss << _ex.code().value() << L" : ";
                    if (_ex.what())
                        wss << _ex.what();

                    ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(wss.str().c_str()));
                    _IsAdbStatus = false;
                    return 1;
                }
                catch (const GameDev::DriverExcept & _ex)
                {
                    ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(_ex.Message()));
                    _IsAdbStatus = false;
                    return 1;
                }
                catch (const std::exception & _ex)
                {
                    if (_ex.what())
                    {
                        std::wstringstream wss;
                        wss << _ex.what();
                        ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(wss.str().c_str()));
                    }
                    _IsAdbStatus = false;
                    return 1;
                }
                catch (...)
                {
                    string_from_res<std::wstring> wres{};
                    ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(
                                        wres.go(hmodule_get(), IDS_ERRA).c_str()
                                        )
                                    );
                    _IsAdbStatus = false;
                    return 1;
                }
                break;
        }
        case IDCANCEL:
        {
            ::PostMessageA(hWnd, WM_CLOSE, wParam, lParam);
            return 1;
        }
        default:
            break;
        }
  return 0;
}

}
