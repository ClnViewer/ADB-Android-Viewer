
#pragma once

#   define _FIND_INDICATOR    8
#   define _INDEX_MARGIN_LNUM 0
#   define _INDEX_MARGIN_FOLD 1
#   define _INDEX_MARGIN_MARK 2

    static inline const char  l_ebFileDesc[]   = "Lua script files (*.lua)\0*.lua\0";
    static inline const char  l_ebFileExt[]    = "lua";
    static inline const char  l_ebFilePath[]   = ".\\";
    //
    static inline const char  l_ebFileName[]   = "plugin-lua.lua";
    static inline const char  l_ebErrorGet[]   = "unable to get saved text from editor";
    static inline const char  l_ebErrorSize[]  = "the opened file has zero length";
    static inline const char  l_ebErrorName[]  = "open file name not defined or empty";
    //
    static inline const char  l_dropEmpty[]    = " - Dropping file is empty..";
    static inline const char  l_dropWarn[]     = " - Dropping multiple files is not supported..";
    static inline const char  l_dropOpen[]     = " - Opening drop file: ";
    static inline const char  l_dropErrorExt[] = "file extension not support, accept only .lua script files.";
    static inline const char  l_dropErrorHandle[] = "not opening drop file, class handle broken..";
    //
    static inline const char  l_autoAppend[]   = "(";
    static inline const char  l_autoStop[]     = ");";
    static inline const char  l_buildLuaRoot[] = "LuaObject";
    static inline const char  l_buildObject[]  =
#   define LFUN__(S,...)      LFUN ## S(__VA_ARGS__)
#   define LFUN_FIRST(A,...)  STR_(A)
#   define LFUN_NEXT(A,...)   " " STR_(A)
#   define LFUN_END(...)      LFUN_NEXT(__VA_ARGS__)
#   include <LuaPluginFunction.h>
    ;
    static inline const char  l_autoObject[]  =
#   define LFUN__(S,...)      LFUN ## S(__VA_ARGS__)
#   define LFUN_FIRST(A,...)  STR_(A) "?1 "
#   define LFUN_NEXT(...)     LFUN_FIRST(__VA_ARGS__)
#   define LFUN_END(A,...)    STR_(A) "?1"
#   include <LuaPluginFunction.h>
    ;
    static inline const char  l_luaWord3[] =
                                   "and break do else elseif end false for function if in local " \
                                   "nil not or repeat return then true until while goto";
    static inline const char  l_luaWord4[] =
                                   "assert collectgarbage dofile error _G getmetatable ipairs loadfile " \
                                   "next pairs pcall print rawequal rawget rawset setmetatable tonumber " \
                                   "tostring type _VERSION xpcall string table math coroutine io os debug " \
                                   "getfenv gcinfo load loadlib loadstring require module select setfenv unpack " \
                                   "_LOADED LUA_PATH _REQUIREDNAME package rawlen package bit32 utf8 _ENV " \
                                   "coroutine.create coroutine.resume coroutine.status coroutine.wrap coroutine.yield coroutine.running coroutine.isyieldable " \
                                   "package.loaders package.seeall package.config package.searchers package.searchpath " \
                                   "package.cpath package.loaded package.loadlib package.path package.preload";
    static inline const char  l_luaWord5[] =
                                   "string.byte string.char string.dump string.find string.format string.gsub " \
                                   "string.len string.lower string.rep string.sub string.upper table.concat " \
                                   "string.gfind string.gmatch string.match string.reverse " \
                                   "string.pack string.packsize string.unpack table.foreach table.foreachi table.getn table.setn " \
                                   "table.insert table.remove table.sort table.maxn table.pack table.unpack table.move " \
                                   "utf8.char utf8.charpattern utf8.codes utf8.codepoint utf8.len utf8.offset";
    static inline const char  l_luaWord6[] =
                                   "io.close io.flush io.input io.lines io.open io.output io.read io.tmpfile io.type " \
                                   "io.write io.stdin io.stdout io.stderr io.popen os.clock os.date os.difftime os.execute os.exit " \
                                   "os.getenv os.remove os.rename os.setlocale os.time os.tmpname";
    static inline const char  l_luaWord7[] =
                                   "math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.deg math.exp math.floor math.frexp math.ldexp " \
                                   "math.log math.max math.min math.pi math.pow math.rad math.random math.randomseed " \
                                   "math.sin math.sqrt math.tan math.cosh math.fmod math.huge math.log10 math.modf " \
                                   "math.mod math.sinh math.tanh math.maxinteger math.mininteger math.tointeger math.type math.ult " \
                                   "bit32.arshift bit32.band bit32.bnot bit32.bor bit32.btest bit32.bxor bit32.extract bit32.replace " \
                                   "bit32.lrotate bit32.lshift bit32.rrotate bit32.rshift";

#   if defined(EDITBOX_STYLE_ACTIVE)
    static inline const char *l_autoMethodXpm[] =
    {
        "12 12 3 1",
        " 	c None",
        ".	c #4B8AF2",
        "+	c #808080",
        "            ",
        "   ...+     ",
        "    ...+    ",
        "     ...+   ",
        "      ...+  ",
        "      ....+ ",
        "      ....+ ",
        "      ...+  ",
        "     ...+   ",
        "    ...+    ",
        "   ...+     ",
        "            "
    };

    static inline const char *l_bookMarkXpm[] =
    {
        "12 12 2 1",
        " 	c None",
        ".	c #C000C0",
        "            ",
        " .......... ",
        " .        . ",
        " . ...... . ",
        " . .    . . ",
        " . . .. . . ",
        " . . .  . . ",
        " . . .  . . ",
        " . . .... . ",
        " . .      . ",
        " . ........ ",
        "            "
    };

    static inline const char *l_errorMarkXpm[] =
    {
        "12 12 4 1",
        " 	c None",
        ".	c #FF0000",
        "+	c #FFFFFF",
        "@	c #FF8D06",
        "      .     ",
        "     ...    ",
        "    ..@..   ",
        "   ..@+@..  ",
        "  ..@@+@@.. ",
        " ..@@@+@@@..",
        " .@@@@@@@@@.",
        " .@@@@+@@@@.",
        " .@@@@@@@@@.",
        " ...........",
        "            ",
        "            "
    };

    static inline const char *l_debugMarkXpm[] =
    {
        "12 12 3 1",
        " 	c None",
        ".	c #FF0000",
        "+	c #FF6666",
        "            ",
        "     .+     ",
        "      .+    ",
        "      +.+   ",
        " ........+  ",
        " .........+ ",
        " .........+ ",
        " ........+  ",
        "      +.+   ",
        "      .+    ",
        "     .+     ",
        "            "
    };

#   endif
