
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_CLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        ::EndDialog(hWnd, _sel);
  return 1;
}

}
