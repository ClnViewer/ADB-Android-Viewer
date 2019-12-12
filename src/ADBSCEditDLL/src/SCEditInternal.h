#pragma once

#if !defined(ADBSCEDIT_INTERNAL_H)
#define ADBSCEDIT_INTERNAL_H 1

#  if defined(ISOLATION_AWARE_ENABLED)
#    undef ISOLATION_AWARE_ENABLED
#  endif
#  define ISOLATION_AWARE_ENABLED 1

#include "../../ADBViewer/src/loader.h"
#include "SCEditDll.h"
#include <Commctrl.h>
#include <stdint.h>


// ID DIALOG:
#define IDD_DIALOG_1NUM                  901
#define IDD_DIALOG_2SR                   902
#define IDC_EDIT1                        910
#define IDC_EDIT2                        911
#define IDC_EDIT3                        912
// { IDD_DIALOG_2SR Control id
#  define IDC_REPLACEONCE                914
#  define IDC_REPLACEALL                 915
#  define IDC_FINDFIRST                  916
#  define IDC_RADIO_MWORD                917
#  define IDC_RADIO_MCASE                918
#  define IDC_RADIO_DDOWN                919
#  define IDC_RADIO_DUP                  920
// }

// ID:
#define ID_MAIN_ICON                     1000
#define ID_VIEWER_ICON                   1001
#define ID_ACCEL                         1002
#define ID_EDITBOX                       1003
#define ID_REBAR                         1004
//#define ID_???                         1005
// ReBar bands
#define ID_REBAR_SEARCH                  1006
#define ID_REBAR_TOOLBAR                 1007
#define ID_REBAR_MENU                    1008
#define ID_TAB_CTRL                      1009
// Info window rich edit control
#define ID_INFO_RTBOX                    1010
// Menu/Event/Button command:
#define IDM_BTN_DBGSTEP                  1011      // ALT-S
#define IDM_BTN_DBGCYCLE                 1012      // ALT-C
#define IDM_BTN_DBGNEXT                  1013      // ALT-N
#define IDM_BTN_DBGBREAK                 1014      // ALT-B
#define IDM_BTN_DBGDUMP                  1015      // ALT-D
#define IDM_BTN_DBGMODE                  1016      // button only
#define IDM_BTN_DBGMUTE                  1017      // menu only
#define IDM_BTN_EXIT                     1018      // ALT-Q
#define IDM_BTN_INFOCLEAR                1019      // ALT-Q
#define IDM_BTN_SCRUN_TEST               1020      // button only
#define IDM_BTN_SCRUN_START              1021      // button only
#define IDM_BTN_SCRUN_STOP               1022      // button only
#define IDM_EVENT_SCRUN_START            1023      // event only
#define IDM_EVENT_SCRUN_STOP             1024      // event only
#define IDM_EVENT_SCRUN_MONITOR          1025      // event only
#define IDM_EVENT_SET_CAPTION            1026      // event only
#define IDM_EDIT_NEW                     40001     // CTRL-N
#define IDM_EDIT_OPEN                    40002     // CTRL-O
#define IDM_EDIT_SAVE                    40003     // CTRL-S, F2
#define IDM_EDIT_SAVEAS                  40004     // CTRL-???? TODO
#define IDM_EDIT_CUT                     40006     // CTRL-X
#define IDM_EDIT_COPY                    40007     // CTRL-C
#define IDM_EDIT_UNDO                    40008     // CTRL-Z
#define IDM_EDIT_REDO                    40009     // CTRL-Y
#define IDM_EDIT_PASTE                   40010     // CTRL-V
#define IDM_EDIT_DELETE                  40011     // key VK_DELETE
#define IDM_EDIT_SELECTALL               40012     // CTRL-A
#define IDM_EDIT_SHOW_NAVIGATE           40013     // button only
#define IDM_EDIT_SHOW_INDENTG            40014     // button only
#define IDM_EDIT_SHOW_ENDLINE            40015     // button only

#define IDM_EDIT_GOTO_LINE               40016     // button / menu only
#define IDM_EDIT_COLOR_INSERT            40017     // button / menu only
#define IDM_EDIT_TEXT_REPLACE            40018     // button / menu only

