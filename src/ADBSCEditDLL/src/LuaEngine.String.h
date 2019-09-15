
#pragma once

namespace Editor
{
	static inline const char g_debug_str_0[]  = "Interrupt debug process";
	static inline const char g_debug_str_1[]  = "Debug run method set to ' step by step '";
	static inline const char g_debug_str_2[]  = "Debug run method set to ' cycle '";
	static inline const char g_debug_str_3[]  = "Script load successfully";
	static inline const char g_debug_str_4[]  = "Create Lua instance error";
	static inline const char g_debug_str_5[]  = "Debug process already running..";
	static inline const char g_debug_str_6[]  = "Debug process stopped";
	static inline const char g_debug_str_7[]  = "Close previous loading script..";
	static inline const char g_debug_str_8[]  = "Debug process not running..";
	static inline const char g_debug_str_9[]  = "Stop running debug process, wait..";
	static inline const char g_debug_str_10[] = "Debug process started";
	static inline const char g_debug_str_11[] = "Run script mode: ' step by step '";
	static inline const char g_debug_str_12[] = "Run script mode: ' cycle '";
	static inline const char g_debug_str_13[] = "Function main(number) not found! Script not continue run!";
	static inline const char g_debug_str_14[] = "function main(number) end..\n\t\tcontinue: ALT-N, break: ALT-B";
	static inline const char g_debug_str_15[] = "function main(number) end..\n\t\trerun script, wait 5 sec..";
	static inline const char g_debug_str_16[] = "\tScript return state from function main(number): ";
	static inline const char g_debug_str_17[] = "Break debug process..";
	static inline const char g_debug_str_18[] = "Script debug process run end";
	static inline const char g_debug_str_19[] = "Script is empty!";
	static inline const char g_debug_str_20[] = "Unknown error";
	static inline const char g_debug_str_21[] = "Lua callback unknown exception";
	static inline const char g_debug_str_22[] = "Debug process running, not change run mode..";

	static inline const char g_debug_help[] =
		" - Debugger key help:\n"                                                                         \
		"\t- ALT-S, F4  : Debug mode ' step by step ', set this, script rerun required\n"                 \
		"\t- ALT-C, F5  : Debug mode ' auto cycle ', set this, script rerun required, default mode\n"     \
		"\t- ALT-N, F6  : Go next line\n"                                                                 \
		"\t- ALT-D, F7  : Dump stack, only ' step by step ' mode\n"                                       \
		"\t- ALT-B, F8  : Break running script - ' exit script '\n"                                       \
		"\n";
};
