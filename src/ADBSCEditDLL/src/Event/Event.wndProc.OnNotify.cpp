#include "../SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

namespace Editor
{
    BOOL SCEdit::_OnNotify(HWND, int32_t, LPNMHDR lph)
    {
        if (!lph)
            return FALSE;

        if (lph->idFrom == ID_EDITBOX)
        {
            m_hedit.notify((void*)lph);
            return TRUE;
        }
        if ((lph->idFrom == ID_REBAR) && (lph->code == RBN_HEIGHTCHANGE))
        {
            if (!m_tb.getsize(m_rect.rtbar))
            {
                ERRORBox();
                return TRUE;
            }
            calcWinSize_();
            m_hedit.setsize(m_rect.redit);
            return TRUE;
        }
        return FALSE;
    }

};
