HLP_LABEL("LUA FUNCTION:")
HLP_ITEM(
	"assert",
	"void assert(Bool:v, String:[message])",
	"Issues an error when the value of its argument v is false (i.e., nil or false);\n" \
	"otherwise, returns all its arguments. message is an error message; when absent, it \n" \
	"defaults to 'assertion failed!'"
)
HLP_ITEM(
	"collectgarbage",
	"void collectgarbage(String:opt, [arg])",
	"This function is a generic interface to the garbage collector.\n" \
	"It performs different functions according to its first argument, opt:\n" \
	"    * 'stop': stops the garbage collector.\n" \
	"    * 'restart': restarts the garbage collector.\n" \
	"    * 'collect': performs a full garbage-collection cycle.\n" \
	"    * 'count': returns the total memory in use by Lua (in Kbytes).\n" \
	"    * 'step': performs a garbage-collection step. The step 'size' is controlled by arg\n" \
	"        (larger values mean more steps) in a non-specified way. If you want to control\n" \
	"        the step size you must experimentally tune the value of arg. Returns true if\n" \
	"        the step finished a collection cycle.\n" \
	"    * 'setpause': sets arg as the new value for the pause of the collector (see §2.10).\n" \
	"        Returns the previous value for pause.\n" \
	"    * 'setstepmul': sets arg as the new value for the step multiplier of the collector\n" \
	"        (see §2.10). Returns the previous value for step."
)
HLP_ITEM(
	"coroutine.create",
	"void coroutine.create(Function:f)",
	"Creates a new coroutine, with body f. f must be a Lua function. Returns this new\n" \
	"coroutine, an object with type 'thread'."
)
HLP_ITEM(
	"coroutine.resume",
	"void coroutine.resume(co, [, val, ...])",
	"Starts or continues the execution of coroutine co. The first time you resume a\n" \
	"coroutine, it starts running its body. The values val1, ··· are passed as the \n" \
	"arguments to the body function. If the coroutine has yielded, resume restarts it; the \n" \
	"values val1, ··· are passed as the results from the yield.\n" \
	"\n" \
	"If the coroutine runs without any errors, resume returns true plus any values passed \n" \
	"to yield (if the coroutine yields) or any values returned by the body function (if the\n" \
	"coroutine terminates). If there is any error, resume returns false plus the error \n" \
	"message."
)
HLP_ITEM(
	"coroutine.running",
	"void coroutine.running()",
	"Returns the running coroutine, or nil when called by the main thread. "
)
HLP_ITEM(
	"coroutine.status",
	"void coroutine.status(co)",
	"Returns the status of coroutine co, as a string: 'running', if the coroutine is running (that is, it called status); \n" \
	"'suspended', if the coroutine is suspended in a call to yield, or if it has not started running yet; \n" \
	"'normal' if the coroutine is active but not running (that is, it has resumed another coroutine); and \n" \
	"'dead' if the coroutine has finished its body function, or if it has stopped with an error. "
)
HLP_ITEM(
	"coroutine.wrap",
	"void coroutine.wrap(f)",
	"Creates a new coroutine, with body f. f must be a Lua function. Returns a function that resumes \n" \
	"the coroutine each time it is called. Any arguments passed to the function behave as the extra \n" \
	"arguments to resume. Returns the same values returned by resume, except the first boolean. In \n" \
	"case of error, propagates the error. "
)
HLP_ITEM(
	"coroutine.yield",
	"void coroutine.yield(...)",
	"Suspends the execution of the calling coroutine. The coroutine cannot be running a C function, a \n" \
	"metamethod, or an iterator. Any arguments to yield are passed as extra results to resume. "
)
HLP_ITEM(
	"debug.debug",
	"void debug.debug()",
	"Enters an interactive mode with the user, running each string that the user enters. Using \n" \
	"simple commands and other debug facilities, the user can inspect global and local variables, \n" \
	"change their values, evaluate expressions, and so on. A line containing only the word cont \n" \
	"finishes this function, so that the caller continues its execution.\n" \
	"\n" \
	"Note that commands for debug.debug are not lexically nested within any function, and so \n" \
	"have no direct access to local variables. "
)
HLP_ITEM(
	"debug.getfenv",
	"void debug.getfenv(o)",
	"Returns the environment of object o. "
)
HLP_ITEM(
	"debug.gethook",
	"void debug.gethook([thread])",
	"Returns the current hook settings of the thread, as three values: the current hook function, the \n" \
	"current hook mask, and the current hook count (as set by the debug.sethook function). "
)
HLP_ITEM(
	"debug.getinfo",
	"void debug.getinfo([thread,], function, [, what])",
	"Returns a table with information about a function. You can give the function directly, or you \n" \
	"can give a number as the value of function, which means the function running at level function \n" \
	"of the call stack of the given thread: level 0 is the current function (getinfo itself); level 1 \n" \
	"is the function that called getinfo; and so on. If function is a number larger than the number \n" \
	"of active functions, then getinfo returns nil.\n" \
	"\n" \
	"The returned table can contain all the fields returned by lua_getinfo, with the string what \n" \
	"describing which fields to fill in. The default for what is to get all information available, \n" \
	"except the table of valid lines. If present, the option 'f' adds a field named func with the \n" \
	"function itself. If present, the option 'L' adds a field named activelines with the table of \n" \
	"valid lines.\n" \
	"\n" \
	"For instance, the expression debug.getinfo(1,'n').name returns a table with a name for the current \n" \
	"function, if a reasonable name can be found, and the expression debug.getinfo(print) returns a table \n" \
	"with all available information about the print function. "
)
HLP_ITEM(
	"debug.getlocal",
	"void debug.getlocal([thread,], level, local)",
	"This function returns the name and the value of the local variable with index local of the function \n" \
	"at level level of the stack. (The first parameter or local variable has index 1, and so on, until the \n" \
	"last active local variable.) The function returns nil if there is no local variable with the given index, \n" \
	"and raises an error when called with a level out of range. (You can call debug.getinfo to check whether \n" \
	"the level is valid.)\n" \
	"\n" \
	"Variable names starting with '(' (open parentheses) represent internal variables (loop control variables, \n" \
	"temporaries, and C function locals). "
)
HLP_ITEM(
	"debug.getmetatable",
	"void debug.getmetatable(object)",
	"Returns the metatable of the given object or nil if it does not have a metatable. "
)
HLP_ITEM(
	"debug.getregistry",
	"void debug.getregistry()",
	"Returns the registry table (see §3.5). "
)
HLP_ITEM(
	"debug.getupvalue",
	"void debug.getupvalue(func, up)",
	"This function returns the name and the value of the upvalue with index up of the function \n" \
	"func. The function returns nil if there is no upvalue with the given index. "
)
HLP_ITEM(
	"debug.setfenv",
	"void debug.setfenv(object, table)",
	"Sets the environment of the given object to the given table. Returns object. "
)
HLP_ITEM(
	"debug.sethook",
	"void debug.sethook([thread,], hook, mask, [, count])",
	"Sets the given function as a hook. The string mask and the number count describe when the hook \n" \
	"will be called. The string mask may have the following characters, with the given meaning:\n" \
	"\n" \
	"    * 'c': the hook is called every time Lua calls a function;\n" \
	"    * 'r': the hook is called every time Lua returns from a function;\n" \
	"    * 'l': the hook is called every time Lua enters a new line of code.\n" \
	"\n" \
	"With a count different from zero, the hook is called after every count instructions.\n" \
	"\n" \
	"When called without arguments, debug.sethook turns off the hook.\n" \
	"\n" \
	"When the hook is called, its first parameter is a string describing the event that has triggered \n" \
	"its call: 'call', 'return' (or 'tail return', when simulating a return from a tail call), 'line', \n" \
	"and 'count'. For line events, the hook also gets the new line number as its second parameter. \n" \
	"Inside a hook, you can call getinfo with level 2 to get more information about the running function \n" \
	"(level 0 is the getinfo function, and level 1 is the hook function), unless the event is 'tail return'. \n" \
	"In this case, Lua is only simulating the return, and a call to getinfo will return invalid data. "
)
HLP_ITEM(
	"debug.setlocal",
	"void debug.setlocal([thread,], level, local, value)",
	"This function assigns the value value to the local variable with index local of the function at level \n" \
	"level of the stack. The function returns nil if there is no local variable with the given index, and \n" \
	"raises an error when called with a level out of range. (You can call getinfo to check whether the level \n" \
	"is valid.) Otherwise, it returns the name of the local variable. "
)
HLP_ITEM(
	"debug.setmetatable",
	"void debug.setmetatable(object, table)",
	"Sets the metatable for the given object to the given table (which can be nil). "
)
HLP_ITEM(
	"debug.setupvalue",
	"void debug.setupvalue(func, up, value)",
	"This function assigns the value value to the upvalue with index up of the function func. The function \n" \
	"returns nil if there is no upvalue with the given index. Otherwise, it returns the name of the upvalue. "
)
HLP_ITEM(
	"debug.traceback",
	"void debug.traceback([thread,], [message], [, level])",
	"Returns a string with a traceback of the call stack. An optional message string is appended at the \n" \
	"beginning of the traceback. An optional level number tells at which level to start the traceback \n" \
	"(default is 1, the function calling traceback). "
)
HLP_ITEM(
	"dofile",
	"void dofile(filename)",
	"Opens the named file and executes its contents as a Lua chunk. When called without arguments, dofile \n" \
	"executes the contents of the standard input (stdin). Returns all values returned by the chunk. In case \n" \
	"of errors, dofile propagates the error to its caller (that is, dofile does not run in protected mode). "
)
HLP_ITEM(
	"error",
	"void error(message, [, level])",
	"Terminates the last protected function called and returns message as the error message. \n" \
	"Function error never returns.\n" \
	"\n" \
	"Usually, error adds some information about the error position at the beginning of the message. \n" \
	"The level argument specifies how to get the error position. With level 1 (the default), the error \n" \
	"position is where the error function was called. Level 2 points the error to where the function \n" \
	"that called error was called; and so on. Passing a level 0 avoids the addition of error position \n" \
	"information to the message. "
)
HLP_ITEM(
	"file.close",
	"void file:close()",
	"Closes file. Note that files are automatically closed when their handles are garbage collected, \n" \
	"but that takes an unpredictable amount of time to happen. "
)
HLP_ITEM(
	"file.flush",
	"void file:flush()",
	"Saves any written data to file. "
)
HLP_ITEM(
	"file.lines",
	"void file:lines()",
	"Returns an iterator function that, each time it is called, returns a new line from the file. \n" \
	"Therefore, the construction\n" \
	"for line in file:lines() do body end\n" \
	"will iterate over all lines of the file. (Unlike io.lines, this function does not close the file when the loop ends.) "
)
HLP_ITEM(
	"file.read",
	"void file:read(···)",
	"Reads the file file, according to the given formats, which specify what to read. For each format, \n" \
	"the function returns a string (or a number) with the characters read, or nil if it cannot read data \n" \
	"with the specified format. When called without formats, it uses a default format that reads the entire \n" \
	"next line (see below).\n" \
	"\n" \
	"The available formats are\n" \
	"\n" \
	"    * '*n': reads a number; this is the only format that returns a number instead of a string.\n" \
	"    * '*a': reads the whole file, starting at the current position. On end of file, it returns the empty string.\n" \
	"    * '*l': reads the next line (skipping the end of line), returning nil on end of file. This is the default format.\n" \
	"    * number: reads a string with up to this number of characters, returning nil on end of file. If number is zero, it \n" \
	"            reads nothing and returns an empty string, or nil on end of file.\n" \
	""
)
HLP_ITEM(
	"file.seek",
	"void file:seek([whence], [, offset])",
	"Sets and gets the file position, measured from the beginning of the file, to the position given by offset \n" \
	"plus a base specified by the string whence, as follows:\n" \
	"\n" \
	"    * 'set': base is position 0 (beginning of the file);\n" \
	"    * 'cur': base is current position;\n" \
	"    * 'end': base is end of file;\n" \
	"\n" \
	"In case of success, function seek returns the final file position, measured in bytes from the beginning of the \n" \
	"file. If this function fails, it returns nil, plus a string describing the error.\n" \
	"\n" \
	"The default value for whence is 'cur', and for offset is 0. Therefore, the call file:seek() returns the \n" \
	"current file position, without changing it; the call file:seek('set') sets the position to the beginning \n" \
	"of the file (and returns 0); and the call file:seek('end') sets the position to the end of the file, and \n" \
	"returns its size. "
)
HLP_ITEM(
	"file.setvbuf",
	"void file:setvbuf(mode, [, size])",
	"Sets the buffering mode for an output file. There are three available modes:\n" \
	"\n" \
	"    * 'no': no buffering; the result of any output operation appears immediately.\n" \
	"    * 'full': full buffering; output operation is performed only when the buffer is full \n" \
	"            (or when you explicitly flush the file (see io.flush)).\n" \
	"    * 'line': line buffering; output is buffered until a newline is output or there is any input \n" \
	"            from some special files (such as a terminal device).\n" \
	"\n" \
	"For the last two cases, size specifies the size of the buffer, in bytes. The default is an appropriate size. "
)
HLP_ITEM(
	"file.write",
	"void file:write(···)",
	"Writes the value of each of its arguments to the file. The arguments must be strings or numbers. To write \n" \
	"other values, use tostring or string.format before write. "
)
HLP_ITEM(
	"getfenv",
	"void getfenv([f])",
	"Returns the current environment in use by the function. f can be a Lua function or a number that specifies \n" \
	"the function at that stack level: Level 1 is the function calling getfenv. If the given function is not a \n" \
	"Lua function, or if f is 0, getfenv returns the global environment. The default for f is 1. "
)
HLP_ITEM(
	"getmetatable",
	"void getmetatable(object)",
	"If object does not have a metatable, returns nil. Otherwise, if the object's metatable has a '__metatable' \n" \
	"field, returns the associated value. Otherwise, returns the metatable of the given object. "
)
HLP_ITEM(
	"io.close",
	"void io.close([file])",
	"Equivalent to file:close(). Without a file, closes the default output file. "
)
HLP_ITEM(
	"io.flush",
	"void io.flush()",
	"Equivalent to file:flush over the default output file. "
)
HLP_ITEM(
	"io.input",
	"void io.input([file])",
	"When called with a file name, it opens the named file (in text mode), and sets its handle as the default \n" \
	"input file. When called with a file handle, it simply sets this file handle as the default input file. When \n" \
	"called without parameters, it returns the current default input file.\n" \
	"\n" \
	"In case of errors this function raises the error, instead of returning an error code. "
)
HLP_ITEM(
	"io.lines",
	"void io.lines([filename])",
	"Opens the given file name in read mode and returns an iterator function that, each time it is called, \n" \
	"returns a new line from the file. Therefore, the construction\n" \
	"\n" \
	"     for line in io.lines(filename) do body end\n" \
	"\n" \
	"will iterate over all lines of the file. When the iterator function detects the end of file, it returns \n" \
	"nil (to finish the loop) and automatically closes the file.\n" \
	"\n" \
	"The call io.lines() (with no file name) is equivalent to io.input():lines(); that is, it iterates over \n" \
	"the lines of the default input file. In this case it does not close the file when the loop ends. "
)
HLP_ITEM(
	"io.open",
	"void io.open(filename, [, mode])",
	"This function opens a file, in the mode specified in the string mode. It returns a new file handle, \n" \
	"or, in case of errors, nil plus an error message.\n" \
	"\n" \
	"The mode string can be any of the following:\n" \
	"\n" \
	"    * 'r': read mode (the default);\n" \
	"    * 'w': write mode;\n" \
	"    * 'a': append mode;\n" \
	"    * 'r+': update mode, all previous data is preserved;\n" \
	"    * 'w+': update mode, all previous data is erased;\n" \
	"    * 'a+': append update mode, previous data is preserved, writing is only allowed at the end of file.\n" \
	"\n" \
	"The mode string can also have a 'b' at the end, which is needed in some systems to open the file in \n" \
	"binary mode. This string is exactly what is used in the standard C function fopen. "
)
HLP_ITEM(
	"io.output",
	"void io.output([file])",
	"Similar to io.input, but operates over the default output file. "
)
HLP_ITEM(
	"io.popen",
	"void io.popen(prog, [, mode])",
	"Starts program prog in a separated process and returns a file handle that you can use to read data from \n" \
	"this program (if mode is 'r', the default) or to write data to this program (if mode is 'w').\n" \
	"\n" \
	"This function is system dependent and is not available on all platforms. "
)
HLP_ITEM(
	"io.read",
	"void io.read(···)",
	"Equivalent to io.input():read. "
)
HLP_ITEM(
	"io.tmpfile",
	"void io.tmpfile()",
	"Returns a handle for a temporary file. This file is opened in update mode and it is automatically \n" \
	"removed when the program ends. "
)
HLP_ITEM(
	"io.type",
	"void io.type(obj)",
	"Checks whether obj is a valid file handle. Returns the string 'file' if obj is an open file handle, \n" \
	"'closed file' if obj is a closed file handle, or nil if obj is not a file handle. "
)
HLP_ITEM(
	"io.write",
	"void io.write(···)",
	"Equivalent to io.output():write. "
)
HLP_ITEM(
	"ipairs",
	"void ipairs(t)",
	"Returns three values: an iterator function, the table t, and 0, so that the construction\n" \
	"\n" \
	"     for i,v in ipairs(t) do body end\n" \
	"\n" \
	"will iterate over the pairs (1,t[1]), (2,t[2]), ···, up to the first integer key absent from the table. "
)
HLP_ITEM(
	"load",
	"void load(func, [, chunkname])",
	"Loads a chunk using function func to get its pieces. Each call to func must return a string that \n" \
	"concatenates with previous results. A return of an empty string, nil, or no value signals the end of \n" \
	"the chunk.\n" \
	"\n" \
	"If there are no errors, returns the compiled chunk as a function; otherwise, returns nil plus the error \n" \
	"message. The environment of the returned function is the global environment.\n" \
	"\n" \
	"chunkname is used as the chunk name for error messages and debug information. When absent, \n" \
	"it defaults to '=(load)'. "
)
HLP_ITEM(
	"loadfile",
	"void loadfile([filename])",
	"Similar to load, but gets the chunk from file filename or from the standard input, \n" \
	"if no file name is given. "
)
HLP_ITEM(
	"loadstring",
	"void loadstring(string, [, chunkname])",
	"Similar to load, but gets the chunk from the given string.\n" \
	"To load and run a given string, use the idiom\n" \
	"     assert(loadstring(s))()\n" \
	"When absent, chunkname defaults to the given string. "
)
HLP_ITEM(
	"math.abs",
	"void math.abs(x)",
	"Returns the absolute value of x. "
)
HLP_ITEM(
	"math.acos",
	"void math.acos(x)",
	"Returns the arc cosine of x (in radians). "
)
HLP_ITEM(
	"math.asin",
	"void math.asin(x)",
	"Returns the arc sine of x (in radians). "
)
HLP_ITEM(
	"math.atan",
	"void math.atan(x)",
	"Returns the arc tangent of x (in radians). "
)
HLP_ITEM(
	"math.atan2",
	"void math.atan2(y, x)",
	"Returns the arc tangent of y/x (in radians), but uses the signs of both parameters to \n" \
	"find the quadrant of the result. (It also handles correctly the case of x being zero.) "
)
HLP_ITEM(
	"math.ceil",
	"void math.ceil(x)",
	"Returns the smallest integer larger than or equal to x. "
)
HLP_ITEM(
	"math.cos",
	"void math.cos(x)",
	"Returns the cosine of x (assumed to be in radians). "
)
HLP_ITEM(
	"math.cosh",
	"void math.cosh(x)",
	"Returns the hyperbolic cosine of x. "
)
HLP_ITEM(
	"math.deg",
	"void math.deg(x)",
	"Returns the angle x (given in radians) in degrees. "
)
HLP_ITEM(
	"math.exp",
	"void math.exp(x)",
	"Returns the value ex. "
)
HLP_ITEM(
	"math.floor",
	"void math.floor(x)",
	"Returns the largest integer smaller than or equal to x. "
)
HLP_ITEM(
	"math.fmod",
	"void math.fmod(x, y)",
	"Returns the remainder of the division of x by y that rounds the quotient towards zero. "
)
HLP_ITEM(
	"math.frexp",
	"void math.frexp(x)",
	"Returns m and e such that x = m2e, e is an integer and the absolute value of m is in \n" \
	"the range [0.5, 1) (or zero when x is zero). "
)
HLP_ITEM(
	"math.ldexp",
	"void math.ldexp(m, e)",
	"Returns m2e (e should be an integer). "
)
HLP_ITEM(
	"math.log",
	"void math.log(x)",
	"Returns the natural logarithm of x. "
)
HLP_ITEM(
	"math.log10",
	"void math.log10(x)",
	"Returns the base-10 logarithm of x. "
)
HLP_ITEM(
	"math.max",
	"void math.max(x, ...)",
	"Returns the maximum value among its arguments. "
)
HLP_ITEM(
	"math.min",
	"void math.min(x, ...)",
	"Returns the minimum value among its arguments. "
)
HLP_ITEM(
	"math.modf",
	"void math.modf(x)",
	"Returns two numbers, the integral part of x and the fractional part of x. "
)
HLP_ITEM(
	"math.pow",
	"void math.pow(x, y)",
	"Returns xy. (You can also use the expression x^y to compute this value.) "
)
HLP_ITEM(
	"math.rad",
	"void math.rad(x)",
	"Returns the angle x (given in degrees) in radians. "
)
HLP_ITEM(
	"math.random",
	"void math.random([m [, n]])",
	"This function is an interface to the simple pseudo-random generator function rand \n" \
	"provided by ANSI C. (No guarantees can be given for its statistical properties.)\n" \
	"\n" \
	"When called without arguments, returns a uniform pseudo-random real number in the \n" \
	"range [0,1). When called with an integer number m, math.random returns a uniform \n" \
	"pseudo-random integer in the range [1, m]. When called with two integer numbers m and \n" \
	"n, math.random returns a uniform pseudo-random integer in the range [m, n]. "
)
HLP_ITEM(
	"math.randomseed",
	"void math.randomseed(x)",
	"Sets x as the 'seed' for the pseudo-random generator: equal seeds produce equal \n" \
	"sequences of numbers. "
)
HLP_ITEM(
	"math.sin",
	"void math.sin(x)",
	"Returns the sine of x (assumed to be in radians). "
)
HLP_ITEM(
	"math.sinh",
	"void math.sinh(x)",
	"Returns the hyperbolic sine of x. "
)
HLP_ITEM(
	"math.sqrt",
	"void math.sqrt(x)",
	"Returns the square root of x. (You can also use the expression x^0.5 to compute this value.) "
)
HLP_ITEM(
	"math.tan",
	"void math.tan(x)",
	"Returns the tangent of x (assumed to be in radians). "
)
HLP_ITEM(
	"math.tanh",
	"void math.tanh(x)",
	"Returns the hyperbolic tangent of x. "
)
HLP_ITEM(
	"module",
	"void module(name, [, ···])",
	"Creates a module. If there is a table in package.loaded[name], this table is the module. \n" \
	"Otherwise, if there is a global table t with the given name, this table is the module. \n" \
	"Otherwise creates a new table t and sets it as the value of the global name and the value \n" \
	"of package.loaded[name]. This function also initializes t._NAME with the given name, t._M \n" \
	"with the module (t itself), and t._PACKAGE with the package name (the full module name \n" \
	"minus last component; see below). Finally, module sets t as the new environment of the \n" \
	"current function and the new value of package.loaded[name], so that require returns t.\n" \
	"\n" \
	"If name is a compound name (that is, one with components separated by dots), module creates \n" \
	"(or reuses, if they already exist) tables for each component. For instance, if name is a.b.c, \n" \
	"then module stores the module table in field c of field b of global a.\n" \
	"\n" \
	"This function can receive optional options after the module name, where each option is a \n" \
	"function to be applied over the module. "
)
HLP_ITEM(
	"next",
	"void next(table, [, index])",
	"Allows a program to traverse all fields of a table. Its first argument is a table and its second \n" \
	"argument is an index in this table. next returns the next index of the table and its associated \n" \
	"value. When called with nil as its second argument, next returns an initial index and its associated \n" \
	"value. When called with the last index, or with nil in an empty table, next returns nil. If the \n" \
	"second argument is absent, then it is interpreted as nil. In particular, you can use next(t) to \n" \
	"check whether a table is empty.\n" \
	"\n" \
	"The order in which the indices are enumerated is not specified, even for numeric indices. (To \n" \
	"traverse a table in numeric order, use a numerical for or the ipairs function.)\n" \
	"\n" \
	"The behavior of next is undefined if, during the traversal, you assign any value to a non-existent \n" \
	"field in the table. You may however modify existing fields. In particular, you may clear existing \n" \
	"fields. "
)
HLP_ITEM(
	"os.clock",
	"void os.clock()",
	"Returns an approximation of the amount in seconds of CPU time used by the program. "
)
HLP_ITEM(
	"os.date",
	"void os.date([format [, time]])",
	"Returns a string or a table containing date and time, formatted according to the given string format.\n" \
	"\n" \
	"If the time argument is present, this is the time to be formatted (see the os.time function \n" \
	"for a description of this value). Otherwise, date formats the current time.\n" \
	"\n" \
	"If format starts with '!', then the date is formatted in Coordinated Universal Time. After this \n" \
	"optional character, if format is the string '*t', then date returns a table with the following fields: \n" \
	"year (four digits), \n" \
	"month (1--12), \n" \
	"day (1--31), \n" \
	"hour (0--23), \n" \
	"min (0--59), \n" \
	"sec (0--61), \n" \
	"wday (weekday, Sunday is 1), \n" \
	"yday (day of the year), and \n" \
	"isdst (daylight saving flag, a boolean).\n" \
	"\n" \
	"If format is not '*t', then date returns the date as a string, formatted according to the same \n" \
	"rules as the C function strftime.\n" \
	"\n" \
	"When called without arguments, date returns a reasonable date and time representation that \n" \
	"depends on the host system and on the current locale (that is, os.date() is equivalent to os.date('%c')). "
)
HLP_ITEM(
	"os.difftime",
	"void os.difftime(t2, t1)",
	"Returns the number of seconds from time t1 to time t2. In POSIX, Windows, and some other systems, \n" \
	"this value is exactly t2-t1. "
)
HLP_ITEM(
	"os.execute",
	"void os.execute([command])",
	"This function is equivalent to the C function system. It passes command to be executed by an operating \n" \
	"system shell. It returns a status code, which is system-dependent. If command is absent, then it returns \n" \
	"nonzero if a shell is available and zero otherwise. "
)
HLP_ITEM(
	"os.exit",
	"void os.exit([code])",
	"Calls the C function exit, with an optional code, to terminate the host program. The default \n" \
	"value for code is the success code. "
)
HLP_ITEM(
	"os.getenv",
	"void os.getenv(varname)",
	"Returns the value of the process environment variable varname, or nil if the variable is not defined. "
)
HLP_ITEM(
	"os.remove",
	"void os.remove(filename)",
	"Deletes the file or directory with the given name. Directories must be empty to be removed. If this \n" \
	"function fails, it returns nil, plus a string describing the error. "
)
HLP_ITEM(
	"os.rename",
	"void os.rename(oldname, newname)",
	"Renames file or directory named oldname to newname. If this function fails, it returns nil, plus a \n" \
	"string describing the error. "
)
HLP_ITEM(
	"os.setlocale",
	"void os.setlocale(locale, [, category])",
	"Sets the current locale of the program. locale is a string specifying a locale; category is an optional \n" \
	"string describing which category to change: 'all', 'collate', 'ctype', 'monetary', 'numeric', or 'time'; \n" \
	"the default category is 'all'. The function returns the name of the new locale, or nil if the request \n" \
	"cannot be honored.\n" \
	"\n" \
	"If locale is the empty string, the current locale is set to an implementation-defined native locale. If \n" \
	"locale is the string 'C', the current locale is set to the standard C locale.\n" \
	"\n" \
	"When called with nil as the first argument, this function only returns the name of the current locale \n" \
	"for the given category. "
)
HLP_ITEM(
	"os.time",
	"void os.time([table])",
	"Returns the current time when called without arguments, or a time representing the date and time specified \n" \
	"by the given table. This table must have fields year, month, and day, and may have fields hour, min, sec, \n" \
	"and isdst (for a description of these fields, see the os.date function).\n" \
	"\n" \
	"The returned value is a number, whose meaning depends on your system. In POSIX, Windows, and some other \n" \
	"systems, this number counts the number of seconds since some given start time (the 'epoch'). \n" \
	"In other systems, the meaning is not specified, and the number returned by time can be used only as an \n" \
	"argument to date and difftime. "
)
HLP_ITEM(
	"os.tmpname",
	"void os.tmpname()",
	"Returns a string with a file name that can be used for a temporary file. The file must be explicitly \n" \
	"opened before its use and explicitly removed when no longer needed.\n" \
	"\n" \
	"On some systems (POSIX), this function also creates a file with that name, to avoid security risks. \n" \
	"(Someone else might create the file with wrong permissions in the time between getting the name and \n" \
	"creating the file.) You still have to open the file to use it and to remove it (even if you do not use it).\n" \
	"\n" \
	"When possible, you may prefer to use io.tmpfile, which automatically removes the file when the program ends. "
)
HLP_ITEM(
	"package.loadlib",
	"void package.loadlib(libname, funcname)",
	"Dynamically links the host program with the C library libname. Inside this library, looks for a function \n" \
	"funcname and returns this function as a C function. (So, funcname must follow the protocol (see lua_CFunction)).\n" \
	"\n" \
	"This is a low-level function. It completely bypasses the package and module system. Unlike require, it \n" \
	"does not perform any path searching and does not automatically adds extensions. libname must be the complete \n" \
	"file name of the C library, including if necessary a path and extension. funcname must be the exact name \n" \
	"exported by the C library (which may depend on the C compiler and linker used).\n" \
	"\n" \
	"This function is not supported by ANSI C. As such, it is only available on some platforms (Windows, \n" \
	"Linux, Mac OS X, Solaris, BSD, plus other Unix systems that support the dlfcn standard). "
)
HLP_ITEM(
	"package.seeall",
	"void package.seeall(module)",
	"Sets a metatable for module with its __index field referring to the global environment, so that this \n" \
	"module inherits values from the global environment. To be used as an option to function module. "
)
HLP_ITEM(
	"pairs",
	"void pairs(t)",
	"Returns three values: the next function, the table t, and nil, so that the construction\n" \
	"\n" \
	"     for k,v in pairs(t) do body end\n" \
	"\n" \
	"will iterate over all key–value pairs of table t.\n" \
	"\n" \
	"See function next for the caveats of modifying the table during its traversal. "
)
HLP_ITEM(
	"pcall",
	"void pcall(f, arg1, ···)",
	"Calls function f with the given arguments in protected mode. This means that any error \n" \
	"inside f is not propagated; instead, pcall catches the error and returns a status code. \n" \
	"Its first result is the status code (a boolean), which is true if the call succeeds without \n" \
	"errors. In such case, pcall also returns all results from the call, after this first result. \n" \
	"In case of any error, pcall returns false plus the error message. "
)
HLP_ITEM(
	"print",
	"void print(···)",
	"Receives any number of arguments, and prints their values to stdout, using the tostring \n" \
	"function to convert them to strings. print is not intended for formatted output, but only \n" \
	"as a quick way to show a value, typically for debugging. For formatted output, use string.format. "
)
HLP_ITEM(
	"rawequal",
	"void rawequal(v1, v2)",
	"Checks whether v1 is equal to v2, without invoking any metamethod. Returns a boolean. "
)
HLP_ITEM(
	"rawget",
	"void rawget(table, index)",
	"Gets the real value of table[index], without invoking any metamethod. table must be a \n" \
	"table; index may be any value. "
)
HLP_ITEM(
	"rawset",
	"void rawset(table, index, value)",
	"Sets the real value of table[index] to value, without invoking any metamethod. table must \n" \
	"be a table, index any value different from nil, and value any Lua value.\n" \
	"\n" \
	"This function returns table. "
)
HLP_ITEM(
	"require",
	"void require(modname)",
	"Loads the given module. The function starts by looking into the package.loaded table to \n" \
	"determine whether modname is already loaded. If it is, then require returns the value stored \n" \
	"at package.loaded[modname]. Otherwise, it tries to find a loader for the module.\n" \
	"\n" \
	"To find a loader, require is guided by the package.loaders array. By changing this array, \n" \
	"we can change how require looks for a module. The following explanation is based on the \n" \
	"default configuration for package.loaders.\n" \
	"\n" \
	"First require queries package.preload[modname]. If it has a value, this value (which should \n" \
	"be a function) is the loader. Otherwise require searches for a Lua loader using the path \n" \
	"stored in package.path. If that also fails, it searches for a C loader using the path stored \n" \
	"in package.cpath. If that also fails, it tries an all-in-one loader (see package.loaders).\n" \
	"\n" \
	"Once a loader is found, require calls the loader with a single argument, modname. If the \n" \
	"loader returns any value, require assigns the returned value to package.loaded[modname]. \n" \
	"If the loader returns no value and has not assigned any value to package.loaded[modname], \n" \
	"then require assigns true to this entry. In any case, require returns the final value of \n" \
	"package.loaded[modname].\n" \
	"\n" \
	"If there is any error loading or running the module, or if it cannot find any loader for \n" \
	"the module, then require signals an error. "
)
HLP_ITEM(
	"select",
	"void select(index, ···)",
	"If index is a number, returns all arguments after argument number index. Otherwise, index \n" \
	"must be the string '#', and select returns the total number of extra arguments it received. "
)
HLP_ITEM(
	"setfenv",
	"void setfenv(f, table)",
	"Sets the environment to be used by the given function. f can be a Lua function or a number \n" \
	"that specifies the function at that stack level: Level 1 is the function calling setfenv. \n" \
	"setfenv returns the given function.\n" \
	"\n" \
	"As a special case, when f is 0 setfenv changes the environment of the running thread. \n" \
	"In this case, setfenv returns no values. "
)
HLP_ITEM(
	"setmetatable",
	"void setmetatable(table, metatable)",
	"Sets the metatable for the given table. (You cannot change the metatable of other types \n" \
	"from Lua, only from C.) If metatable is nil, removes the metatable of the given table. \n" \
	"If the original metatable has a '__metatable' field, raises an error.\n" \
	"\n" \
	"This function returns table. "
)
HLP_ITEM(
	"string.byte",
	"void string.byte(s [, i [, j]])",
	"Returns the internal numerical codes of the characters s[i], s[i+1], ···, s[j]. The default \n" \
	"value for i is 1; the default value for j is i.\n" \
	"\n" \
	"Note that numerical codes are not necessarily portable across platforms. "
)
HLP_ITEM(
	"string.char",
	"void string.char(···)",
	"Receives zero or more integers. Returns a string with length equal to the number of arguments, \n" \
	"in which each character has the internal numerical code equal to its corresponding argument.\n" \
	"\n" \
	"Note that numerical codes are not necessarily portable across platforms. "
)
HLP_ITEM(
	"string.dump",
	"void string.dump(function)",
	"Returns a string containing a binary representation of the given function, so that a later \n" \
	"loadstring on this string returns a copy of the function. function must be a Lua function \n" \
	"without upvalues. "
)
HLP_ITEM(
	"string.find",
	"void string.find(s, pattern, [, init [, plain]])",
	"Looks for the first match of pattern in the string s. If it finds a match, then find returns \n" \
	"the indices of s where this occurrence starts and ends; otherwise, it returns nil. A third, \n" \
	"optional numerical argument init specifies where to start the search; its default value is 1 \n" \
	"and can be negative. A value of true as a fourth, optional argument plain turns off the \n" \
	"pattern matching facilities, so the function does a plain 'find substring' operation, with \n" \
	"no characters in pattern being considered 'magic'. Note that if plain is given, then init \n" \
	"must be given as well.\n" \
	"\n" \
	"If the pattern has captures, then in a successful match the captured values are also returned, \n" \
	"after the two indices. "
)
HLP_ITEM(
	"string.format",
	"void string.format(formatstring, ···)",
	"Returns a formatted version of its variable number of arguments following the description \n" \
	"given in its first argument (which must be a string). The format string follows the same \n" \
	"rules as the printf family of standard C functions. The only differences are that the options/modifiers \n" \
	"*, l, L, n, p, and h are not supported and that there is an extra option, q. The q option formats \n" \
	"a string in a form suitable to be safely read back by the Lua interpreter: the string is \n" \
	"written between double quotes, and all double quotes, newlines, embedded zeros, and backslashes \n" \
	"in the string are correctly escaped when written. For instance, the call\n" \
	"\n" \
	"     string.format('%q', 'a string with ''quotes'' and \n new line')\n" \
	"\n" \
	"will produce the string:\n" \
	"\n" \
	"     'a string with \''quotes\'' and \\n" \
	"      new line'\n" \
	"\n" \
	"The options c, d, E, e, f, g, G, i, o, u, X, and x all expect a number as argument, whereas \n" \
	"q and s expect a string.\n" \
	"\n" \
	"This function does not accept string values containing embedded zeros, except as arguments \n" \
	"to the q option. "
)
HLP_ITEM(
	"string.gmatch",
	"void string.gmatch(s, pattern)",
	"Returns an iterator function that, each time it is called, returns the next captures from \n" \
	"pattern over string s. If pattern specifies no captures, then the whole match is produced \n" \
	"in each call.\n" \
	"\n" \
	"As an example, the following loop\n" \
	"\n" \
	"     s = 'hello world from Lua'\n" \
	"     for w in string.gmatch(s, '%a+') do\n" \
	"       print(w)\n" \
	"     end\n" \
	"\n" \
	"will iterate over all the words from string s, printing one per line. The next example collects \n" \
	"all pairs key=value from the given string into a table:\n" \
	"\n" \
	"     t = {}\n" \
	"     s = 'from=world, to=Lua'\n" \
	"     for k, v in string.gmatch(s, '(%w+)=(%w+)') do\n" \
	"       t[k] = v\n" \
	"     end\n" \
	"\n" \
	"For this function, a '^' at the start of a pattern does not work as an anchor, as this would \n" \
	"prevent the iteration. "
)
HLP_ITEM(
	"string.gsub",
	"void string.gsub(s, pattern, repl, [, n])",
	"Returns a copy of s in which all (or the first n, if given) occurrences of the pattern have \n" \
	"been replaced by a replacement string specified by repl, which can be a string, a table, or \n" \
	"a function. gsub also returns, as its second value, the total number of matches that occurred.\n" \
	"\n" \
	"Look at the online documentation for this function."
)
HLP_ITEM(
	"string.len",
	"void string.len(s)",
	"Receives a string and returns its length. The empty string '' has length 0. Embedded zeros are \n" \
	"counted, so 'a\000bc\000' has length 5. "
)
HLP_ITEM(
	"string.lower",
	"void string.lower(s)",
	"Receives a string and returns a copy of this string with all uppercase letters changed to \n" \
	"lowercase. All other characters are left unchanged. The definition of what an uppercase \n" \
	"letter is depends on the current locale. "
)
HLP_ITEM(
	"string.match",
	"void string.match(s, pattern, [, init])",
	"Looks for the first match of pattern in the string s. If it finds one, then match returns the \n" \
	"captures from the pattern; otherwise it returns nil. If pattern specifies no captures, then \n" \
	"the whole match is returned. A third, optional numerical argument init specifies where to \n" \
	"start the search; its default value is 1 and can be negative. "
)
HLP_ITEM(
	"string.rep",
	"void string.rep(s, n)",
	"Returns a string that is the concatenation of n copies of the string s. "
)
HLP_ITEM(
	"string.reverse",
	"void string.reverse(s)",
	"Returns a string that is the string s reversed. "
)
HLP_ITEM(
	"string.sub",
	"void string.sub(s, i, [, j])",
	"Returns the substring of s that starts at i and continues until j; i and j can be negative. \n" \
	"If j is absent, then it is assumed to be equal to -1 (which is the same as the string length). \n" \
	"In particular, the call string.sub(s,1,j) returns a prefix of s with length j, and string.sub(s, -i) \n" \
	"returns a suffix of s with length i. "
)
HLP_ITEM(
	"string.upper",
	"void string.upper(s)",
	"Receives a string and returns a copy of this string with all lowercase letters changed to \n" \
	"uppercase. All other characters are left unchanged. The definition of what a lowercase letter \n" \
	"is depends on the current locale. "
)
HLP_ITEM(
	"table.concat",
	"void table.concat(table, [, sep [, i [, j]]])",
	"Given an array where all elements are strings or numbers, returns table[i]..sep..table[i+1] ··· sep..table[j]. \n" \
	"The default value for sep is the empty string, the default for i is 1, and the default for j is the length \n" \
	"of the table. If i is greater than j, returns the empty string. "
)
HLP_ITEM(
	"table.insert",
	"void table.insert(table, [pos,], value)",
	"Inserts element value at position pos in table, shifting up other elements to open space, \n" \
	"if necessary. The default value for pos is n+1, where n is the length of the table (see §2.5.5), \n" \
	"so that a call table.insert(t,x) inserts x at the end of table t. "
)
HLP_ITEM(
	"table.maxn",
	"void table.maxn(table)",
	"Returns the largest positive numerical index of the given table, or zero if the table has no \n" \
	"positive numerical indices. (To do its job this function does a linear traversal of the whole table.) "
)
HLP_ITEM(
	"table.remove",
	"void table.remove(table, [, pos])",
	"Removes from table the element at position pos, shifting down other elements to close the space, \n" \
	"if necessary. Returns the value of the removed element. The default value for pos is n, where n \n" \
	"is the length of the table, so that a call table.remove(t) removes the last element of table t. "
)
HLP_ITEM(
	"table.sort",
	"void table.sort(table, [, comp])",
	"Sorts table elements in a given order, in-place, from table[1] to table[n], where n is the length \n" \
	"of the table. If comp is given, then it must be a function that receives two table elements, and \n" \
	"returns true when the first is less than the second (so that not comp(a[i+1],a[i]) will be true \n" \
	"after the sort). If comp is not given, then the standard Lua operator lessthan is used instead.\n" \
	"\n" \
	"The sort algorithm is not stable; that is, elements considered equal by the given order may have \n" \
	"their relative positions changed by the sort. "
)
HLP_ITEM(
	"tonumber",
	"void tonumber(e, [, base])",
	"Tries to convert its argument to a number. If the argument is already a number or a string \n" \
	"convertible to a number, then tonumber returns this number; otherwise, it returns nil.\n" \
	"\n" \
	"An optional argument specifies the base to interpret the numeral. The base may be any integer \n" \
	"between 2 and 36, inclusive. In bases above 10, the letter 'A' (in either upper or lower case) \n" \
	"represents 10, 'B' represents 11, and so forth, with 'Z' representing 35. In base 10 (the default), \n" \
	"the number can have a decimal part, as well as an optional exponent part (see §2.1). In other \n" \
	"bases, only unsigned integers are accepted. "
)
HLP_ITEM(
	"tostring",
	"void tostring(e)",
	"Receives an argument of any type and converts it to a string in a reasonable format. For complete \n" \
	"control of how numbers are converted, use string.format.\n" \
	"\n" \
	"If the metatable of e has a '__tostring' field, then tostring calls the corresponding value with \n" \
	"e as argument, and uses the result of the call as its result. "
)
HLP_ITEM(
	"type",
	"void type(v)",
	"Returns the type of its only argument, coded as a string. The possible results of this function \n" \
	"are 'nil' (a string, not the value nil), 'number', 'string', 'boolean', 'table', 'function', 'thread', and 'userdata'. "
)
HLP_ITEM(
	"unpack",
	"void unpack(list, [, i [, j]])",
	"Returns the elements from the given table. This function is equivalent to\n" \
	"\n" \
	"     return list[i], list[i+1], ···, list[j]\n" \
	"\n" \
	"except that the above code can be written only for a fixed number of elements. By default, i is 1 and \n" \
	"j is the length of the list, as defined by the length operator (see §2.5.5). "
)
HLP_ITEM(
	"xpcall",
	"void xpcall(f, err)",
	"This function is similar to pcall, except that you can set a new error handler.\n" \
	"\n" \
	"xpcall calls function f in protected mode, using err as the error handler. Any error inside f is \n" \
	"not propagated; instead, xpcall catches the error, calls the err function with the original error \n" \
	"object, and returns a status code. Its first result is the status code (a boolean), which is true \n" \
	"if the call succeeds without errors. In this case, xpcall also returns all results from the call, \n" \
	"after this first result. In case of any error, xpcall returns false plus the result from err. "
)

#undef HLP_LABEL
#undef HLP_ITEM
