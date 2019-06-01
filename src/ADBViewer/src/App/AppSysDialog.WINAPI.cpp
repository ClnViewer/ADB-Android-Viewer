
#include "../ADBViewer.h"
#include  "SDL2/SDL_syswm.h"

#if defined(OS_WIN)

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
    SDL_SysWMinfo wmInfo{};
    SDL_VERSION(&wmInfo.version);

    if (!SDL_GetWindowWMInfo(win, &wmInfo))
        return nullptr;

    return wmInfo.info.win.window;
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

bool AppSysDialog::savefile(std::string&, const LPCSTR, const LPCSTR, const LPCSTR) { return false; }
bool AppSysDialog::openfile(std::string&, const LPCSTR, const LPCSTR, const LPCSTR) { return false; }

#endif
