
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_CTLCOLORSTATIC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) hWnd;
    (void) lParam;

    return _style.ColorStatic((HDC)wParam);
}

}
