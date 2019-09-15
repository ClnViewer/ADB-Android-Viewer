#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

namespace Editor
{
    void SCEdit::_OnSize(HWND hwnd, UINT, int32_t, int32_t)
    {
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
