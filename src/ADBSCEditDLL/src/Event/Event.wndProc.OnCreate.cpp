
#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

#define break_error() { ERRORBox(); break; }

namespace Editor
{
    BOOL SCEdit::_OnCreate(HWND hwnd, LPCREATESTRUCT)
    {
        do
        {
            m_dock.SetParent(hwnd);

            if (!m_tb.create(hwnd, m_hinst))
                break_error();

            if (!m_tb.getsize(m_rect.rtbar))
                break_error();

            if (!m_hedit.create(hwnd, m_hinst, m_rect.redit, m_text))
                break_error();

            if (!m_hinfo.create(hwnd, m_hinst, m_rect.rinf, g_scedit_version))
                break_error();

            m_dock.AddChild(m_hinfo.handle());
            return TRUE;
        }
        while (0);
        return FALSE;
    }
};
