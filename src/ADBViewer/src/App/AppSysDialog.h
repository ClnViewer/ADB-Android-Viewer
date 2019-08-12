#pragma once

#include "../ADBViewer.h"

class AppSysDialog
{
public:
    //
#   if defined(OS_WIN)
    static HWND m_hWnd;
    static HWND gethwnd(SDL_Window*);
#   endif

    static bool savefile(SDL_Window*, std::string&, const LPCSTR, const LPCSTR, const LPCSTR);
    static bool openfile(SDL_Window*, std::string&, const LPCSTR, const LPCSTR, const LPCSTR);    static void cliptextset(SDL_Window*, std::string const&);
};
