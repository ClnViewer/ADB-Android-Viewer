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

INT_PTR AdbMgrDialog::_OnWM_DRAWITEM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) hWnd;
    (void) uMsg;
    (void) wParam;

            const DRAWITEMSTRUCT *di = (DRAWITEMSTRUCT*)lParam;

            if ((!di) || (di->CtlType != ODT_BUTTON))
                return 0;

            SIZE sz;
            std::wstring wtxt{}; wtxt.resize(100);
            int tsz = ::GetWindowTextW(di->hwndItem, wtxt.data(), wtxt.size());
            wtxt.resize(tsz);
            ::GetTextExtentPoint32W(di->hDC, wtxt.c_str(), wtxt.length(), &sz);

            if (di->itemState & ODS_SELECTED)
            {
                (void) ::SetTextColor(
                            di->hDC,
                            ((m_style.IsDefaultStyle()) ?
                                m_style.RGBColorTxtButtonDefault() : m_style.RGBColorBg()
                             )
                        );
                (void) ::SetBkColor(
                            di->hDC,
                            ((m_style.IsDefaultStyle()) ?
                                m_style.RGBColorBgButtonLightDefault() : m_style.RGBColorTxt()
                             )
                        );
            }
            else
            {
                (void) ::SetTextColor(
                            di->hDC,
                            ((m_style.IsDefaultStyle()) ?
                                m_style.RGBColorTxtButtonDefault() : m_style.RGBColorTxt()
                             )
                        );
                (void) ::SetBkColor(
                            di->hDC,
                            ((m_style.IsDefaultStyle()) ?
                                m_style.RGBColorBgButtonDefault() : m_style.RGBColorBg()
                             )
                        );
            }

            ::ExtTextOutW(
                di->hDC,
                (((di->rcItem.right - di->rcItem.left) - sz.cx) / 2),
                (((di->rcItem.bottom - di->rcItem.top) - sz.cy) / 2),
                ETO_OPAQUE | ETO_CLIPPED,
                &di->rcItem,
                wtxt.c_str(),
                wtxt.length(),
                NULL
            );
            ::DrawEdge(
                di->hDC,
                const_cast<LPRECT>(&di->rcItem),
                (di->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED),
                BF_RECT
            );

            /// Focus border set
            /*
            if (di->itemState & ODS_SELECTED)
                DrawFocusRect(di->hDC, &di->rcItem);
            */

  return 1;
}

}
