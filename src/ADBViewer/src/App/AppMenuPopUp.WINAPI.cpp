
#include "../ADBViewer.h"
#include  "SDL2/SDL_syswm.h"
#include  "../../../ADBDriverDLL/src/Utils/stdStringUtils.h"

AppMenuPopUp::AppMenuPopUp()
    : m_app(nullptr)
{
    gui.rect.h = 0;
    gui.rect.w = 0;
    gui.rect.x = 0;
    gui.rect.y = 0;
    gui.texture = nullptr;
    gui.instance = nullptr;
    gui.active = false;
}

AppMenuPopUp::~AppMenuPopUp() {}

#if defined(OS_WIN)
#include  "AppMenuPopUp.WINAPI.h"

static HWND f_GetHWND(SDL_Window *win)
{
    SDL_SysWMinfo wmInfo{};
    SDL_VERSION(&wmInfo.version);

    if (!SDL_GetWindowWMInfo(win, &wmInfo))
        return nullptr;

    return wmInfo.info.win.window;
}

static const wchar_t * f_GetStringFromMap(int32_t id, std::map<int32_t, std::wstring> & rmap)
{
    auto res = rmap.find(id);
    if (res != rmap.end())
        return res->second.c_str();
    return nullptr;
}

bool AppMenuPopUp::init(App *app)
{
    if (!app)
        return false;

    m_app = app;
    if (!(m_hwnd = f_GetHWND(m_app->m_window)))
        return false;

    bool ret = initgui(app);
    if (ret)
        gui.active = false;
    return ret;
}

void AppMenuPopUp::show()
{
    if (!m_hwnd)
        return;

    HBRUSH l_hbrush   = NULL;
    HMENU  l_hMenuPop = NULL,
           l_hCapMenu = NULL;
    do
    {
        if (
            (!(l_hMenuPop = ::CreatePopupMenu())) ||
            (!(l_hCapMenu = ::CreateMenu()))
           )
           break;

        RESOURCE_MAP(IDS_MENUMSG0, IDS_MENUEND, rmap);

        MENU_ADD(l_hCapMenu, IDS_MENUMSG0,  rmap, l_hMenuPop);
        ::AppendMenuW(l_hMenuPop,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU1, IDS_MENUMSG1,  rmap, l_hMenuPop);
        ::AppendMenuW(l_hMenuPop,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU2, IDS_MENUMSG2,  rmap, l_hMenuPop);
        ::AppendMenuW(l_hMenuPop,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU3, IDS_MENUMSG3,  rmap, l_hMenuPop);

        MENU_ITEM_ADD(ID_CMD_POP_MENU4, IDS_MENUMSG4,  rmap, l_hCapMenu);
        ::AppendMenuW(l_hCapMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU5, IDS_MENUMSG5,  rmap, l_hCapMenu);

        /// light: RGB(191,227,103)
        if (!(l_hbrush = CreateSolidBrush(RGB(151,192,36))))
            break;

        int32_t x, y;
        MENUINFO mi{};

        mi.cbSize = sizeof(mi);
        mi.hbrBack = (HBRUSH) l_hbrush;
        mi.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE | MIM_BACKGROUND;
        mi.dwStyle = MNS_AUTODISMISS | MNS_NOCHECK;

        ::SetMenuInfo(l_hMenuPop, &mi);
        ::SDL_GetGlobalMouseState(&x, &y);

        SDL_Event cmdEvent{};
        cmdEvent.type = m_app->m_uevent;

        switch (::TrackPopupMenu(
                        l_hMenuPop,
                        TPM_RETURNCMD | TPM_NONOTIFY |
                        TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,
                        x, y,
                        0,
                        m_hwnd,
                        NULL
                    )
                )
        {
            case ID_CMD_POP_MENU1:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU1;
                    break;
                }
            case ID_CMD_POP_MENU2:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU2;
                    break;
                }
            case ID_CMD_POP_MENU3:
                {
                    cmdEvent.type = SDL_QUIT;
                    break;
                }
            case ID_CMD_POP_MENU4:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU4;
                    break;
                }
            case ID_CMD_POP_MENU5:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU5;
                    break;
                }
            default:
                break;
        }
        if ((cmdEvent.type == SDL_QUIT) || (cmdEvent.user.code != 0))
            SDL_PushEvent(&cmdEvent);
    }
    while (0);

    if (l_hMenuPop)
        ::DestroyMenu(l_hMenuPop);
    if (l_hCapMenu)
        ::DestroyMenu(l_hCapMenu);
    if (l_hbrush)
        ::DeleteObject(l_hbrush);
}

#else

bool AppMenuPopUp::init(App *app) { return false; }
void AppMenuPopUp::show() { /* */ }

#endif
