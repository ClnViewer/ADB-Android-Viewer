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

static inline const LPCSTR l_openApkFilter = "APK files (*.apk)\0*.apk\0";
static inline const LPCSTR l_openApkExt = "apk";
static inline const LPCSTR l_openCurDir = ".\\";

AppMenuBar::AppMenuBar()
    : m_cursor{nullptr} { guiBase::gui.rect = {}; }

AppMenuBar::~AppMenuBar()
{
    if (m_cursor[0])
        SDL_FreeCursor(m_cursor[0]);
    if (m_cursor[1])
        SDL_FreeCursor(m_cursor[1]);

    m_cursor[0] = nullptr;
    m_cursor[1] = nullptr;
}

bool AppMenuBar::init(App *app)
{
    if (!app)
        return false;

    m_app = app;

    if (
        (!(m_cursor[0] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW))) ||
        (!(m_cursor[1] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)))
       )
        return false;

    return initgui(app);
}

bool AppMenuBar::tinit(SDL_Texture **texture)
    {
        SDL_Surface *l_image_surface_img = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_MENU
                );
        if (!l_image_surface_img)
            return false;

        guiBase::gui.rect.w = l_image_surface_img->w;
        guiBase::gui.rect.h = l_image_surface_img->h;
        guiBase::gui.rect.x = 0;
        guiBase::gui.rect.y = 0;

        SDL_Texture *l_texture = SDL_CreateTextureFromSurface(
                m_app->m_renderer,
                l_image_surface_img
            );
        SDL_FreeSurface(l_image_surface_img);

        if (!l_texture)
            return false;

        GuiLock(
            std::swap(*texture, l_texture);
        );

        if (l_texture)
            SDL_DestroyTexture(l_texture);


        return true;
    }

bool AppMenuBar::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        if (tinit(texture))
            guiBase::ActiveOn();
        return guiBase::IsActive();
    }

void AppMenuBar::setcursor(uint32_t id)
{
    if (m_cursor[id])
        SDL_SetCursor(m_cursor[id]);
}

ResManager::IndexStringResource AppMenuBar::clickpos(int32_t d, int32_t w, int32_t h, int32_t ucode)
{
    /// Custom user event (POPUPMENU)
    switch(ucode)
    {
        case ID_CMD_POP_MENU1: return ResManager::IndexStringResource::RES_STR_APK;
        case ID_CMD_POP_MENU2: return ResManager::IndexStringResource::RES_STR_STOP;
        default:
            break;
    }

    /// Cursor normalize, left/right padding
    if ((w < 3) || ((d - w) < 3))
        return ResManager::IndexStringResource::RES_STR_UNKNOWN;

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
    switch (h)
    {
        case 5   ... 35:  return ResManager::IndexStringResource::RES_STR_QUIT;
        case 40  ... 70:  return ResManager::IndexStringResource::RES_STR_START;
        case 78  ... 108: return ResManager::IndexStringResource::RES_STR_STOP;
        case 115 ... 145: return ResManager::IndexStringResource::RES_STR_ADBSET;
        case 153 ... 180: return ResManager::IndexStringResource::RES_STR_SCALE;
        case 187 ... 213: return ResManager::IndexStringResource::RES_STR_POSINFO;
        case 222 ... 251: return ResManager::IndexStringResource::RES_STR_CAPTURE_D;
        case 258 ... 288: return ResManager::IndexStringResource::RES_STR_FULLSCREEN;
        case 294 ... 323: return ResManager::IndexStringResource::RES_STR_APK;
        default:          return ResManager::IndexStringResource::RES_STR_UNKNOWN;
    }
#   pragma GCC diagnostic pop
}

bool AppMenuBar::event(SDL_Event *ev, const void *instance)
{
    AppMenuBar *amb = static_cast<AppMenuBar*>(
                const_cast<void*>(instance)
            );

    if (amb->m_app->m_appeditor.isactive())
        return false;

    if (ev->type == AppConfig::instance().cnf_uevent)
        return amb->mousebutton(ev, amb, ev->user.code);

    if (ev->motion.x > amb->gui.rect.w)
        return false;

    switch(ev->type)
    {
        case SDL_RENDER_TARGETS_RESET:
        case SDL_RENDER_DEVICE_RESET:
            {
                (void) amb->evresize(&amb->gui.texture);
                return false;
            }
        case SDL_MOUSEBUTTONDOWN:
            {
                return amb->mousebutton(ev, amb, -1);
            }
        case SDL_MOUSEMOTION:
            {
                return amb->mousemove(ev, amb);
            }
        default:
            break;
    }
    return false;
}

