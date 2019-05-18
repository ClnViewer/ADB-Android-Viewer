
#include "../ADBViewer.h"
#include  "SDL2/SDL_syswm.h"

static inline const LPCSTR l_openBmpFilter = "Bitmap files (*.bmp)\0*.bmp\0";
static inline const LPCSTR l_openBmpExt = "bmp";
static inline const LPCSTR l_openApkFilter = "APK files (*.apk)\0*.apk\0";
static inline const LPCSTR l_openApkExt = "apk";
static inline const LPCSTR l_openCurDir = ".\\";

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

bool AppMenu::savefile(std::string & fname)
{
    OPENFILENAMEA sfn{};
    std::stringstream ss;
    ss << fname << std::to_string(time(NULL)) << "." << l_openBmpExt;
    fname.assign(ss.str().c_str());
    fname.resize(512);

    HWND hwnd;
    if (!(hwnd = f_GetHWND(m_app->m_window)))
        return false;

    f_SetupFileDialog(
        hwnd, &sfn, fname,
        OFN_LONGNAMES | OFN_OVERWRITEPROMPT,
        l_openBmpFilter, l_openBmpExt, l_openCurDir
    );

    if (GetSaveFileNameA(&sfn))
        return true;
    return false;
}

bool AppMenu::openfile(std::string & fname)
{
    OPENFILENAMEA sfn{};
    fname.resize(512);

    HWND hwnd;
    if (!(hwnd = f_GetHWND(m_app->m_window)))
        return false;

    f_SetupFileDialog(
        hwnd, &sfn, fname,
        OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
        l_openApkFilter, l_openApkExt, l_openCurDir
    );

    if (GetOpenFileNameA(&sfn))
        return true;
    return false;
}
