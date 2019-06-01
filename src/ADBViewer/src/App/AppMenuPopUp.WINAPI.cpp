
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

//#if defined(OS_WIN)
#include  "AppMenuPopUp.WINAPI.h"

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
    if (!(m_hwnd = AppSysDialog::gethwnd(m_app->m_window)))
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
    HMENU  l_hPopMenu = NULL,
           l_hCapMenu = NULL,
           l_hScrMenu = NULL,
           l_hScrTypeMenu = NULL,
           l_hPlugMenu = NULL;
    do
    {
        if (
            (!(l_hPopMenu = ::CreatePopupMenu())) ||
            (!(l_hCapMenu = ::CreateMenu())) ||
            (!(l_hScrMenu = ::CreateMenu())) ||
            (!(l_hPlugMenu = ::CreateMenu()))||
            (!(l_hScrTypeMenu = ::CreateMenu()))
           )
           break;

        MENUITEMINFO mit{};
        mit.cbSize = sizeof(mit);
        mit.fMask = MIIM_STATE;
        mit.fState = MFS_CHECKED;

        RESOURCE_MAP(IDS_MENUMSG0, IDS_MENUEND, rmap);

        MENU_ADD(l_hCapMenu, IDS_MENUMSG0,  rmap, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        MENU_ADD(l_hScrMenu, IDS_MENUMSG6,  rmap, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        MENU_ADD(l_hPlugMenu, IDS_MENUMSG12,  rmap, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        if ((m_app) && (!m_app->m_appeditor.isactive()))
            MENU_ITEM_ADD(ID_CMD_POP_MENU6, IDS_MENUMSG7,  rmap, l_hScrMenu);
        else
        {
            MENU_ITEM_ADD(ID_CMD_POP_MENU7, IDS_MENUMSG11,  rmap, l_hScrMenu);
            ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU6, IDS_MENUMSG8,  rmap, l_hScrMenu);
            ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ADD(l_hScrTypeMenu, IDS_MENUMSG13,  rmap, l_hScrMenu);

            MENU_ITEM_ADD(ID_CMD_POP_MENU9,  IDS_MENUMSG14,  rmap, l_hScrTypeMenu);
            ::AppendMenuW(l_hScrTypeMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU10, IDS_MENUMSG15,  rmap, l_hScrTypeMenu);

            switch (m_app->m_appeditor.scrtype)
            {
                case AppEditor::AppEditorScriptType::SCR_CHECK_AND_CLICK:
                    {
                        ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
                        MENU_ITEM_ADD(ID_CMD_POP_MENU8, IDS_MENUMSG9,  rmap, l_hScrMenu);

                        ::SetMenuItemInfo(l_hScrTypeMenu, ID_CMD_POP_MENU9, FALSE, &mit);
                        break;
                    }
                case AppEditor::AppEditorScriptType::SCR_CLICK_ONLY:
                    {
                        ::SetMenuItemInfo(l_hScrTypeMenu, ID_CMD_POP_MENU10, FALSE, &mit);
                        break;
                    }
                default:
                    break;
            }
        }

        MENU_ITEM_ADD(ID_CMD_POP_MENU1, IDS_MENUMSG1,  rmap, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU2, IDS_MENUMSG2,  rmap, l_hPopMenu);

        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU3, IDS_MENUMSG3,  rmap, l_hPopMenu);

        MENU_ITEM_ADD(ID_CMD_POP_MENU4, IDS_MENUMSG4,  rmap, l_hCapMenu);
        ::AppendMenuW(l_hCapMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU5, IDS_MENUMSG5,  rmap, l_hCapMenu);

        /// Plugin menu list

        std::vector<Plugins::AppPluginManager::Plugin_s> plist =
                    Plugins::AppPluginManager::instance().listplugin();

        for (uint32_t i = 0; i < plist.size(); i++)
        {
            std::wstring ws(plist[i].name.begin(), plist[i].name.end());
            ::AppendMenuW(l_hPlugMenu, MF_STRING, (i + 50000U), ws.c_str());
            if (plist[i].state)
                ::SetMenuItemInfo(l_hPlugMenu, (i + 50000U), FALSE, &mit);
        }

        /// light: RGB(191,227,103)
        if (!(l_hbrush = ::CreateSolidBrush(RGB(151,192,36))))
            break;

        int32_t x, y;
        MENUINFO mim{}, mip{};

        mim.cbSize = sizeof(mim);
        mim.hbrBack = (HBRUSH) l_hbrush;
        mim.fMask = MIM_APPLYTOSUBMENUS | MIM_STYLE | MIM_BACKGROUND;
        mim.dwStyle = MNS_AUTODISMISS | MNS_NOCHECK;

        ::memcpy(&mip, &mim, sizeof(mip));
        mip.dwStyle = MNS_AUTODISMISS | MNS_CHECKORBMP;

        ::SetMenuInfo(l_hPopMenu,     &mim);
        ::SetMenuInfo(l_hPlugMenu,    &mip);
        ::SetMenuInfo(l_hScrTypeMenu, &mip);
        ::SDL_GetGlobalMouseState(&x, &y);

        SDL_Event cmdEvent{};
        cmdEvent.type = m_app->m_uevent;

        uint32_t idx = ::TrackPopupMenu(
                        l_hPopMenu,
                        TPM_RETURNCMD | TPM_NONOTIFY |
                        TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,
                        x, y,
                        0,
                        m_hwnd,
                        NULL
                    );

#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (idx)
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
            case ID_CMD_POP_MENU6:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU6;
                    break;
                }
            case ID_CMD_POP_MENU7:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU7;
                    break;
                }
            case ID_CMD_POP_MENU8:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU8;
                    break;
                }
            case ID_CMD_POP_MENU9:
                {
                    m_app->m_appeditor.scrtype = AppEditor::AppEditorScriptType::SCR_CHECK_AND_CLICK;
                    cmdEvent.user.code = 0;
                    break;
                }
            case ID_CMD_POP_MENU10:
                {
                    m_app->m_appeditor.scrtype = AppEditor::AppEditorScriptType::SCR_CLICK_ONLY;
                    cmdEvent.user.code = 0;
                    break;
                }
            case 50000 ... 50999:
                {
                    uint32_t i = (idx - 50000U);
                    if (i >= plist.size())
                        break;
                    if (plist[i].state)
                        Plugins::AppPluginManager::instance().disableplugin(plist[i].name);
                    else
                        Plugins::AppPluginManager::instance().enableplugin(plist[i].name);
                    cmdEvent.user.code = 0;
                    break;
                }
            default:
                break;
        }
#       pragma GCC diagnostic pop

        if ((cmdEvent.type == SDL_QUIT) || (cmdEvent.user.code != 0))
            SDL_PushEvent(&cmdEvent);
    }
    while (0);

    if (l_hPlugMenu)
        ::DestroyMenu(l_hPlugMenu);
    if (l_hScrTypeMenu)
        ::DestroyMenu(l_hScrTypeMenu);
    if (l_hScrMenu)
        ::DestroyMenu(l_hScrMenu);
    if (l_hCapMenu)
        ::DestroyMenu(l_hCapMenu);
    if (l_hPopMenu)
        ::DestroyMenu(l_hPopMenu);
    if (l_hbrush)
        ::DeleteObject(l_hbrush);
}

#if defined(OS_WIN)
#else

bool AppMenuPopUp::init(App *app) { return false; }
void AppMenuPopUp::show() { /* */ }

#endif
