
#include "../ADBViewer.h"
#include  "SDL2/SDL_syswm.h"
#include  "../../../ADBDriverDLL/src/Utils/stdStringUtils.h"

#define MENU_ITEM_ADD(ID1,ID2,M) __extension__ ({                               \
    auto wtxt = ResManager::stringpopup(ID2, AppConfig::instance().cnf_lang);   \
    AppendMenuW(M, MF_STRING, ID1, wtxt);                                       \
    })

#define MENU_ADD(PM,ID2,M) __extension__ ({                                     \
    auto wtxt = ResManager::stringpopup(ID2, AppConfig::instance().cnf_lang);   \
    AppendMenuW(M, MF_STRING | MF_POPUP, (UINT_PTR)PM, wtxt);                   \
    })

typedef struct
{
    uint32_t cid;
    int32_t  key;
    ResManager::IndexStringPopUpMenu sid;

} ACmdKey;

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

static ACmdKey l_acmdkey[] =
{
#  define __MENU_ACMD(A,B,C) { .cid = A, .key = C, .sid = B },
#  include "AppMenuPopUpAcmd.WINAPI.h"
};

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
           l_hCmdMenu = NULL,
           l_hScrMenu = NULL,
           l_hScrTypeMenu = NULL,
           l_hPlugMenu = NULL,
           l_hLangMenu = NULL;
    do
    {
        if (
            (!(l_hPopMenu = ::CreatePopupMenu())) ||
            (!(l_hCapMenu = ::CreateMenu())) ||
            (!(l_hCmdMenu = ::CreateMenu())) ||
            (!(l_hScrMenu = ::CreateMenu())) ||
            (!(l_hPlugMenu = ::CreateMenu()))||
            (!(l_hLangMenu = ::CreateMenu()))||
            (!(l_hScrTypeMenu = ::CreateMenu()))
           )
           break;

        MENUITEMINFO mit{};
        mit.cbSize = sizeof(mit);
        mit.fMask = MIIM_STATE;
        mit.fState = MFS_CHECKED;

        MENU_ADD(l_hCapMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_0, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        MENU_ADD(l_hCmdMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_16, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        MENU_ADD(l_hScrMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_6, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        MENU_ADD(l_hPlugMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_12, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);

        if ((m_app) && (!m_app->m_appeditor.isactive()))
            MENU_ITEM_ADD(ID_CMD_POP_MENU6, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_7, l_hScrMenu);
        else
        {
            MENU_ITEM_ADD(ID_CMD_POP_MENU7, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_11, l_hScrMenu);
            ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU6, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_8, l_hScrMenu);
            ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ADD(l_hScrTypeMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_13, l_hScrMenu);

            MENU_ITEM_ADD(ID_CMD_POP_MENU9,  ResManager::IndexStringPopUpMenu::RES_STR_POPUP_14, l_hScrTypeMenu);
            ::AppendMenuW(l_hScrTypeMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU10, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_15, l_hScrTypeMenu);

            switch (m_app->m_appeditor.scrtype)
            {
                case AppEditor::AppEditorScriptType::SCR_CHECK_AND_CLICK:
                    {
                        ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);
                        MENU_ITEM_ADD(ID_CMD_POP_MENU8, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_9, l_hScrMenu);

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

        MENU_ITEM_ADD(ID_CMD_POP_MENU1, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_1, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU2, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_2, l_hPopMenu);

        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ADD(l_hLangMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_27, l_hPopMenu);
        {
            MENU_ITEM_ADD(ID_CMD_POP_MENU11, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_28, l_hLangMenu);
            ::AppendMenuW(l_hLangMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU12, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_29, l_hLangMenu);
            ::AppendMenuW(l_hLangMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU13, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_30, l_hLangMenu);

            if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_RU)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU11, FALSE, &mit);
            else if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_EN)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU12, FALSE, &mit);
            else if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_CN)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU13, FALSE, &mit);
        }

        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU3, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_3, l_hPopMenu);

        MENU_ITEM_ADD(ID_CMD_POP_MENU4, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_4, l_hCapMenu);
        ::AppendMenuW(l_hCapMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU5, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_5, l_hCapMenu);

        /// Plugins menu list
        std::vector<Plugins::AppPluginManager::Plugin_s> plist =
                    Plugins::AppPluginManager::instance().listplugin();

        for (uint32_t i = 0; i < plist.size(); i++)
        {
            std::wstring ws(plist[i].name.begin(), plist[i].name.end());
            ::AppendMenuW(l_hPlugMenu, MF_STRING, (i + 50000U), ws.c_str());
            if (plist[i].state)
                ::SetMenuItemInfo(l_hPlugMenu, (i + 50000U), FALSE, &mit);
        }

        /// Android direct command menu list
        for (uint32_t i = 0; i < __NELE(l_acmdkey); i++)
        {
            auto wtxt = ResManager::stringpopup(l_acmdkey[i].sid, AppConfig::instance().cnf_lang);
            ::AppendMenuW(l_hCmdMenu, MF_STRING, l_acmdkey[i].cid, wtxt);
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
        ::SetMenuInfo(l_hLangMenu,    &mip);
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
            case ID_CMD_POP_MENU11:
                {
                    AppConfig::instance().cnf_lang = ResManager::IndexLanguageResource::LANG_RU;
                    break;
                }
            case ID_CMD_POP_MENU12:
                {
                    AppConfig::instance().cnf_lang = ResManager::IndexLanguageResource::LANG_EN;
                    break;
                }
            case ID_CMD_POP_MENU13:
                {
                    AppConfig::instance().cnf_lang = ResManager::IndexLanguageResource::LANG_CN;
                    break;
                }
            case ID_CMD_POP_MENU30 ... ID_CMD_POP_MENU39:
                {
                    for (uint32_t i = 0; i < __NELE(l_acmdkey); i++)
                        if (idx == l_acmdkey[i].cid)
                        {
                            AppConfig::instance().cnf_adb.SendSpecialKey(
                                    GameDev::ADBDriver::KeysType::KEYS_ANDROID,
                                    l_acmdkey[i].key
                                );
                            break;
                        }
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

    if (l_hLangMenu)
        ::DestroyMenu(l_hLangMenu);
    if (l_hPlugMenu)
        ::DestroyMenu(l_hPlugMenu);
    if (l_hScrTypeMenu)
        ::DestroyMenu(l_hScrTypeMenu);
    if (l_hScrMenu)
        ::DestroyMenu(l_hScrMenu);
    if (l_hCmdMenu)
        ::DestroyMenu(l_hCmdMenu);
    if (l_hCapMenu)
        ::DestroyMenu(l_hCapMenu);
    if (l_hPopMenu)
        ::DestroyMenu(l_hPopMenu);
    if (l_hbrush)
        ::DeleteObject(l_hbrush);
}

#else

bool AppMenuPopUp::init(App *app) { return false; }
void AppMenuPopUp::show() { /* */ }

#endif
