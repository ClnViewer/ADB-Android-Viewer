
#include "../AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::_OnWM_NCHITTEST(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        POINT p{};
        RECT r{};
        ::GetCursorPos(&p);
        ::GetClientRect(GetDesktopWindow(), &r);
        if(::PtInRect(&r, p))
        {
            SetWindowLong(hWnd, DWL_MSGRESULT, HTCAPTION);
            return 1;
        }
        return 0;
}

}
