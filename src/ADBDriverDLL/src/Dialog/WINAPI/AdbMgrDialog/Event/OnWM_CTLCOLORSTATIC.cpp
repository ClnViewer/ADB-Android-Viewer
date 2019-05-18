
#include "../AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::_OnWM_CTLCOLORSTATIC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) hWnd;
    (void) uMsg;

    switch(::GetDlgCtrlID((HWND)lParam))
    {
    case DLG_ADBSTAT:
        {
            if (_adb->AdbCheck())
                return _style.ColorStatic((HDC)wParam, RGB(10,250,10));
            return _style.ColorStatic((HDC)wParam, RGB(250,10,10));
        }
    default:
        {
            return _style.ColorStatic((HDC)wParam);
        }
    }
}

}
