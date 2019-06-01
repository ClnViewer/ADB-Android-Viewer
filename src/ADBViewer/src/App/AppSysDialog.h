#pragma once

#include "../ADBViewer.h"

class AppSysDialog
{
public:
    //
    static bool savefile(SDL_Window*, std::string&, const LPCSTR, const LPCSTR, const LPCSTR);
    static bool openfile(SDL_Window*, std::string&, const LPCSTR, const LPCSTR, const LPCSTR);

#   if defined(OS_WIN)
    static HWND gethwnd(SDL_Window*);
#   endif
};
