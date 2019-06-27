/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../ADBViewer.h"

#if defined(OS_WIN)
#include  "SDL2/SDL_syswm.h"

HWND AppSysDialog::m_hWnd = nullptr;

static void f_SetupFileDialog(
    HWND hWnd, OPENFILENAMEA *sfn, std::string & s, DWORD flags,
    LPCSTR s_openFilter, LPCSTR s_openExt, LPCSTR s_openDir
)
{
    sfn->lStructSize = sizeof(OPENFILENAMEW);
    sfn->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags;
    sfn->lpstrFilter = s_openFilter;
    sfn->lpstrDefExt = s_openExt;
    sfn->lpstrInitialDir = s_openDir;
    sfn->hwndOwner = hWnd;
    sfn->lpstrFile = static_cast<LPSTR>(s.data());
    sfn->nMaxFile = s.size();
}

static HWND f_GetHWND(SDL_Window *win)
{
    if (!AppSysDialog::m_hWnd)
    {
        SDL_SysWMinfo wmInfo{};
        SDL_VERSION(&wmInfo.version);

        if (!SDL_GetWindowWMInfo(win, &wmInfo))
            return nullptr;
        AppSysDialog::m_hWnd = wmInfo.info.win.window;
    }
    return AppSysDialog::m_hWnd;
}

bool AppSysDialog::savefile(
    SDL_Window *win,
    std::string & fname,
    const LPCSTR strFilter, const LPCSTR strExt, const LPCSTR strCurDir
)
{
    OPENFILENAMEA sfn{};
    std::stringstream ss;
    ss << fname << std::to_string(time(NULL)) << "." << strExt;
    fname.assign(ss.str().c_str());
    fname.resize(512);

    HWND hwnd;
    if (!(hwnd = f_GetHWND(win)))
        return false;

    f_SetupFileDialog(
        hwnd, &sfn, fname,
        OFN_LONGNAMES | OFN_OVERWRITEPROMPT,
        strFilter, strExt, strCurDir
    );

    if (GetSaveFileNameA(&sfn))
        return true;
    return false;
}

bool AppSysDialog::openfile(
    SDL_Window *win,
    std::string & fname,
    const LPCSTR strFilter, const LPCSTR strExt, const LPCSTR strCurDir
)
{
    OPENFILENAMEA sfn{};
    fname.resize(512);

    HWND hwnd;
    if (!(hwnd = f_GetHWND(win)))
        return false;

    f_SetupFileDialog(
        hwnd, &sfn, fname,
        OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
        strFilter, strExt, strCurDir
    );

    if (GetOpenFileNameA(&sfn))
        return true;
    return false;
}

HWND AppSysDialog::gethwnd(SDL_Window *win)
{
    return f_GetHWND(win);
}

#else

bool AppSysDialog::savefile(std::string&, const char*, const char*, const char*)
{
    return false;
}
bool AppSysDialog::openfile(std::string&, const char*, const char*, const char*)
{
    return false;
}

#endif
