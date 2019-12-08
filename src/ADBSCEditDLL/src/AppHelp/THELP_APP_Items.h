
HLP_LABEL("EDITOR KEY:")
HLP_ITEM("CTRL-N", "Load default skeleton script", "(editor operation)")
HLP_ITEM("CTRL-O", "Open script", "(editor operation)")
HLP_ITEM("CTRL-S, F2", "Save script as default file", "(editor operation)")
HLP_ITEM("CTRL-F2", "Save script as selected file name", "(editor operation)")

HLP_ITEM("CTRL-A", "Select all code edit text", "(editor operation)")
HLP_ITEM("CTRL-C", "Copy selected text", "(editor operation)")
HLP_ITEM("CTRL-V", "Paste from clipboard text", "(editor operation)")
HLP_ITEM("CTRL-X", "Cut selected text", "(editor operation)")
HLP_ITEM("CTRL-Y", "Redo editor text", "(editor operation)")
HLP_ITEM("CTRL-Z", "Undo editor text", "(editor operation)")

HLP_ITEM("CTRL-Q", "Quit", "(all operation)")
HLP_ITEM("ALT-T", "Set window always top", "(all operation)")
HLP_ITEM("F1", "Show this short help", "(all operation)")

HLP_LABEL("DEBUGGER KEY:")
HLP_ITEM("ALT-V, F3", "Using external DebugView", "(debugger operation)")
HLP_ITEM("ALT-S, F4", "Debug mode ' step by step '", "(debugger operation)")
HLP_ITEM("ALT-C, F5", "Debug mode ' auto cycle '", "(debugger default option)")
HLP_ITEM("ALT-D, F6", "Dump stack, only ' step by step ' mode", "(debugger operation)")
HLP_ITEM("ALT-N, F7", "Go next line", "(debugger operation)")
HLP_ITEM("ALT-B, F8", "Break running script, immediate close and exit script", "(debugger operation)")

HLP_LABEL("VERSION:")
HLP_ITEM("Android ADB Viewer script editor", AVIEW_FULLVERSION_STRING, "Build: " AVIEW_DATE "/" AVIEW_MONTH "/" AVIEW_YEAR)
HLP_ITEM(nullptr, nullptr, "https://github.com/ClnViewer/ADB-Android-Viewer")

#undef HLP_LABEL
#undef HLP_ITEM
