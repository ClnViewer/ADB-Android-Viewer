#pragma once

#if !defined(ADBSCEDI_INTERNAL_H)
#define ADBSCEDI_INTERNAL_H 1

#include "../../ADBViewer/src/loader.h"
#include "SCEditDll.h"
#include <Commctrl.h>

// ID:
#define ID_MAIN_ICON                     1000
#define ID_VIEWER_ICON                   1001
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

#define IDM_EDIT_PASTE_CODE_0            2010      // menu only
#define IDM_EDIT_PASTE_CODE_1            2011      // menu only
#define IDM_EDIT_PASTE_CODE_2            2012      // menu only
#define IDM_EDIT_PASTE_CODE_3            2013      // menu only
#define IDM_EDIT_PASTE_CODE_4            2014      // menu only
#define IDM_EDIT_PASTE_CODE_5            2015      // menu only
#define IDM_EDIT_PASTE_CODE_6            2016      // menu only
#define IDM_EDIT_PASTE_CODE_7            2017      // menu only
#define IDM_EDIT_PASTE_CODE_8            2018      // menu only
#define IDM_EDIT_PASTE_CODE_9            2019      // menu only
#define IDM_EDIT_PASTE_CODE_10           2020      // menu only
#define IDM_EDIT_PASTE_CODE_11           2021      // menu only
#define IDM_EDIT_PASTE_CODE_12           2022      // menu only
#define IDM_EDIT_PASTE_CODE_13           2023      // menu only

#define IDM_MAIN_SHOW_VERSION            2051
#define IDM_MAIN_EXTDBGV                 2052      // ALT-V, F3
#define IDM_MAIN_WINTOP                  2053      // ALT-T

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

#  define _BUILD_IMAGELITE_DRAW 1
#  include <ImageLite.h>
#  include <LuaEngineEditor.h>

#  include "WindowBase.h"
#  include "WindowDock.h"
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
#if defined(_DEBUG)
    "\n\n"                                                                                                      \
    "-- skeleton.lua\n"                                                                                         \
    "-- Default ADB Viewer Lua script skeleton.\n"                                                              \
    "\n\n"                                                                                                      \
    "require \"imageTable\"\n"                                                                                  \
    "\n\n"                                                                                                      \
    "function main(n)\n"                                                                                        \
    "    local x = LuaObject:stateGet()\n"                                                                      \
    "    print(\"state = \", n, \":\", x)\n"                                                                    \
    "    \n"                                                                                                    \
    "    local default_object = {\n"                                                                            \
    "       width  = 38,\n"                                                                                     \
    "       height = 32,\n"                                                                                     \
    "       itype  = 0,\n"                                                                                      \
    "       data = {{ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }}\n"                                                   \
    "    };\n"                                                                                                  \
    "    local int_object = { 1, 2, 3, 4, 5 }\n"                                                                \
    "    \n"                                                                                                    \
    "    -- set default image\n"                                                                                \
    "    local s = LuaObject:imageDefault(\"TestImageLite0.png\")\n"                                            \
    "    print(\"default frame buffer emulate image = \", s)\n"                                                 \
    "    \n"                                                                                                    \
    "    -- get Android frame buffer, LuaImage format\n"                                                        \
    "    local img_screen0 = LuaObject:screenGet()\n"                                                           \
    "    print(\"screen: width = \", img_screen0.width, \", height = \", img_screen0.height)\n"                 \
    "    print(\"screen type = \", img_screen0.itype)\n"                                                        \
    "    LuaObject:imageTableShow(img_screen0)\n"                                                               \
    "    \n"                                                                                                    \
    "    -- get region from Android frame buffer, LuaImage format\n"                                            \
    "    local img_screen1 = LuaObject:screenGetCord(50, 20, 200, 200)\n"                                       \
    "    print(\"screen region: width = \", img_screen1.width, \", height = \", img_screen1.height)\n"          \
    "    print(\"screen region type = \", img_screen1.itype)\n"                                                 \
    "    LuaObject:imageTableShow(img_screen1)\n"                                                               \
    "    \n"                                                                                                    \
    "    -- iterate LuaImage\n"                                                                                 \
    "       for k,v in ipairs(img_screen1.data) do\n"                                                           \
    "            print(k, \" = \", v[1], \":\", v[2], \":\", v[3])\n"                                           \
    "       end\n"                                                                                              \
    "    \n"                                                                                                    \
    "    LuaObject:imageTableShow(default_object)\n"                                                            \
    "    LuaObject:imageTableShow(int_object)\n"                                                                \
    "    LuaObject:imageTableShow(default_img)\n"                                                               \
    "    \n"                                                                                                    \
    "    local pos = LuaObject:imageGetPosition(100, 200)\n"                                                    \
    "    --* checkPixelByCord(x, y, R, G, B)\n"                                                                \
    "    local b1  = LuaObject:checkPixelByCord(970, 400, 77, 182, 172)\n"                                     \
    "    local b2 = false\n"                                                                                    \
    "    if pos ~= nil then\n"                                                                                  \
    "        --* checkPixelByPos(position, R, G, B)\n"                                                         \
    "        b2  = LuaObject:checkPixelByPos(pos, 204, 204, 204)\n"                                            \
    "    end\n"                                                                                                 \
    "    -- compare image\n"                                                                                    \
    "    -- default screen test image is name \"scedit-default.png\"\n"                                         \
    "    local d = LuaObject:imageCompare(\"TestImageLite0.png\")\n"                                            \
    "    --* imageCompareRegion(file name, x, y, w, h)\n"                                                       \
    "    local r = LuaObject:imageCompareRegion(\"TestImageLite0.png\", 0, 0, 100, 200)\n"                      \
    "    -- show/edit image\n"                                                                                  \
    "    LuaObject:imageShow(\"TestImageLite0.png\")\n"                                                         \
    "    LuaObject:imageShow(\"TestImageLite1.png\")\n"                                                         \
    "    LuaObject:imageShow(\"TestImageLite2.png\")\n"                                                         \
    "    LuaObject:imageShow(\"TestImageLite3.png\")\n"                                                         \
    "    \n"                                                                                                    \
    "    -- logical code body\n"                                                                                \
    "    print(\"main = \", n)\n"                                                                               \
    "    n = n + 1\n"                                                                                           \
    "    local z = 0\n"                                                                                         \
    "    if x ~= nil then\n"                                                                                    \
    "        z = x + n\n"                                                                                       \
    "    end\n"                                                                                                 \
    "    local s = \"test string\"\n"                                                                           \
    "    local t = {}\n"                                                                                        \
    "    t[0] = 10\n"                                                                                           \
    "    t[1] = 100\n"                                                                                          \
    "    t[2] = 1000\n"                                                                                         \
    "    -- see sense help and annotation\n"                                                                    \
    "    \n"                                                                                                    \
    "    x = x + 1\n"                                                                                           \
    "    LuaObject:stateSet(x)\n"                                                                               \
    "end\n\n"                                                                                                   \
    "return 0\n";
