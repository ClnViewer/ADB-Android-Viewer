
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_CTLCOLORDLG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) hWnd;
    (void) wParam;
    (void) lParam;

    return _style.ColorDialog();
}

}