bool AppMenuBar::mousemove(SDL_Event *ev, AppMenuBar *amb)
{
    ResManager::IndexStringResource id;
    switch((id = amb->clickpos(
                    amb->gui.rect.w,
                    ev->motion.x,
                    ev->motion.y,
                    -1)))
    {
        case ResManager::IndexStringResource::RES_STR_QUIT:
        case ResManager::IndexStringResource::RES_STR_START:
        case ResManager::IndexStringResource::RES_STR_STOP:
        case ResManager::IndexStringResource::RES_STR_ADBSET:
        case ResManager::IndexStringResource::RES_STR_SCALE:
        case ResManager::IndexStringResource::RES_STR_POSINFO:
        case ResManager::IndexStringResource::RES_STR_CAPTURE_C:
        case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
        case ResManager::IndexStringResource::RES_STR_CAPTURE_F:
        case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
        case ResManager::IndexStringResource::RES_STR_APK:
        {
            infoset(
                MgrType::MGR_MENU,
                ResManager::stringload(id, AppConfig::instance().cnf_lang),
                (__LINE__ + id), ev
            );
            amb->setcursor(1U);
            return true;
        }
        default:
        {
            infoset(
                MgrType::MGR_MENU,
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_UNKNOWN,
                    AppConfig::instance().cnf_lang
                ),
                __LINE__, ev
            );
            amb->setcursor(0U);
            break;
        }
    }
    return false;
}

