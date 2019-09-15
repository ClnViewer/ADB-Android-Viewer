#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

namespace Editor
{
    UINT SCEdit::_OnNCHitTest(HWND, int32_t, int32_t)
    {
        //const LRESULT ret = ::DefWindowProcA(hwnd_, umsg_, wp_, lp_);
        //return (ret == HTCLIENT) ? HTCAPTION : ret;
        return HTCAPTION;
    }
};
