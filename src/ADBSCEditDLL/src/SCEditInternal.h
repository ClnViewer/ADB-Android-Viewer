#pragma once

#if !defined(ADBSCEDI_INTERNAL_H)
#define ADBSCEDI_INTERNAL_H 1

#include "../../ADBViewer/src/loader.h"
#include <Commctrl.h>

// ID:
#define ID_ICON                          1001
#define ID_ACCEL                         1002
#define ID_EDITBOX                       1003
#define ID_REBAR                         1004
// ReBar bands
#define ID_REBAR_SEARCH                  1005
#define ID_REBAR_TOOLBAR                 1006
#define ID_REBAR_MENU                    1007
// bitmap sprites
#define ID_REBAR_BITMAP                  1008
#define ID_TOOLBAR_BITMAP                1009
// Info window rich edit control
#define ID_INFO_RTBOX                    1010
// Menu/Event/Button command:
#define IDM_MAIN_DBGSTEP                 1011      // ALT-S
#define IDM_MAIN_DBGCYCLE                1012      // ALT-C
#define IDM_MAIN_DBGNEXT                 1013      // ALT-N
#define IDM_MAIN_DBGBREAK                1014      // ALT-B
#define IDM_MAIN_DBGDUMP                 1015      // ALT-D
#define IDM_MAIN_DBGMODE                 1016      // button only
#define IDM_MAIN_EXIT                    1017      // ALT-Q
#define IDM_SCRUN_TEST                   1018      // button only
#define IDM_SCRUN_START                  1019      // button only
#define IDM_SCRUN_STOP                   1020      // button only
#define IDM_BTN_SCRUN_START              1021      // event only
#define IDM_BTN_SCRUN_STOP               1022      // event only
#define IDM_REBAR_FIND_TEXT              1023      // event only
#define IDM_FILE_NEW                     40001     // CTRL-N
#define IDM_FILE_OPEN                    40002     // CTRL-O
#define IDM_FILE_SAVE                    40003     // CTRL-S, F2
#define IDM_FILE_SAVEAS                  40004     // CTRL-???? TODO
#define IDM_EDIT_CUT                     40006     // CTRL-X
#define IDM_EDIT_COPY                    40007     // CTRL-C
#define IDM_EDIT_UNDO                    40008     // CTRL-Z
#define IDM_EDIT_REDO                    40009     // CTRL-Y
#define IDM_EDIT_PASTE                   40010     // CTRL-V
#define IDM_EDIT_DELETE                  40011     // key VK_DELETE
#define IDM_EDIT_SELECTALL               40012     // CTRL-A
#define IDM_EDIT_SHOW_NAVIGATE           2001      // button only
#define IDM_EDIT_SHOW_INDENTG            2002      // button only
#define IDM_EDIT_SHOW_ENDLINE            2003      // button only
#define IDM_EDIT_SHOW_DBGLINE            2004      // function only
#define IDM_EDIT_SHOW_ANNOTATION         2005      // function only
#define IDM_EDIT_SHOW_HELP               2006      // F1
#define IDM_MAIN_SHOW_VERSION            2007
#define IDM_MAIN_EXTDBGV                 2008      // ALT-V, F3
#define IDM_MAIN_WINTOP                  2009      // ALT-T

// Index toolBar button index:
#define TB_BTN_NAVIGATTE                 8
#define TB_BTN_MODE                      18
#define TB_BTN_CHECK                     19
#define TB_BTN_START                     20
#define TB_BTN_STOP                      21
#define TB_BTN_EXIT                      23

#if defined(__cplusplus)

#  include <algorithm>
#  include <iomanip>
#  include <string>
#  include <sstream>
#  include <thread>
#  include <chrono>
#  include <atomic>
#  include <vector>
#  include <map>
#  include "LoadDll.h"
#  include "WindowBase.h"
#  include "WindowDock.h"
#  include "LuaEngine.h"
#  include "SearchBar.h"
#  include "MenuBar.h"
#  include "ReBar.h"
#  include "ToolBar.h"
#  include "EditBox.h"
#  include "InfoBox.h"
#  include "SCEditConfig.h"
#  include "SCEditPrintBox.h"
#  include "SCEdit.h"

static inline const char g_scedit_error[] = "unknown error";
static inline const char g_scedit_default_skeleton[] =
    "\n\n"                                                                                                      \
    "-- skeleton.lua\n"                                                                                         \
    "-- Default ADB Viewer Lua script skeleton.\n"                                                              \
    "\n\n"                                                                                                      \
    "function main(n)\n"                                                                                        \
    "    local x = LuaObject:stateGet()\n"                                                                      \
    "	 print(\"state = \", n, \":\", x)\n"                                                                    \
    "    -- logical code body\n"                                                                                \
    "    -- see sense help and annotation\n"                                                                    \
    "	 print(\"main = \", n)\n"                                                                               \
    "    n = n + 1\n"                                                                                           \
    "    local z = x + n\n"                                                                                     \
    "    local s = \"test string\"\n"                                                                           \
    "    LuaObject:stateSet(n)\n"                                                                               \
    "end\n\n"                                                                                                   \
    "return 0\n";

static inline const char g_scedit_version[] =
    " -- Android ADB Viewer script editor "                                                                     \
    AVIEW_FULLVERSION_STRING ", build: "                                                                        \
    AVIEW_DATE "/" AVIEW_MONTH "/" AVIEW_YEAR                                                                   \
    " - started.";

static inline const char g_scedit_help[] =
    "\n- Editor key:\n"                                                                                         \
    "  - CTRL-N \t\t: Load default skeleton script (editor)\n"                                                  \
    "  - CTRL-O \t\t: Open script (editor)\n"                                                                   \
    "  - CTRL-S, F2\t: Save script (editor)\n"                                                                  \
    "  - CTRL-F2\t: Save script as .. (editor)\n"                                                               \
    "  - CTRL-A \t\t: Select all (editor)\n"                                                                    \
    "  - CTRL-C \t\t: Copy  (editor)\n"                                                                         \
    "  - CTRL-V \t\t: Paste (editor)\n"                                                                         \
    "  - CTRL-X \t\t: Cut   (editor)\n"                                                                         \
    "  - CTRL-Y \t\t: Redo  (editor)\n"                                                                         \
    "  - CTRL-Z \t\t: Undo  (editor)\n"                                                                         \
    "  - CTRL-Q \t\t: Quit  (all)\n"                                                                            \
    "  - ALT-T  \t\t: Set window always top (all)\n"                                                            \
    "  - F1 \t\t\t: toggle this short help (editor)\n"                                                          \
    "\n- Debugger key:\n"                                                                                       \
    "  - ALT-S, F4\t: Debug mode ' step by step ' (debugger)\n"                                                 \
    "  - ALT-C, F5\t: Debug mode ' auto cycle ' - default (debugger)\n"                                         \
    "  - ALT-N, F6\t: Go next line (debugger)\n"                                                                \
	"  - ALT-D, F7\t: Dump stack, only ' step by step ' mode (debugger)\n"                                      \
    "  - ALT-B, F8\t: Break running script - ' exit script ' (debugger)\n"                                      \
    "  - ALT-V, F3\t: Using external DebugView (debugger)\n"                                                    \
    "\n";

#endif

#endif
