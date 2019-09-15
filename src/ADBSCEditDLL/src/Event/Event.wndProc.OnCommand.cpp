
#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

#define _CMD_BUILD(A,B)   \
    case A: { B; break; }

#define _CMD_BUILD2(A,B,C) \
    case A:                \
    case B: { C; break; }

namespace Editor
{
    void SCEdit::_OnCommand(HWND hwnd, UINT, WPARAM wParam, LPARAM lParam)
    {
        int32_t id = LOWORD(wParam);
        switch (id)
        {
#           include "Command/cmd_MAIN_EXIT.cxx"
#           include "Command/cmd_FILE_NEW.cxx"
#           include "Command/cmd_FILE_OPEN.cxx"
#           include "Command/cmd_FILE_SAVE.cxx"               // and IDM_FILE_SAVEAS
#           include "Command/cmd_EDIT_UNDO.cxx"
#           include "Command/cmd_EDIT_REDO.cxx"
#           include "Command/cmd_EDIT_COPY.cxx"
#           include "Command/cmd_EDIT_PASTE.cxx"
#           include "Command/cmd_EDIT_CUT.cxx"
#           include "Command/cmd_EDIT_DELETE.cxx"
#           include "Command/cmd_EDIT_SELECTALL.cxx"
#           include "Command/cmd_EDIT_SHOW_NAVIGATE.cxx"
#           include "Command/cmd_EDIT_SHOW_ENDLINE.cxx"
#           include "Command/cmd_EDIT_SHOW_INDENTG.cxx"
#           include "Command/cmd_EDIT_SHOW_DBGLINE.cxx"
#           include "Command/cmd_EDIT_SHOW_HELP.cxx"
#           include "Command/cmd_EDIT_SHOW_ANNOTATION.cxx"
#           include "Command/cmd_MAIN_SHOW_VERSION.cxx"
#           include "Command/cmd_REBAR_FIND_TEXT.cxx"
#           include "Command/cmd_SCRUN_START.cxx"             // and IDM_SCRUN_TEST
#           include "Command/cmd_SCRUN_STOP.cxx"
#           include "Command/cmd_BTN_SCRUN_STOP.cxx"
#           include "Command/cmd_BTN_SCRUN_START.cxx"
#           include "Command/cmd_MAIN_DBGMODE.cxx"
#           include "Command/cmd_MAIN_DBGSTEP.cxx"
#           include "Command/cmd_MAIN_DBGCYCLE.cxx"
#           include "Command/cmd_MAIN_DBGNEXT.cxx"
#           include "Command/cmd_MAIN_DBGBREAK.cxx"
#           include "Command/cmd_MAIN_DBGDUMP.cxx"
#           include "Command/cmd_MAIN_EXTDBGV.cxx"
#           include "Command/cmd_MAIN_WINTOP.cxx"

            default: break;
        }
    }
};
