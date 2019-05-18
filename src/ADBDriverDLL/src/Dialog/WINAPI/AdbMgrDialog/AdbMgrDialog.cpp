
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
    _adb = &adb;

    if (!_hinst)
        _hinst = hmodule_get();

    return DialogBoxParamW(
               _hinst,
               MAKEINTRESOURCEW(_rid),
               NULL,
               &AdbMgrDialog::DialogRedirect,
               reinterpret_cast<LPARAM>(this)
           );
}

AdbMgrDialog::AdbMgrDialog(DWORD rid)
    : _hwnd(nullptr), _hinst(nullptr), _rid(rid),
      _IsFuture(false), _IsAdbStatus(false), _adb(nullptr) {}

AdbMgrDialog::AdbMgrDialog(HINSTANCE hinst, DWORD rid)
    : _hwnd(nullptr), _hinst(hinst), _rid(rid),
      _IsFuture(false), _IsAdbStatus(false), _adb(nullptr) {}

AdbMgrDialog::AdbMgrDialog(HINSTANCE hinst, DWORD rid, COLORREF creft, COLORREF crefb)
    : _hwnd(nullptr), _hinst(hinst), _rid(rid),
      _IsFuture(false), _IsAdbStatus(false), _adb(nullptr)
    {
        _style.Set(creft, crefb);
    }

AdbMgrDialog::~AdbMgrDialog()
{
    _style.~StyleDialog();
}

}


