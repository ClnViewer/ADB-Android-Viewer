#define STR_(A) # A
#define RGB_DESC_ "R,G,B - RGB color format numeric definition, value 0-255"

LFUN__(
	_FIRST,
	checkTime,
	"[bool] LuaObject:checkTime(string time_start, string time_end)\r\ntime start, time_end - string format:\r\n\"HH:mm:ss\" or \"HH:mm\" or \"HH\"",
	INPUT_STRING_2,
	RETURN_NUMBER
)
LFUN__(
	_NEXT,
	checkPixelsByPos,
	"[bool] LuaObject:checkPixelsByPos(int position, int R, int G, int B)\r\nposition - screen buffer position,\r\n" RGB_DESC_,
	INPUT_NUMBER_4,
	RETURN_BOOL
)
LFUN__(
	_NEXT,
	checkPixelsByCord,
	"[bool] LuaObject:checkPixelsByCord(int width, int heigth, int R, int G, int B)\r\nwidth, heigth - Android screen x,y position,\r\n" RGB_DESC_,
	INPUT_NUMBER_5,
	RETURN_BOOL
)
LFUN__(
	_NEXT,
	adbClick,
	"[void] LuaObject:adbClick(int x, int y)\r\nclick to Android screen x, y position",
	INPUT_NUMBER_2,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	adbSwipe,
	"[void] LuaObject:adbSwipe(int start_x, int start_y, int end_x, int end_y, int duration)\r\nswipe Android screen from start_x, start_y position - to end_x, end_y position,\r\nduration - is milliseconds value",
	INPUT_NUMBER_5,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	adbKey,
	"[void] LuaObject:adbKey(int key)\r\nkey - ANDROID key type",
	INPUT_NUMBER,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	adbText,
	"[void] LuaObject:adbText(string text)\r\ntext - string only ASCII character in utf8 format",
	INPUT_STRING,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	stateSet,
	"[void] LuaObject:stateSet(int state)\r\nstate - cycle iteration number set",
	INPUT_NUMBER,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	stateGet,
	"(int) LuaObject:stateGet()\r\nreturn state - curent cycle iteration",
	INPUT_NONE,
	RETURN_NUMBER
)
LFUN__(
	_NEXT,
	stateSleep,
	"[void] LuaObject:stateSleep(int second)\r\nsecond - time sleep in seconds",
	INPUT_NUMBER,
	RETURN_NONE
)
LFUN__(
	_NEXT,
	screenGetCord,
	"[{ int x, int y }] LuaObject:screenGetByCord(int position)\r\nreturn x,y coordinates from given position",
	INPUT_NUMBER,
	RETURN_NUMBER_2
)
LFUN__(
	_NEXT,
	screenGetFb,
	"[table {}] LuaObject:screenGetFb()\r\nreturn full frame buffer data",
	INPUT_NONE,
	RETURN_OBJECT
)
LFUN__(
	_END,
	screenGet,
	"[{ int x, int y }, table {}] LuaObject:screenGet()\r\nreturn x,y screen resolution and full frame buffer data",
	INPUT_NONE,
	RETURN_NUMBER_2_OBJECT
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
