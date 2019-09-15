
#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

namespace Editor
{
    void SCEdit::_OnMove(HWND hwnd, int32_t x, int32_t)
    {
        if (x == 1) // y ??
            return;

        if (!calcWinSize_(hwnd))
        {
            ERRORBox();
            return;
        }
        m_dock.WindowMoved(hwnd);
        m_hedit.setsize(m_rect.redit);
        m_hinfo.setsize(m_rect.rinf);
    }
};
