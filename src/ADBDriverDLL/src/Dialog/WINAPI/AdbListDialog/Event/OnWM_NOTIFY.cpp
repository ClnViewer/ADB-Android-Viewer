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

#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_NOTIFY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) hWnd;
    (void) uMsg;
    (void) wParam;

    switch (((LPNMHDR)lParam)->code)
    {
    case NM_DBLCLK:
        {
            if (_sel > -1)
            {
                ::EndDialog(hWnd, _sel);
                break;
            }
            return 0;
        }
    case LVN_ITEMCHANGED:
        {

        NMLISTVIEW *nlv = reinterpret_cast<NMLISTVIEW*>(lParam);
        if (nlv)
            if (
                (nlv->hdr.code == LVN_ITEMCHANGED) &&
                (nlv->hdr.idFrom == DLG_LISTV) &&
                (nlv->uNewState & LVIS_SELECTED) &&
                ((_sel = ListView_GetNextItem(_hwnd, -1, LVNI_ALL | LVNI_SELECTED)) >= 0)
            )
            {
                LVITEMW lvi{};
                _key.clear(); _key.resize(100);
                _val.clear(); _val.resize(100);

                lvi.iSubItem = 0;
                lvi.cchTextMax = 100;
                lvi.pszText = (LPWSTR) _key.data();
                SendMessageW(_hwnd, LVM_GETITEMTEXTW, (WPARAM)_sel, (LPARAM) (LVITEMW*)&lvi);

                lvi.iSubItem = 1;
                lvi.cchTextMax = 100;
                lvi.pszText = (LPWSTR) _val.data();
                SendMessageW(_hwnd, LVM_GETITEMTEXTW, (WPARAM)_sel, (LPARAM) (LVITEMW*)&lvi);

                SetDlgItemTextW(hWnd, DLG_LISTI, static_cast<LPCWSTR>(_val.c_str()));
            }

        break;
        }
        default:
            break;
    }

  return 1;
}

}
