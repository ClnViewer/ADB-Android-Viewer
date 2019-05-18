
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_COMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) lParam;

        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            ::EndDialog(hWnd, _sel);
            return 1;
        }
        case IDCANCEL:
        {
            ::EndDialog(hWnd, -1);
            return 1;
        }
        default:
            break;
        }
  return 0;
}

}
