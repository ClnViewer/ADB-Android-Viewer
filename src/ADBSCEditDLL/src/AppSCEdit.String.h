#pragma once

#if !defined(ADBSCEDIT_STRING_H)
#define ADBSCEDIT_STRING_H 1


#if defined(__cplusplus)

static inline const char g_scedit_error[] = "unknown error";
static inline const char g_scedit_saved[] = "script NOT saved, press twice to exit without saving..";
static inline const char g_scedit_load[]  = "error load script from editor panel";
static inline const char g_scedit_empty[] = "NOT run empty script";
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
    "    print(\"state = \", x, \", old return = \", n)\n"                                                      \
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
    "    --* checkPixelByCord(x, y, R, G, B)\n"                                                                 \
    "    local b1  = LuaObject:checkPixelByCord(970, 400, 77, 182, 172)\n"                                      \
    "    local b2 = false\n"                                                                                    \
    "    if pos ~= nil then\n"                                                                                  \
    "        --* checkPixelByPos(position, R, G, B)\n"                                                          \
    "        b2  = LuaObject:checkPixelByPos(pos, 204, 204, 204)\n"                                             \
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
    "    return n + 5\n"                                                                                        \
    "end\n\n"                                                                                                   \
    "return 0\n";
#else
    "\n\n"                                                                                                      \
    "-- skeleton.lua\n"                                                                                         \
    "-- Default ADB Viewer Lua script skeleton.\n"                                                              \
    "\n\n"                                                                                                      \
    "function main(arg)\n"                                                                                      \
    "    -- pass numeric value main(arg) from returned prevision call\n"                                        \
    "    local state = LuaObject:stateGet()\n"                                                                  \
    "    print(\"state = \", state, \", old return value = \", arg)\n"                                          \
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
    "        -- example check numeric value main(arg)\n"                                                        \
    "        if arg == -1 then\n"                                                                               \
    "            LuaObject:stateSet(1)\n"                                                                       \
    "            return 0\n"                                                                                    \
    "        end\n"                                                                                             \
    "    \n"                                                                                                    \
    "        local x = 55\n"                                                                                    \
    "        local y = 120\n"                                                                                   \
    "    \n"                                                                                                    \
    "            if state == 0 then LuaObject:adbClick(x + 10, y + 20)\n"                                       \
    "        elseif state == 1 then LuaObject:adbClick(x + 15, y + 30)\n"                                       \
    "        elseif state == 2 then LuaObject:adbClick(x + 20, y + 40)\n"                                       \
    "        -- etc ..\n"                                                                                       \
    "        elseif state == 3 then arg = -1\n"                                                                 \
    "        end\n"                                                                                             \
    "    \n"                                                                                                    \
    "    -- any code\n"                                                                                         \
    "    \n"                                                                                                    \
    "    state = state + 1\n"                                                                                   \
    "    LuaObject:stateSet(state)\n"                                                                           \
    "    LuaObject:stateSleep(10)\n"                                                                            \
    "    -- save numeric value, returned as argument main(arg) from next call\n"                                \
    "    return arg\n"                                                                                          \
    "end\n\n"                                                                                                   \
    "return 0\n";
#endif

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
    "function main(valueOld)\n"                                                                                 \
    "    -- pass numeric value main(arg) from returned prevision call\n"                                        \
    "    local state = LuaObject:stateGet()\n"                                                                  \
    "    print(\"state = \", state, \", old return value = \", valueOld)\n"                                     \
    "    \n"                                                                                                    \
    "    -- code begin\n"                                                                                       \
    "    \n"                                                                                                    \
    "        local tbl_click = {\n"                                                                             \
    "           {110,210},\n"                                                                                   \
    "           {120,220},\n"                                                                                   \
    "           {130,230}\n"                                                                                    \
    "        }\n"                                                                                               \
    "            if state == 0 then LuaObject:adbClick(tbl_click[1][1], tbl_click[1][2])\n"                     \
    "        elseif state == 1 then LuaObject:adbClick(tbl_click[2][1], tbl_click[2][2])\n"                     \
    "        elseif state == 2 then LuaObject:adbClick(tbl_click[3][1], tbl_click[3][2])\n"                     \
    "        -- etc ..\n"                                                                                       \
    "        end\n"                                                                                             \
    "    \n"                                                                                                    \
    "    -- code end\n\n"                                                                                       \
    "    LuaObject:stateSleep(5)\n"                                                                             \
    "    \n"                                                                                                    \
    "    state = state + 1\n"                                                                                   \
    "    LuaObject:stateSet(state)\n"                                                                           \
    "    -- save numeric value, returned as argument main(arg) from next call\n"                                \
    "    return valueOld\n"                                                                                     \
    "end\n\n"                                                                                                   \
    "return 0\n";

static inline const char g_scedit_EDIT_PASTE_CODE_1[] =
    "\n\n"                                                                                                      \
    "-- main function template, 'Check & Click' mode\n"                                                         \
    "function main(valueOld)\n"                                                                                 \
    "    -- pass numeric value main(arg) from returned prevision call\n"                                        \
    "    local state = LuaObject:stateGet()\n"                                                                  \
    "    print(\"state = \", state, \", old return value = \", valueOld)\n"                                     \
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
    "            valueOld = 1\n"                                                                                \
    "        else\n"                                                                                            \
    "            print(\"screen NOT equals\")\n"                                                                \
    "            valueOld = 0\n"                                                                                \
    "        end\n"                                                                                             \
    "    \n"                                                                                                    \
    "    -- code end\n\n"                                                                                       \
    "    LuaObject:stateSleep(5)\n"                                                                             \
    "    \n"                                                                                                    \
    "    state = state + 1\n"                                                                                   \
    "    LuaObject:stateSet(state)\n"                                                                           \
    "    -- save numeric value, returned as argument main(arg) from next call\n"                                \
    "    return valueOld\n"                                                                                     \
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
    "        -- show new image, LuaImage format\n"                                                              \
    "        -- warning, working only this editor, development only!\n"                                         \
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
    "        -- show new image, LuaImage format\n"                                                              \
    "        -- warning, working only this editor, development only!\n"                                         \
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
    "    -- warning, working only this editor, development only!\n"                                             \
    "    local s = LuaObject:imageDefault(\"scedit-default.png\")\n"                                            \
    "    print(\"default frame buffer emulate image = \", s)\n"                                                 \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_13[] =
    "    \n"                                                                                                    \
    "    -- using DebugView or other software for show trace message\n"                                         \
    "    -- warning, working only Lua ADB Viewer plugin\n"                                                      \
    "    -- in editor, checking menu ' Debug ' -> ' Using DebugView ' or press ' ALT-V '\n"                     \
    "    -- DebugView's links:\n"                                                                               \
    "    -- 1.  https://debugviewpp.wordpress.com\n"                                                            \
    "    -- 1.  https://github.com/CobaltFusion/DebugViewPP/files/1408599/DebugView.v1.5.zip\n"                 \
    "    -- 2.  https://docs.microsoft.com/ru-ru/sysinternals/downloads/debugview\n"                            \
    "    -- 2.  https://download.sysinternals.com/files/DebugView.zip\n"                                        \
    "    LuaObject:traceOn()\n"                                                                                 \
    "    -- trace code begin\n"                                                                                 \
    "    \n"                                                                                                    \
    "        print(\"I'm trace!\")\n"                                                                           \
    "    \n"                                                                                                    \
    "    -- trace code end\n"                                                                                   \
    "    LuaObject:traceOff()\n"                                                                                \
    "\n";

static inline const char g_scedit_EDIT_PASTE_CODE_14[] =
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
