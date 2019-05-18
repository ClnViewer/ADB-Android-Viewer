
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_CHAR(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        // TODO (clanc#1#): ?? re.. CONTROL style
        if ((wParam == VK_RETURN) && (_sel > -1))
        {
            ::EndDialog(hWnd, _sel);
            return 1;
        }

    return 0;
}

}