#else
    "\n\n"                                                                                                      \
    "-- skeleton.lua\n"                                                                                         \
    "-- Default ADB Viewer Lua script skeleton.\n"                                                              \
    "\n\n"                                                                                                      \
    "function main(n)\n"                                                                                        \
    "    local cstate = LuaObject:stateGet()\n"                                                                 \
    "    if state ~= cstate then\n"                                                                             \
    "        print(\"state = \", state, \":\", cstate)\n"                                                       \
    "    end\n"                                                                                                 \
    "    \n\n"                                                                                                  \
    "    -- logical code body\n"                                                                                \
    "    \n"                                                                                                    \
    "        -- set default image\n"                                                                            \
    "        -- development function, only editor support\n"                                                    \
    "        local s = LuaObject:imageDefault(\"scedit-default.png\")\n"                                        \
    "        print(\"default frame buffer emulate image set = \", s)\n"                                         \
    "    \n"                                                                                                    \
    "        -- see editor menu -> <Edit> -> <Insert Lua code> for more snippet example\n"                      \
    "    \n"                                                                                                    \
    "            if state == 0 then LuaObject:adbClick(x,y)\n"                                                  \
    "        elseif state == 1 then LuaObject:adbClick(x,y)\n"                                                  \
    "        elseif state == 2 then LuaObject:adbClick(x,y)\n"                                                  \
    "        -- etc ..\n"                                                                                       \
    "    \n"                                                                                                    \
    "    -- any code\n"                                                                                         \
    "    \n"                                                                                                    \
    "    state = state + 1\n"                                                                                   \
    "    LuaObject:stateSet(state)\n"                                                                           \
    "    LuaObject:stateSleep(10)\n"                                                                            \
    "end\n\n"                                                                                                   \
    "return 0\n";
