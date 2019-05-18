
#include "AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::__DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CHAR:         return _OnWM_CHAR(hWnd, uMsg, wParam, lParam);
    case WM_COMMAND:      return _OnWM_COMMAND(hWnd, uMsg, wParam, lParam);
    case WM_NOTIFY:       return _OnWM_NOTIFY(hWnd, uMsg, wParam, lParam);
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

INT_PTR CALLBACK AdbListDialog::DialogRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG)
        SetWindowLongPtr(hWnd, DWLP_USER, lParam);

    AdbListDialog *pThis =
        reinterpret_cast<AdbListDialog*>(GetWindowLongPtr(hWnd, DWLP_USER));

    if (pThis)
        return pThis->__DialogProc(hWnd, uMsg, wParam, lParam);
    return FALSE;
}

INT_PTR AdbListDialog::Show(SelectedList & dict)
{
    return Show(dict, 0);
}

INT_PTR AdbListDialog::Show(SelectedList & dict, int32_t colw, bool isreverse)
{
    _isreverse = isreverse;
    return Show(dict, colw);
}

INT_PTR AdbListDialog::Show(SelectedList & dict, int32_t colw)
{
    if (!_hinst)
        _hinst = hmodule_get();

    _dict = dict;
    _p.x = ((colw) ? colw : 100);
    _p.y = ((colw) ? (470 - colw) : 370);

    return DialogBoxParamW(
               _hinst,
               MAKEINTRESOURCEW(_rid),
               NULL,
               &AdbListDialog::DialogRedirect,
               reinterpret_cast<LPARAM>(this)
           );
}

std::wstring AdbListDialog::SelectedKey() const
{
    return _key;
}

std::wstring AdbListDialog::SelectedValue() const
{
    return _val;
}

AdbListDialog::AdbListDialog(DWORD rid)
    : _p({0,0}), _hwnd(nullptr), _hinst(nullptr), _rid(rid), _sel(-1), _isreverse(false) {}

AdbListDialog::AdbListDialog(HINSTANCE hinst, DWORD rid)
    : _p({0,0}), _hwnd(nullptr), _hinst(hinst), _rid(rid), _sel(-1), _isreverse(false) {}

AdbListDialog::AdbListDialog(HINSTANCE hinst, DWORD rid, COLORREF creft, COLORREF crefb)
    : _p({0,0}), _hwnd(nullptr), _hinst(hinst), _rid(rid), _sel(-1), _isreverse(false)
    {
        _style.Set(creft, crefb);
    }

AdbListDialog::~AdbListDialog()
{
    _style.~StyleDialog();
}

}


