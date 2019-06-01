
#include "../ADBViewer.h"

static inline const LPCSTR l_openBmpFilter = "Bitmap files (*.bmp)\0*.bmp\0";
static inline const LPCSTR l_openBmpExt = "bmp";
static inline const LPCSTR l_openApkFilter = "APK files (*.apk)\0*.apk\0";
static inline const LPCSTR l_openApkExt = "apk";
static inline const LPCSTR l_openCurDir = ".\\";

AppMenuBar::AppMenuBar()
    : m_cursor{nullptr}
{
    gui.rect.h = __H_default;
    gui.rect.w = 32;
    gui.rect.x = 0;
    gui.rect.y = 0;
}

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

    SDL_Surface *l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_MENU
                );
    if (!l_image_surface)
        return false;

    gui.texture = SDL_CreateTextureFromSurface(m_app->m_renderer, l_image_surface);
    SDL_FreeSurface(l_image_surface);
    if (!gui.texture)
        return false;

    if (
        (!(m_cursor[0] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW))) ||
        (!(m_cursor[1] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)))
       )
        return false;

    return initgui(app);
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
        case ID_CMD_POP_MENU4: return ResManager::IndexStringResource::RES_STR_CAPTURE_D;
        case ID_CMD_POP_MENU5: return ResManager::IndexStringResource::RES_STR_CAPTURE_F;
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

    if (ev->type == amb->m_app->m_uevent)
        return amb->mousebutton(ev, amb, ev->user.code);

    if (ev->motion.x > amb->gui.rect.w)
        return false;

    switch(ev->type)
    {
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
        case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
        case ResManager::IndexStringResource::RES_STR_CAPTURE_F:
        case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
        case ResManager::IndexStringResource::RES_STR_APK:
        {
            infoset(
                MgrType::MGR_MENU,
                ResManager::stringload(id),
                (__LINE__ + id), ev
            );
            setcursor(1U);
            return true;
        }
        default:
        {
            infoset(
                MgrType::MGR_MENU,
                ResManager::stringload(ResManager::IndexStringResource::RES_STR_UNKNOWN),
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
                                    ResManager::IndexStringResource::RES_STR_ADBCONNECTED
                                    ),
                                -1, ev
                            );
                            return true;
                        }

                        if (!AppConfig::instance().cnf_adb.IsDeviceID())
                            AppConfig::instance().cnf_adb.GetDeviceListUI();

                        amb->m_app->m_appvideo.run();
                        amb->infoset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBCONNECT
                                ),
                            -1, ev
                        );
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_STOP:
                    {
                        if (AppConfig::instance().cnf_isstop)
                        {
                            amb->infoset(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_ADBDISCONNECTED
                                    ),
                                -1, ev
                            );
                            return true;
                        }
                        amb->m_app->m_appvideo.stop();
                        amb->infoset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBDISCONNECT
                                ),
                            -1, ev
                        );
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_ADBSET:
                    {
                        AppConfig::instance().cnf_adb.GetDeviceSetupUI();
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_SCALE:
                    {
                        amb->m_app->m_appvideo.stop();
                        AppConfig::instance().cnf_scale = ((AppConfig::instance().cnf_scale.load() == 2U) ? 1U : 2U);
                        amb->m_app->m_appvideo.run();
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_POSINFO:
                    {
                        AppConfig::instance().cnf_ispos = !(AppConfig::instance().cnf_ispos);
                        if (!AppConfig::instance().cnf_ispos)
                            amb->infoset(MgrType::MGR_MENU, "", -1, ev);
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_D:
                    {
                        amb->screenshot(ev, false);
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE_F:
                    {
                        amb->screenshot(ev, true);
                        return true;
                    }
                    case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
                    {
                        SDL_SetWindowFullscreen(
                            amb->m_app->m_window,
                            ((AppConfig::instance().cnf_isfullscreen) ? 0U : SDL_WINDOW_FULLSCREEN)
                        );
                        if (AppConfig::instance().cnf_isfullscreen)
                        {
                            SDL_SetWindowSize(
                                    amb->m_app->m_window,
                                    __W_default + amb->gui.rect.w,
                                    __H_default
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
    }
    return false;
}

bool AppMenuBar::screenshot(SDL_Event *ev, bool isdialog)
{
    if (!m_app)
        return false;

    std::string fname(
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_CAPFILENAME
            )
        );

#   if !defined(OS_WIN)
    isdialog = false;
#   endif

    if (isdialog)
    {
        if (!AppSysDialog::savefile(m_app->m_window, fname, l_openBmpFilter, l_openBmpExt, l_openCurDir))
            return false;
    }
    else
    {
        std::stringstream ss;
        ss << fname << std::to_string(time(NULL)) << ".bmp";
        fname.assign(ss.str().c_str());
    }

    SDL_Surface *l_ss_surface = SDL_CreateRGBSurface(
                0,
                m_app->gui.rect.w,
                m_app->gui.rect.h,
                32,
                0x00ff0000,
                0x0000ff00,
                0x000000ff,
                0xff000000
            );

    if (!l_ss_surface)
        return false;

    SDL_RenderReadPixels(
            m_app->m_renderer,
            &m_app->gui.rect,
            SDL_GetWindowPixelFormat(m_app->m_window),
            l_ss_surface->pixels,
            l_ss_surface->pitch
        );

    SDL_SaveBMP(l_ss_surface, fname.c_str());
    SDL_FreeSurface(l_ss_surface);

    std::stringstream ss;
    ss << ResManager::stringload(ResManager::IndexStringResource::RES_STR_FILESAVE);
    ss << fname.c_str();
    infoset(MgrType::MGR_MENU, ss.str(), -1, ev);
    return true;
}

void AppMenuBar::infoset(MgrType mgrt, std::string const & s, int32_t id, SDL_Event *ev)
{
    std::stringstream ss;
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
#   else
    ss << ResManager::stringload(
            ResManager::IndexStringResource::RES_STR_APPTITLENAME
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
                    uint32_t x = ((AppConfig::instance().cnf_scale) ?
                        ((ev->motion.x - gui.rect.w) * AppConfig::instance().cnf_scale) :
                         (ev->motion.x - gui.rect.w)
                        );
                    uint32_t y = ((AppConfig::instance().cnf_scale) ?
                        (ev->motion.y * AppConfig::instance().cnf_scale) :
                         ev->motion.y
                        );
                    uint32_t w = ((AppConfig::instance().cnf_scale) ?
                        (m_app->m_appvideo.gui.rect.w * AppConfig::instance().cnf_scale) :
                         m_app->m_appvideo.gui.rect.w
                        );
                    uint32_t h = ((AppConfig::instance().cnf_scale) ?
                        (m_app->m_appvideo.gui.rect.h * AppConfig::instance().cnf_scale) :
                         m_app->m_appvideo.gui.rect.h
                        );
#                   if !defined (_BUILD_FRAME_NO_TITLE)
                    ss << " - ";
#                   endif
                    ss << "( " << w << "x" << h << " )";
                    ss << " - " << "( X: " << x << " Y: " << y << " )";

                    /*
                    if (m_app->m_appeditor.isactive())
                    {
                        ss << " - (R:" << m_app->m_appeditor.m_pixel.rgb.r_;
                        ss << " G:" << m_app->m_appeditor.m_pixel.rgb.g_;
                        ss << " B:" << m_app->m_appeditor.m_pixel.rgb.b_ << ") ";
                    }
                    */
                }
#               if defined (_BUILD_FRAME_NO_TITLE)
                else
                {
                    m_app->m_appinfo.clear();
                    return;
                }
#               endif
            }
            else
            {
#               if defined (_BUILD_FRAME_NO_TITLE)
                m_app->m_appinfo.clear();
#               endif
                return;
            }
            break;
        }
        case MgrType::MGR_MENU:
        {
#           if defined (_BUILD_FRAME_NO_TITLE)
            if (s.empty())
            {
                m_app->m_appinfo.clear();
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
#               if defined (_BUILD_FRAME_NO_TITLE)
                m_app->m_appinfo.clear();
#               endif
                return;
            }
    }
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
    SDL_Point offset = { (gui.rect.w + 1), 0 };
    m_app->m_appinfo.draw(ss.str(), &offset, id);
#   else
    settitle(ss.str());
#   endif

}

void AppMenuBar::settitle(std::string const & s)
{
    SDL_SetWindowTitle(m_app->m_window, s.c_str());
}


