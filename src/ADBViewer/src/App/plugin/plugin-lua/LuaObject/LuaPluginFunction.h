#define STR_(A) # A
#define RGB_DESC_ "R,G,B - RGB color format numeric definition, value 0-255"

LFUN__(
    _FIRST,
    checkTime,
    "[bool] LuaObject:checkTime(string time_start, string time_end)\r\n" \
    "time start, time_end - string format:\r\n\"HH:mm:ss\" or \"HH:mm\" or \"HH\"\r\n" \
    "return: -1 is error, 0 is not time period and 1 is period active",
    INPUT_STRING_2,
    RETURN_NUMBER,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    checkPixelsByPos,
    "[bool] LuaObject:checkPixelsByPos(int position, int R, int G, int B)\r\n" \
    "[bool] LuaObject:checkPixelsByPos(int position, { int R, int G, int B })\r\n" \
    "position - screen buffer position,\r\n" RGB_DESC_,
    INPUT_NUMBER_4_OR_NUMBER_1_TABLE,
    RETURN_BOOL,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    checkPixelsByCord,
    "[bool] LuaObject:checkPixelsByCord(int width, int heigth, int R, int G, int B)\r\n" \
    "[bool] LuaObject:checkPixelsByCord(int width, int heigth, { int R, int G, int B })\r\n" \
    "width, heigth - Android screen x,y position,\r\n" RGB_DESC_,
    INPUT_NUMBER_5_OR_NUMBER_2_TABLE,
    RETURN_BOOL,
    LFEXT_BYNAME
)

LFUN__(
    _NEXT,
    imageDefault,
    "[void] LuaObject:imageDefault(string filename)\r\n" \
    "set default bmp, png image as Android frame buffer\r\n" \
    "! development only",
    INPUT_STRING,
    RETURN_STRING,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    imageShow,
    "[void] LuaObject:imageShow(string filename)\r\n" \
    "show user defined bmp, png image\r\n" \
    "! development only",
    INPUT_STRING,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    imageTableShow,
    "[void] LuaObject:imageTableShow({ Lua image object table })\r\n" \
    "show LuaImage object table, see example saved from internal viewer\r\n" \
    "! development only",
    INPUT_TABLE,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    imageCompare,
    "[double] LuaObject:imageCompare(string filename)\r\n" \
    "compare default screen to user defined bmp, png image",
    INPUT_STRING,
    RETURN_DOUBLE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    imageCompareRegion,
    "[double] LuaObject:imageCompareRegion(string filename, int x, int y, int width, int heigth)\r\n" \
    "compare default screen to user defined bmp, png image, witch region rectangle",
    INPUT_STRING_1_NUMBER_4,
    RETURN_DOUBLE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    imageGetPosition,
    "[int] LuaObject:imageGetPosition(int x, int y)\r\n" \
    "get default screen buffer position from x, y",
    INPUT_NUMBER_2,
    RETURN_NUMBER,
    LFEXT_BYNAME
)

LFUN__(
    _NEXT,
    adbClick,
    "[void] LuaObject:adbClick(int x, int y)\r\n" \
    "click to Android screen x, y position",
    INPUT_NUMBER_2,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    adbSwipe,
    "[void] LuaObject:adbSwipe(int start_x, int start_y, int end_x, int end_y, int duration)\r\n" \
    "swipe Android screen from start_x, start_y position - to end_x, end_y position,\r\n" \
    "duration - is milliseconds value",
    INPUT_NUMBER_5,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    adbKey,
    "[void] LuaObject:adbKey(int key)\r\n" \
    "key - ANDROID key type",
    INPUT_NUMBER,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    adbText,
    "[void] LuaObject:adbText(string text)\r\n" \
    "text - string only ASCII character in utf8 format",
    INPUT_STRING,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    stateSet,
    "[void] LuaObject:stateSet(int state)\r\n" \
    "state - cycle iteration number set",
    INPUT_NUMBER,
    RETURN_NONE,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    stateGet,
    "[int] LuaObject:stateGet()\r\n" \
    "return state - curent cycle iteration",
    INPUT_NONE,
    RETURN_NUMBER,
    LFEXT_BYNAME
)
LFUN__(
    _NEXT,
    stateSleep,
    "[void] LuaObject:stateSleep(int second)\r\n" \
    "second - time sleep in seconds",
    INPUT_NUMBER,
    RETURN_NONE,
    LFEXT_BYNAME
)

LFUN__(
    _NEXT,
    screenGetCord,
    "[{table}] LuaObject:screenGetRegion(int x, int y, int w, int h)\r\n" \
    "[{table}] LuaObject:screenGetRegion({int x, int y, int w, int h})\r\n" \
    "return region from frame buffer data, LuaImage type",
    INPUT_NUMBER_4_OR_NUMBER_1_TABLE,
    RETURN_TABLE,
    LFEXT_NONE
)
LFUN__(
    _NEXT,
    screenGet,
    "[{table}] LuaObject:screenGet()\r\n" \
    "return full frame buffer data, LuaImage type",
    INPUT_NONE,
    RETURN_TABLE,
    LFEXT_NONE
)

#undef STR_
#undef RGB_DESC_
#if defined(LFUN__)
#   undef LFUN__
#endif
#if defined(LFUN_FIRST)
#   undef LFUN_FIRST
#endif
#if defined(LFUN_NEXT)
#   undef LFUN_NEXT
#endif
#if defined(LFUN_END)
#   undef LFUN_END
#endif