bool AppMenuBar::mousebutton(SDL_Event *ev, AppMenuBar *amb, int32_t ucode)
{
    if ((ucode > 0) || (ev->button.button == SDL_BUTTON_LEFT))
    {
        SDL_Event cmdEvent{};
        cmdEvent.type = AppConfig::instance().cnf_uevent;
        cmdEvent.user.code = 0;

                switch(amb->clickpos(
                        amb->gui.rect.w,
                        ev->motion.x,
                        ev->motion.y,
                        ucode))
                {
                    case ResManager::IndexStringResource::RES_STR_QUIT:
                    {
                        AppConfig::instance().cnf_isrun = false;
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_START:
                    {
                        if (!AppConfig::instance().cnf_isstop)
                        {
                            amb->infoset(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_ADBCONNECTED,
                                    AppConfig::instance().cnf_lang
                                    ),
                                -1, ev
                            );
                            return true;
                        }

                        if (!AppConfig::instance().cnf_adb.IsDeviceID())
                            AppConfig::instance().cnf_adb.GetDeviceListUI();

                        amb->infoset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBCONNECT,
                                AppConfig::instance().cnf_lang
                                ),
                            -1, ev
                        );
                        cmdEvent.user.code = ID_CMD_POP_MENU97;
                        amb->setcursor(0U);
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_STOP:
                    {
                        if (AppConfig::instance().cnf_isstop)
                        {
                            amb->infoset(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_ADBDISCONNECTED,
                                    AppConfig::instance().cnf_lang
                                    ),
                                -1, ev
                            );
                            return true;
                        }
                        amb->infoset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBDISCONNECT,
                                AppConfig::instance().cnf_lang
                                ),
                            -1, ev
                        );
                        cmdEvent.user.code = ID_CMD_POP_MENU98;
                        amb->setcursor(0U);
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_ADBSET:
                    {
                        AppConfig::instance().cnf_adb.GetDeviceSetupUI();
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_SCALE:
                    {
                        AppConfig::instance().cnf_disp_ratio = ((AppConfig::instance().cnf_disp_ratio.load() >= 2U) ? 1U : 2U);
                        cmdEvent.user.code = ID_CMD_POP_MENU99;
                        amb->setcursor(0U);
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_POSINFO:
                    {
                        AppConfig::instance().cnf_ispos = !(AppConfig::instance().cnf_ispos);
                        if (!AppConfig::instance().cnf_ispos)
                            amb->infoset(MgrType::MGR_MENU, "", -1, ev);
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_C:
                    {
                        cmdEvent.user.code = ID_CMD_POP_MENU22;
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
                    {
                        cmdEvent.user.code = ID_CMD_POP_MENU4;
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_F:
                    {
                        cmdEvent.user.code = ID_CMD_POP_MENU5;
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
                    {
                        SDL_SetWindowFullscreen(
                            amb->m_app->m_window,
                            ((AppConfig::instance().cnf_isfullscreen) ? 0U : SDL_WINDOW_FULLSCREEN_DESKTOP)
                        );
                        if (AppConfig::instance().cnf_isfullscreen)
                        {
                            SDL_SetWindowSize(
                                    amb->m_app->m_window,
                                    AppConfig::instance().cnf_disp_point.x,
                                    AppConfig::instance().cnf_disp_point.y
                                );
                            SDL_SetWindowPosition(
                                    amb->m_app->m_window,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED
                                );
                        }
                        AppConfig::instance().cnf_isfullscreen = !(AppConfig::instance().cnf_isfullscreen);
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_APK:
                    {
                        std::string fname;
                        if (AppSysDialog::openfile(m_app->m_window, fname, l_openApkFilter, l_openApkExt, l_openCurDir))
                            AppConfig::instance().cnf_adb.InstallApk(fname);
                        return true;
                    }
                    default:
                    {
                        break;
                    }
                }
                if (cmdEvent.user.code)
                {
                    SDL_PushEvent(&cmdEvent);
                    return true;
                }
    }
    return false;
}

void AppMenuBar::infoset(MgrType mgrt, std::string const & s, int32_t id, SDL_Event *ev)
{
    std::stringstream ss;
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
#   else
    ss << ResManager::stringload(
            ResManager::IndexStringResource::RES_STR_APPTITLENAME,
            AppConfig::instance().cnf_lang
        );
    ss << "v." << AVIEW_FULLVERSION_STRING;
    ss << " r." << AVIEW_SVN_REVISION;
#   endif

    switch(mgrt)
    {
        case MgrType::MGR_MAIN:
        {
            if (ev->motion.x > gui.rect.w)
            {
                if ((!AppConfig::instance().cnf_isstop) && (AppConfig::instance().cnf_ispos))
                {
                    uint32_t x = ((AppConfig::instance().cnf_disp_ratio) ?
                        ((ev->motion.x - gui.rect.w) * AppConfig::instance().cnf_disp_ratio) :
                         (ev->motion.x - gui.rect.w)
                        );
                    uint32_t y = ((AppConfig::instance().cnf_disp_ratio) ?
                        (ev->motion.y * AppConfig::instance().cnf_disp_ratio) :
                         ev->motion.y
                        );
                    uint32_t w = ((AppConfig::instance().cnf_disp_ratio) ?
                        (m_app->m_appvideo.gui.rect.w * AppConfig::instance().cnf_disp_ratio) :
                         m_app->m_appvideo.gui.rect.w
                        );
                    uint32_t h = ((AppConfig::instance().cnf_disp_ratio) ?
                        (m_app->m_appvideo.gui.rect.h * AppConfig::instance().cnf_disp_ratio) :
                         m_app->m_appvideo.gui.rect.h
                        );
#                   if !defined (_BUILD_FRAME_NO_TITLE)
                    ss << " - ";
#                   endif
                    ss << "( " << w << "x" << h << " )";
                    ss << " - " << "( X: " << x << " Y: " << y << " )";
                }
#               if defined (_BUILD_FRAME_NO_TITLE)
                else
                {
                    m_app->m_appmsgbar.clear();
                    return;
                }
#               endif
            }
            else
            {
//#               if defined (_BUILD_FRAME_NO_TITLE)
                //m_app->m_appmsgbar.clear();
//#               endif
                return;
            }
            break;
        }
        case MgrType::MGR_MENU:
        {
#           if defined (_BUILD_FRAME_NO_TITLE)
            if (s.empty())
            {
                m_app->m_appmsgbar.clear();
                return;
            }
            ss << s.c_str();
#           else
            if (!s.empty())
                ss << " - " << s.c_str();
#           endif
            break;
        }
        default:
            {
//#               if defined (_BUILD_FRAME_NO_TITLE)
                //m_app->m_appmsgbar.clear();
//#               endif
                return;
            }
    }
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
    AddMessageQueue(
        ss.str(),
        3U,
        id
    );
#   else
    settitle(ss.str());
#   endif

}

void AppMenuBar::settitle(std::string const & s)
{
    SDL_SetWindowTitle(m_app->m_window, s.c_str());
}