#define IDM_TOOLBOX_SHOW_HELP            40019     // F1 / Menu (window)
#define IDM_TOOLBOX_SHOW_API             40020     // ? / Menu (window)
#define IDM_TOOLBOX_SHOW_MONITOR         40021     // ? / Menu (window)
#define IDM_EVENT_PLUGIN_INIT            40022     // event only
#define IDM_EVENT_TXT_INSERT             40023     // event only
#define IDM_EVENT_TXT_NEW                40024     // event only
//
#define IDM_EVENT_EDIT_DBGLINE           40025     // event only
#define IDM_EVENT_EDIT_ANNOTATION        40026     // event only
#define IDM_EVENT_EDIT_FINDTEXT          40027     // event only

#define IDM_EDIT_PASTE_CODE_0            41010     // menu only
#define IDM_EDIT_PASTE_CODE_1            41011     // menu only
#define IDM_EDIT_PASTE_CODE_2            41012     // menu only
#define IDM_EDIT_PASTE_CODE_3            41013     // menu only
#define IDM_EDIT_PASTE_CODE_4            41014     // menu only
#define IDM_EDIT_PASTE_CODE_5            41015     // menu only
#define IDM_EDIT_PASTE_CODE_6            41016     // menu only
#define IDM_EDIT_PASTE_CODE_7            41017     // menu only
#define IDM_EDIT_PASTE_CODE_8            41018     // menu only
#define IDM_EDIT_PASTE_CODE_9            41019     // menu only
#define IDM_EDIT_PASTE_CODE_10           41020     // menu only
#define IDM_EDIT_PASTE_CODE_11           41021     // menu only
#define IDM_EDIT_PASTE_CODE_12           41022     // menu only
#define IDM_EDIT_PASTE_CODE_13           41023     // menu only
#define IDM_EDIT_PASTE_CODE_14           41024     // menu only

#define IDM_BTN_SHOW_VERSION             42010
#define IDM_BTN_EXTDBGV                  42011     // ALT-V, F3
#define IDM_BTN_WINTOP                   42012     // ALT-T

#define IDM_BTN_PLUGIN_FIRST             43000     // Plugins ID first
#define IDM_BTN_PLUGIN_END               43100     // Plugins ID end
#define IDM_BTN_PLUGIN_RESCAN            (IDM_BTN_PLUGIN_END + 1)

// bitmap sprites
#define ID_IMGL_REBAR_BITMAP             44001
#define ID_IMGL_TITLE_BITMAP             44002
#define ID_IMGL_TOOLBAR_BITMAP           44003
#define ID_IMGL_TOOLAPI_BITMAP           44004

// tab index start
#define ID_TAB_IDX                       4000

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
#  include <memory>
#  include <array>
#  include <functional>
#  include <iterator>
#  include <optional>
#  include <regex>

#  include "Utils/LoadDll.h"
#  include "Utils/ExceptionPrint.h"
#  include "Utils/OLE/OLEInternal.h"
#  include "Utils/GUI/DialogInternal.h"

#  define _BUILD_IMAGELITE_DRAW 1
#  include <ImageLite.h>
#  include "MDIEngine/MDIInternal.h"
#  include <LuaEngine.Editor.h>
#  include <LuaPlugin.Editor.h>

# include "AppHelp/AppHelp.h"
# include "WinClass/WinBase.h"
# include "WinClass/MainWin/MainWin.h"
# include "WinClass/ClientWin/ClientWin.h"
# include "WinClass/EditBox/EditBox.h"
# include "WinClass/InfoBox/InfoBox.h"
# include "WinClass/ToolBar/ReBar.h"
# include "WinClass/ToolBar/MenuBar.h"
# include "WinClass/ToolBar/SearchBar.h"
# include "WinClass/ToolBar/ToolBar.h"
# include "WinClass/ToolBox/ToolBox.h"

# include "AppSCEdit.h"
# include "AppSCEdit.String.h"

//
#define CONF_MAIN_HANDLE()                                                                                                 \
    MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>()

//

#endif

#endif
