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
    guiBase::gui.rect.h = 0;
    guiBase::gui.rect.w = 0;
    guiBase::gui.rect.x = 0;
    guiBase::gui.rect.y = 0;
    guiBase::gui.texture = nullptr;
    guiBase::gui.instance = nullptr;
    guiBase::ActiveOff();
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

    bool ret = guiBase::initgui(app);
    if (ret)
        guiBase::ActiveOff();
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
           l_hLangMenu = NULL,
           l_hDisplayMenu = NULL,
           l_hDisplayRatioMenu = NULL,
           l_hDisplayRotateMenu = NULL;
    do
    {
        if (
            (!(l_hPopMenu = ::CreatePopupMenu())) ||
            (!(l_hCapMenu = ::CreateMenu())) ||
            (!(l_hCmdMenu = ::CreateMenu())) ||
            (!(l_hScrMenu = ::CreateMenu())) ||
            (!(l_hPlugMenu = ::CreateMenu()))||
            (!(l_hLangMenu = ::CreateMenu()))||
            (!(l_hScrTypeMenu = ::CreateMenu())) ||
            (!(l_hDisplayMenu = ::CreateMenu())) ||
            (!(l_hDisplayRatioMenu = ::CreateMenu())) ||
            (!(l_hDisplayRotateMenu = ::CreateMenu()))
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
                        /// TODO: check result text
                        ::AppendMenuW(l_hScrMenu,  MF_SEPARATOR, 0, NULL);

                        if (m_app->m_appeditor.istarget())
                            MENU_ITEM_ADD(ID_CMD_POP_MENU8, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_10, l_hScrMenu);
                        else
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

        MENU_ITEM_ADD(ID_CMD_POP_MENU100, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_44, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU24, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_45, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
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
            ::AppendMenuW(l_hLangMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU14, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_31, l_hLangMenu);

            if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_RU)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU11, FALSE, &mit);
            else if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_EN)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU12, FALSE, &mit);
            else if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_DM)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU13, FALSE, &mit);
            else if (AppConfig::instance().cnf_lang == ResManager::IndexLanguageResource::LANG_CN)
                ::SetMenuItemInfo(l_hLangMenu, ID_CMD_POP_MENU14, FALSE, &mit);
        }

        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ADD(l_hDisplayMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_32, l_hPopMenu);
        {
            MENU_ADD(l_hDisplayRatioMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_33, l_hDisplayMenu);
            ::AppendMenuW(l_hDisplayMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ADD(l_hDisplayRotateMenu, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_34, l_hDisplayMenu);
            ::AppendMenuW(l_hDisplayMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU15, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_35, l_hDisplayMenu);

            MENU_ITEM_ADD(ID_CMD_POP_MENU16, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_36, l_hDisplayRotateMenu);
            ::AppendMenuW(l_hDisplayRotateMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU17, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_43, l_hDisplayRotateMenu);
            //::AppendMenuW(l_hDisplayRotateMenu,  MF_SEPARATOR, 0, NULL);
            //MENU_ITEM_ADD(ID_CMD_POP_MENU23, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_36, l_hDisplayRotateMenu);

            switch (AppConfig::instance().cnf_disp_rotate)
            {
                case 360U:
                    {
                        ::SetMenuItemInfo(l_hDisplayRotateMenu, ID_CMD_POP_MENU16, FALSE, &mit);
                        break;
                    }
                case 90U: //TODO: ?? Mirror this :)
                    {
                        ::SetMenuItemInfo(l_hDisplayRotateMenu, ID_CMD_POP_MENU17, FALSE, &mit);
                        break;
                    }
            }

            MENU_ITEM_ADD(ID_CMD_POP_MENU18, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_38, l_hDisplayRatioMenu);
            ::AppendMenuW(l_hDisplayRatioMenu,  MF_SEPARATOR, 0, NULL);
            MENU_ITEM_ADD(ID_CMD_POP_MENU19, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_39, l_hDisplayRatioMenu);
            //::AppendMenuW(l_hDisplayRatioMenu,  MF_SEPARATOR, 0, NULL);
            //MENU_ITEM_ADD(ID_CMD_POP_MENU20, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_40, l_hDisplayRatioMenu);

            switch (AppConfig::instance().cnf_disp_ratio)
            {
                case 1U:
                    {
                        ::SetMenuItemInfo(l_hDisplayRatioMenu, ID_CMD_POP_MENU18, FALSE, &mit);
                        break;
                    }
                case 2U:
                    {
                        ::SetMenuItemInfo(l_hDisplayRatioMenu, ID_CMD_POP_MENU19, FALSE, &mit);
                        break;
                    }
                case 3U:
                    {
                        //::SetMenuItemInfo(l_hDisplayRatioMenu, ID_CMD_POP_MENU20, FALSE, &mit);
                        break;
                    }
            }
        }

        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU21, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_41, l_hPopMenu);
        ::AppendMenuW(l_hPopMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU3, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_3, l_hPopMenu);

        MENU_ITEM_ADD(ID_CMD_POP_MENU4, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_4, l_hCapMenu);
        ::AppendMenuW(l_hCapMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU5, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_5, l_hCapMenu);
        ::AppendMenuW(l_hCapMenu,  MF_SEPARATOR, 0, NULL);
        MENU_ITEM_ADD(ID_CMD_POP_MENU22, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_42, l_hCapMenu);

        /// Plugins menu list
        std::vector<Plugins::AppPluginManager::Plugin_s> plist =
                    Plugins::AppPluginManager::instance().listplugin();

        for (uint32_t i = 0U; i < plist.size(); i++)
        {
            std::wstring ws(plist[i].name.begin(), plist[i].name.end());
            ::AppendMenuW(l_hPlugMenu, MF_STRING, (i + 50000U), ws.c_str());
            if (plist[i].state)
                ::SetMenuItemInfo(l_hPlugMenu, (i + 50000U), FALSE, &mit);
        }

        /// Android direct command menu list
        for (uint32_t i = 0U; i < __NELE(l_acmdkey); i++)
        {
            auto wtxt = ResManager::stringpopup(l_acmdkey[i].sid, AppConfig::instance().cnf_lang);
            ::AppendMenuW(l_hCmdMenu, MF_STRING, l_acmdkey[i].cid, wtxt);
        }
        /// Android notify call command to menu list
        MENU_ITEM_ADD(ID_CMD_POP_MENU50, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_46, l_hCmdMenu);
        MENU_ITEM_ADD(ID_CMD_POP_MENU51, ResManager::IndexStringPopUpMenu::RES_STR_POPUP_47, l_hCmdMenu);

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
        ::SetMenuInfo(l_hDisplayMenu, &mim);
        ::SetMenuInfo(l_hDisplayRatioMenu, &mip);
        ::SetMenuInfo(l_hDisplayRotateMenu,&mip);
        ::SDL_GetGlobalMouseState(&x, &y);

        SDL_Event cmdEvent{};
        cmdEvent.type = AppConfig::instance().cnf_uevent;

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
                    AppConfig::instance().cnf_lang = ResManager::IndexLanguageResource::LANG_DM;
                    break;
                }
            case ID_CMD_POP_MENU14:
                {
                    AppConfig::instance().cnf_lang = ResManager::IndexLanguageResource::LANG_CN;
                    break;
                }
            case ID_CMD_POP_MENU15:
                {
                    AppConfig::instance().cnf_disp_bender = !(AppConfig::instance().cnf_disp_bender.load());
                    cmdEvent.user.code = ID_CMD_POP_MENU15;
                    break;
                }
            case ID_CMD_POP_MENU16:
            case ID_CMD_POP_MENU17:
                {
                    switch (idx)
                    {
                        case ID_CMD_POP_MENU16: AppConfig::instance().cnf_disp_rotate = 360U; break;
                        case ID_CMD_POP_MENU17: AppConfig::instance().cnf_disp_rotate = 90U; break;
                    }
                    cmdEvent.user.code = ID_CMD_POP_MENU99;
                    break;
                }
            case ID_CMD_POP_MENU18:
            case ID_CMD_POP_MENU19:
            case ID_CMD_POP_MENU20:
                {
                    switch (idx)
                    {
                        case ID_CMD_POP_MENU18: AppConfig::instance().cnf_disp_ratio = 1U; break;
                        case ID_CMD_POP_MENU19: AppConfig::instance().cnf_disp_ratio = 2U; break;
                        // case ID_CMD_POP_MENU20: AppConfig::instance().cnf_disp_ratio = 3U; break;
                    }
                    cmdEvent.user.code = ID_CMD_POP_MENU99;
                    break;
                }
            case ID_CMD_POP_MENU21:
                {
                    AppConfig::instance().SaveToFile();
                    cmdEvent.user.code = 0;
                    break;
                }
            case ID_CMD_POP_MENU22:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU22;
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
            case ID_CMD_POP_MENU50:
            case ID_CMD_POP_MENU51:
                {
                    int32_t key = ((idx == ID_CMD_POP_MENU50) ? 82 : 92
                                    /* 75 : 78 */
                                );
                    for (uint32_t i = 0; i < 2; i++)
                        AppConfig::instance().cnf_adb.SendSpecialKey(
                                GameDev::ADBDriver::KeysType::KEYS_ANDROID,
                                key
                            );

                    cmdEvent.user.code = 0;
                    break;
                }
            case ID_CMD_POP_MENU100:
                {
                    cmdEvent.user.code = ID_CMD_POP_MENU100;
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

    if (l_hDisplayRotateMenu)
        ::DestroyMenu(l_hDisplayRotateMenu);
    if (l_hDisplayRatioMenu)
        ::DestroyMenu(l_hDisplayRatioMenu);
    if (l_hDisplayMenu)
        ::DestroyMenu(l_hDisplayMenu);
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