#endif

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
    "  - ALT-D, F6\t: Dump stack, only ' step by step ' mode (debugger)\n"                                      \
    "  - ALT-N, F7\t: Go next line (debugger)\n"                                                                \
    "  - ALT-B, F8\t: Break running script - ' exit script ' (debugger)\n"                                      \
    "  - ALT-V, F3\t: Using external DebugView (debugger)\n"                                                    \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_0[] =
    "\n\n"                                                                                                      \
    "-- main function template, 'Click' mode\n"                                                                 \
    "function main(stateOld)\n"                                                                                 \
    "    local state = LuaObject:stateGet()\n"                                                                  \
    "    -- good state value == (stateOld + 1)\n"                                                               \
    "    if state ~= stateOld then\n"                                                                           \
    "        print(\"state = \", state, \":\", stateOld)\n"                                                     \
    "    end\n"                                                                                                 \
    "    \n"                                                                                                    \
    "    -- code begin\n"                                                                                       \
    "    \n"                                                                                                    \
    "        local tbl_click = {\n"                                                                                 \
    "           {110,210},\n"                                                                                       \
    "           {120,220},\n"                                                                                       \
    "           {130,230}\n"                                                                                        \
    "        }\n"                                                                                                   \
    "            if stateOld == 0 then LuaObject:adbClick(tbl_click[1][1], tbl_click[1][2])\n"                  \
    "        elseif stateOld == 1 then LuaObject:adbClick(tbl_click[2][1], tbl_click[2][2])\n"                  \
    "        elseif stateOld == 2 then LuaObject:adbClick(tbl_click[3][1], tbl_click[3][2])\n"                  \
    "        -- etc ..\n"                                                                                       \
    "    \n"                                                                                                    \
    "    -- code end\n\n"                                                                                       \
    "    stateOld = stateOld + 1\n"                                                                             \
    "    LuaObject:stateSet(stateOld)\n"                                                                        \
    "    LuaObject:stateSleep(5)\n"                                                                             \
    "end\n\n"                                                                                                   \
    "return 0\n";

static inline const char g_scedit_EDIT_PASTE_CODE_1[] =
    "\n\n"                                                                                                      \
    "-- main function template, 'Check & Click' mode\n"                                                         \
    "function main(stateOld)\n"                                                                                 \
    "    local state = LuaObject:stateGet()\n"                                                                  \
    "    -- good state value == (stateOld + 1)\n"                                                               \
    "    if state ~= stateOld then\n"                                                                           \
    "        print(\"state = \", state, \":\", stateOld)\n"                                                     \
    "    end\n"                                                                                                 \
    "    \n"                                                                                                    \
    "    -- code begin\n"                                                                                       \
    "    \n"                                                                                                    \
    "        local tbl_pixels_by_cord = {\n"                                                                    \
    "           {110,210, 127,139,143},\n"                                                                      \
    "           {120,220, 77,182,172},\n"                                                                       \
    "           {130,230, 77,182,172}\n"                                                                        \
    "        }\n"                                                                                               \
    "        local tbl_click = {\n"                                                                             \
    "           {110,210},\n"                                                                                   \
    "           {120,220},\n"                                                                                   \
    "           {130,230}\n"                                                                                    \
    "        }\n"                                                                                               \
    "        local cmp = LuaObject:checkPixelsByCord(tbl_pixels_by_cord)\n"                                     \
    "        if cmp == true then\n"                                                                             \
    "            LuaObject:adbClick(tbl_click[1][1], tbl_click[1][2])\n"                                        \
    "        else\n"                                                                                            \
    "            print(\"screen NOT equals\")\n"                                                                \
    "        end\n"                                                                                             \
    "    \n"                                                                                                    \
    "    -- code end\n\n"                                                                                       \
    "    stateOld = stateOld + 1\n"                                                                             \
    "    LuaObject:stateSet(stateOld)\n"                                                                        \
    "    LuaObject:stateSleep(5)\n"                                                                             \
    "end\n\n"                                                                                                   \
    "return 0\n";

