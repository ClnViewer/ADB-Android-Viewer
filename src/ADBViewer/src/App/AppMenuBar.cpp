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

AppMenuBar::~AppMenuBar()
    {
        for (uint32_t i = 0U; i < __NELE(m_icon_menu); i++)
            m_icon_menu[i].Off();
    }

bool AppMenuBar::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = nullptr;
        guiBase::gui.texture = nullptr;

        bool ret = guiBase::initgui(app);

        do
        {
            if (!ret)
                break;

            SDL_Rect rect{};

            rect.w = 0;
            rect.h = 0;
            rect.x = 0;
            rect.y = 0;

            ret = m_icon_menu[0].init(
                    app,
                    rect,
                    ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        return eventcb(ev);
                    }
                );
            if (!ret)
                break;

            ret = m_icon_menu[1].init(
                    app,
                    rect,
                    ResManager::IndexImageResource::RES_IMG_MENU_DISABLED,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        return false;
                    }
                );
            if (!ret)
                break;

            m_icon_menu[0].On();
            m_icon_menu[1].Off();
        }
        while (0);

        return ret;
    }

bool AppMenuBar::isenabled()
    {
        return m_icon_menu[0].guiBase::GetGui<guiRenderer_s>()->active.load();
    }

bool AppMenuBar::iconstate()
    {
        bool check = m_app->m_appeditor.isenabled();
        if (!check)
            check = m_app->m_appinput.isenabled();

        if (check)
        {
            if (!m_icon_idx)
            {
                m_icon_menu[0].Off();
                m_icon_menu[1].On();
                m_icon_idx = 1;
            }
            return false;
        }
        else if (m_icon_idx)
        {
            m_icon_menu[1].Off();
            m_icon_menu[0].On();
            m_icon_idx = 0;
        }
        return true;
    }

bool AppMenuBar::uevent(SDL_Event *ev, const void *instance)
    {
        AppMenuBar *amb = static_cast<AppMenuBar*>(
                const_cast<void*>(instance)
            );

        if (!amb)
            return false;

        switch (ev->user.code)
        {
            /// Change menu enable/disable
            case ID_CMD_POP_MENU102:
                {
                    (void) amb->iconstate();
                    return true;
                }
            /// Install APK
            case ID_CMD_POP_MENU1:
                {
                    (void) amb->mb_cmd_APK(ev);
                    return true;
                }
            /// Stop ADB connect, send event ID_CMD_POP_MENU98
            case ID_CMD_POP_MENU2:
                {
                    int32_t id = amb->mb_cmd_STOP(ev);
                    if (id > 0)
                        amb->guiBase::PushEvent(id);
                    return true;
                }
        }
        return false;
    }

bool AppMenuBar::eventcb(SDL_Event *ev)
{
    /// Cursor normalize, left/right padding
    if ((ev->motion.x < 3) || ((m_icon_menu[0].guiBase::GetGui<SDL_Rect>()->w - ev->motion.x) < 3))
    {
        guiBase::PushEvent(ID_CMD_POP_MENU26);
        return true;
    }

    if (!iconstate())
        return false;

    guiBase::PushEvent(ID_CMD_POP_MENU27);
    ResManager::IndexStringResource res;

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
    switch (ev->motion.y)
    {
        case 5   ... 35:  res = ResManager::IndexStringResource::RES_STR_QUIT; break;
        case 40  ... 70:  res = ResManager::IndexStringResource::RES_STR_START; break;
        case 78  ... 108: res = ResManager::IndexStringResource::RES_STR_STOP; break;
        case 115 ... 145: res = ResManager::IndexStringResource::RES_STR_ADBSET; break;
        case 153 ... 180: res = ResManager::IndexStringResource::RES_STR_SCALE; break;
        case 187 ... 213: res = ResManager::IndexStringResource::RES_STR_POSINFO; break;
        case 222 ... 251: res = ResManager::IndexStringResource::RES_STR_CAPTURE_D; break;
        case 258 ... 288: res = ResManager::IndexStringResource::RES_STR_FULLSCREEN; break;
        case 294 ... 323: res = ResManager::IndexStringResource::RES_STR_APK; break;
        default:          res = ResManager::IndexStringResource::RES_STR_UNKNOWN; break;
    }
#   pragma GCC diagnostic pop

    switch (ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
            {
                int32_t eid = -1;

                switch (res)
                {
                    case ResManager::IndexStringResource::RES_STR_QUIT:
                        {
                            eid = mb_cmd_QUIT(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_START:
                        {
                            eid = mb_cmd_START(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_STOP:
                        {
                            eid = mb_cmd_STOP(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_ADBSET:
                        {
                            eid = mb_cmd_ADBSET(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_SCALE:
                        {
                            eid = mb_cmd_SCALE(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_POSINFO:
                        {
                            eid = mb_cmd_POSINFO(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
                        {
                            eid = mb_cmd_CAPTURE_D(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
                        {
                            eid = mb_cmd_FULLSCREEN(ev);
                            break;
                        }
                    case ResManager::IndexStringResource::RES_STR_APK:
                        {
                            eid = mb_cmd_APK(ev);
                            break;
                        }
                    default:
                        break;
                }
                if (eid < 0)
                    return false;
                else if (eid == 0)
                    return true;
                else if (eid > 0)
                {
                    guiBase::PushEvent(eid);
                    return true;
                }
                break;
            }
        case SDL_MOUSEMOTION:
            {
                switch (res)
                {
                    case ResManager::IndexStringResource::RES_STR_QUIT:
                    case ResManager::IndexStringResource::RES_STR_START:
                    case ResManager::IndexStringResource::RES_STR_STOP:
                    case ResManager::IndexStringResource::RES_STR_ADBSET:
                    case ResManager::IndexStringResource::RES_STR_SCALE:
                    case ResManager::IndexStringResource::RES_STR_POSINFO:
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
                    case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
                    case ResManager::IndexStringResource::RES_STR_APK:
                        {
                            m_app->m_appmsgbar.PrintInfo(
                                MgrType::MGR_MENU,
                                ResManager::stringload(res, AppConfig::instance().cnf_lang),
                                (__LINE__ + res), ev
                            );
                            return true;
                        }
                    default:
                        {
                            m_app->m_appmsgbar.PrintInfo(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_UNKNOWN,
                                    AppConfig::instance().cnf_lang
                                ),
                                __LINE__, ev
                            );
                            guiBase::PushEvent(ID_CMD_POP_MENU26);
                            return false;
                        }
                }
                break;
            }
    }
    return false;
}
