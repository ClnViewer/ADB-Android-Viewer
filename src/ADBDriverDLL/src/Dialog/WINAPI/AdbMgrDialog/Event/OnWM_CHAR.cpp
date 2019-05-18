
#include "../AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::_OnWM_CHAR(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        // TODO (clanc#1#): ?? re.. CONTROL style
        if (wParam == VK_RETURN)
        {
            ::PostMessageA(hWnd, WM_CLOSE, wParam, lParam);
            return 1;
        }

    return 0;
}

}