static inline const char g_scedit_EDIT_PASTE_CODE_2[] =
    "    -- compare image\n"                                                                                    \
    "    local cmp = LuaObject:imageCompare(\"MyImage0.png\")\n"                                                \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"image NOT equals\")\n"                                                                     \
    "    else\n"                                                                                                \
    "        print(\"image equals\")\n"                                                                         \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_3[] =
    "    -- compare image region,\n"                                                                            \
    "    -- region is frame buffer source image rectangle\n"                                                    \
    "    local cmp = LuaObject:imageCompareRegion(\"MyImage0.png\", 0, 0, 100, 200)\n"                          \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"image NOT equals\")\n"                                                                     \
    "    else\n"                                                                                                \
    "        print(\"image equals\")\n"                                                                         \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_4[] =
    "    -- get Android frame buffer, LuaImage format\n"                                                        \
    "    local img_screen = LuaObject:screenGet()\n"                                                            \
    "    if img_screen ~= nil then\n"                                                                           \
    "        print(\"screen: width = \", img_screen.width, \", height = \", img_screen.height)\n"               \
    "        print(\"screen  type  = \", img_screen.itype)\n"                                                   \
    "        -- show new image, development only!\n"                                                            \
    "        LuaObject:imageTableShow(img_screen)\n"                                                            \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_5[] =
    "    -- get region from Android frame buffer, LuaImage format\n"                                            \
    "    -- x, y - offset from start, w, h - width and height capture image\n"                                  \
    "    local img_screen = LuaObject:screenGetCord(50, 20, 200, 200)\n"                                        \
    "    if img_screen ~= nil then\n"                                                                           \
    "        print(\"screen: width = \", img_screen.width, \", height = \", img_screen.height)\n"               \
    "        print(\"screen  type  = \", img_screen.itype)\n"                                                   \
    "        -- show new image, development only!\n"                                                            \
    "        LuaObject:imageTableShow(img_screen)\n"                                                            \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_6[] =
    "    -- iterate image, LuaImage format\n"                                                                   \
    "    local img_screen = LuaObject:screenGet()\n"                                                            \
    "    if img_screen ~= nil then\n"                                                                           \
    "        for k,v in ipairs(img_screen.data) do\n"                                                           \
    "            print(k, \" = \", v[1], \":\", v[2], \":\", v[3])\n"                                           \
    "        end\n"                                                                                             \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_7[] =
    "    -- save image, from LuaImage format\n"                                                                 \
    "    -- first argument: name of saved image, second argument: count of save image\n"                        \
    "    LuaObject:imageSave(\"MySaveImage\", 3)\n"                                                             \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_8[] =
    "    -- check pixel by position\n"                                                                          \
    "    local pos = LuaObject:imageGetPosition(100, 200)\n"                                                    \
    "    -- format: checkPixelByPos(position, { R, G, B })\n"                                                   \
    "    local cmp = LuaObject:checkPixelByPos(pos, { 77, 182, 172 })\n"                                        \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"pixel NOT equals\")\n"                                                                     \
    "    else\n"                                                                                                \
    "        print(\"pixel equals\")\n"                                                                         \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_9[] =
    "    -- check pixel by coordinate\n"                                                                        \
    "    -- format: checkPixelByCord(x, y, { R, G, B })\n"                                                      \
    "    local cmp = LuaObject:checkPixelByCord(100, 200, { 77, 182, 172 })\n"                                  \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"pixel NOT equals\")\n"                                                                     \
    "    else\n"                                                                                                \
    "        print(\"pixel equals\")\n"                                                                         \
    "    end\n"                                                                                                 \
    "\n";


static inline const char g_scedit_EDIT_PASTE_CODE_10[] =
    "    -- check pixels array by position\n"                                                                   \
    "    -- format: pixels array(4) {{ position, R, G, B }, {...}, ..}\n"                                       \
    "    local tbl_pixels_by_pos = {\n"                                                                         \
    "       {1437840, 127,139,143},\n"                                                                          \
    "       {1262451, 77,182,172},\n"                                                                           \
    "       {1271676, 77,182,172}\n"                                                                            \
    "    }\n"                                                                                                   \
    "    local cmp = LuaObject:checkPixelsByPos(tbl_pixels_by_pos)\n"                                           \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"pixels NOT equals\")\n"                                                                    \
    "    else\n"                                                                                                \
    "        print(\"pixels equals\")\n"                                                                        \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_11[] =
    "    -- check pixels array by coordinate\n"                                                                 \
    "    -- format: pixels array(5) {{ x, y, R, G, B }, {...}, ..}\n"                                           \
    "    local tbl_pixels_by_cord = {\n"                                                                        \
    "       {110,210, 127,139,143},\n"                                                                          \
    "       {120,220, 77,182,172},\n"                                                                           \
    "       {130,230, 77,182,172}\n"                                                                            \
    "    }\n"                                                                                                   \
    "    local cmp = LuaObject:checkPixelsByCord(tbl_pixels_by_cord)\n"                                         \
    "    if cmp ~= true then\n"                                                                                 \
    "        print(\"pixels NOT equals\")\n"                                                                    \
    "    else\n"                                                                                                \
    "        print(\"pixels equals\")\n"                                                                        \
    "    end\n"                                                                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_12[] =
    "    -- set default image, emulate Android frame buffer\n"                                                  \
    "    -- editor option, development only!\n"                                                                 \
    "    local s = LuaObject:imageDefault(\"scedit-default.png\")\n"                                            \
    "    print(\"default frame buffer emulate image = \", s)\n"                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_13[] =
    "    -- example LuaImage format\n"                                                                          \
    "    local LuaImage_object = {\n"                                                                           \
    "       width  = 32,\n"                                                                                     \
    "       height = 32,\n"                                                                                     \
    "    -- pixels array format: IBT_RGB == 0\n"                                                                \
    "       itype  = 0,\n"                                                                                      \
    "    -- pixels array: { R, G, B }\n"                                                                        \
    "       data = {{ 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, ...}\n"                                        \
    "    };\n"                                                                                                  \
    "\n";

#endif

#endif
