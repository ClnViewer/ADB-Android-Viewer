
#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

namespace Editor
{
    void SCEdit::_OnClose(HWND)
    {
        ::PostQuitMessage(0);
    }
};
