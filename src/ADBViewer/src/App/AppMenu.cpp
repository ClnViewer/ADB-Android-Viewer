
#include "../ADBViewer.h"

AppMenu::AppMenu()
    : m_menu_texture(nullptr), m_menu_rect({}),
      m_isrun(true), m_isstop(true), m_ispos(false), m_isfullscreen(false),
      m_scale(2U), m_compress(9U),
      m_cursor{nullptr}
{
    m_menu_rect.h = __H_default;
    m_menu_rect.w = 32;
    m_menu_rect.x = 0;
    m_menu_rect.y = 0;
}

AppMenu::~AppMenu()
{
    SDL_DestroyTexture(m_menu_texture);
    if (m_cursor[0])
        SDL_FreeCursor(m_cursor[0]);
    if (m_cursor[1])
        SDL_FreeCursor(m_cursor[1]);
    m_cursor[0] = nullptr;
    m_cursor[1] = nullptr;
}

bool AppMenu::init(App *app)
{
    if (!app)
        return false;

    m_app = app;

    SDL_Surface *l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_MENU
                );
    if (!l_image_surface)
        return false;

    m_menu_texture = SDL_CreateTextureFromSurface(m_app->m_window_renderer, l_image_surface);
    SDL_FreeSurface(l_image_surface);
    if (!m_menu_texture)
        return false;

    if (
        (!(m_cursor[0] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW))) ||
        (!(m_cursor[1] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)))
       )
        return false;

    return true;
}

void AppMenu::setcursor(uint32_t id)
{
    if (m_cursor[id])
        SDL_SetCursor(m_cursor[id]);
}

ResManager::IndexStringResource AppMenu::clickpos(int32_t d, int32_t w, int32_t h)
{
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
        case 222 ... 251: return ResManager::IndexStringResource::RES_STR_CAPTURE;
        case 258 ... 288: return ResManager::IndexStringResource::RES_STR_FULLSCREEN;
        case 294 ... 323: return ResManager::IndexStringResource::RES_STR_APK;
        default:          return ResManager::IndexStringResource::RES_STR_UNKNOWN;
    }
#   pragma GCC diagnostic pop
}

bool AppMenu::mouse()
{
    if (
        (!m_app) ||
        (m_app->m_window_event.motion.x > m_menu_rect.w)
       )
        return false;

    switch(m_app->m_window_event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            {
                mousebutton();
                break;
            }
        case SDL_MOUSEMOTION:
            {
                mousemove();
                break;
            }
        default:
            break;
    }
    return true;
}

void AppMenu::mousemove()
{
    ResManager::IndexStringResource id;
    switch((id = clickpos(
                    m_menu_rect.w,
                    m_app->m_window_event.motion.x,
                    m_app->m_window_event.motion.y)))
    {
        case ResManager::IndexStringResource::RES_STR_QUIT:
        case ResManager::IndexStringResource::RES_STR_START:
        case ResManager::IndexStringResource::RES_STR_STOP:
        case ResManager::IndexStringResource::RES_STR_ADBSET:
        case ResManager::IndexStringResource::RES_STR_SCALE:
        case ResManager::IndexStringResource::RES_STR_POSINFO:
        case ResManager::IndexStringResource::RES_STR_CAPTURE:
        case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
        case ResManager::IndexStringResource::RES_STR_APK:
        {
#           if !defined (_BUILD_FRAME_NO_TITLE)
            titleset(
                MgrType::MGR_MENU,
                ResManager::stringload(id)
            );
#           endif
            setcursor(1U);
            break;
        }
        default:
        {
#           if !defined (_BUILD_FRAME_NO_TITLE)
            titleset(
                MgrType::MGR_MENU,
                ResManager::stringload(ResManager::IndexStringResource::RES_STR_UNKNOWN)
            );
#           endif
            setcursor(0U);
            break;
        }
    }
}

void AppMenu::mousebutton()
{
    switch (m_app->m_window_event.button.button)
    {
        case SDL_BUTTON_LEFT:
            {
                switch(clickpos(
                        m_menu_rect.w,
                        m_app->m_window_event.motion.x,
                        m_app->m_window_event.motion.y))
                {
                    case ResManager::IndexStringResource::RES_STR_QUIT:
                    {
                        m_isrun = false;
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_START:
                    {
                        if (!m_isstop)
                        {
#                           if !defined (_BUILD_FRAME_NO_TITLE)
                            titleset(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_ADBCONNECTED
                                    )
                            );
#                           endif
                            break;
                        }

                        if (!m_app->m_adb.IsDeviceID())
                            m_app->m_adb.GetDeviceListUI();

                        m_isstop = false;
                        m_app->run();
#                       if !defined (_BUILD_FRAME_NO_TITLE)
                        titleset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBCONNECT
                                )
                        );
#                       endif
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_STOP:
                    {
                        if (m_isstop)
                        {
#                           if !defined (_BUILD_FRAME_NO_TITLE)
                            titleset(
                                MgrType::MGR_MENU,
                                ResManager::stringload(
                                    ResManager::IndexStringResource::RES_STR_ADBDISCONNECTED
                                    )
                            );
#                           endif
                            break;
                        }
                        m_isstop = true;
                        m_app->jointh();
                        m_app->screen();
#                       if !defined (_BUILD_FRAME_NO_TITLE)
                        titleset(
                            MgrType::MGR_MENU,
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ADBDISCONNECT
                                )
                        );
#                       endif
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_ADBSET:
                    {
                        m_app->m_adb.GetDeviceSetupUI();
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_SCALE:
                    {
                        m_isstop = true;
                        m_app->jointh();
                        m_scale = ((m_scale.load() == 2U) ? 1U : 2U);
                        m_isstop = false;
                        m_app->run();
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_POSINFO:
                    {
#                       if !defined (_BUILD_FRAME_NO_TITLE)
                        m_ispos = !(m_ispos);
                        if (!m_ispos)
                            titleset(MgrType::MGR_MENU, "");
#                       endif
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_CAPTURE:
                    {
                        screenshot();
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_FULLSCREEN:
                    {
                        SDL_SetWindowFullscreen(
                            m_app->m_window,
                            ((m_isfullscreen) ? 0U : SDL_WINDOW_FULLSCREEN)
                        );
                        if (m_isfullscreen)
                        {
                            SDL_SetWindowSize(
                                    m_app->m_window,
                                    __W_default + m_menu_rect.w,
                                    __H_default
                                );
                            SDL_SetWindowPosition(
                                    m_app->m_window,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED
                                );
                        }
                        m_isfullscreen = !(m_isfullscreen);
                        break;
                    }
                    case ResManager::IndexStringResource::RES_STR_APK:
                    {
                        std::string fname;
                        if (openfile(fname))
                            m_app->m_adb.InstallApk(fname);

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                break;
            }
        case SDL_BUTTON_RIGHT:
            {
                SDL_ShowSimpleMessageBox(0, "Mouse", "MENU Right button was pressed!", nullptr);
                break;
            }
        default:
            {
                break;
            }
    }
}

bool AppMenu::screenshot()
{
    if (!m_app)
        return false;

    std::string fname(
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_CAPFILENAME
            )
        );
    if (!savefile(fname))
        return false;

    SDL_Surface *l_ss_surface = SDL_CreateRGBSurface(
                0,
                m_app->m_main_rect.w,
                m_app->m_main_rect.h,
                32,
                0x00ff0000,
                0x0000ff00,
                0x000000ff,
                0xff000000
            );

    if (!l_ss_surface)
        return false;

    SDL_RenderReadPixels(
            m_app->m_window_renderer,
            &m_app->m_main_rect,
            SDL_GetWindowPixelFormat(m_app->m_window),
            l_ss_surface->pixels,
            l_ss_surface->pitch
        );

    SDL_SaveBMP(l_ss_surface, fname.c_str());
    SDL_FreeSurface(l_ss_surface);

    std::stringstream ss;
    ss << ResManager::stringload(ResManager::IndexStringResource::RES_STR_FILESAVE);
    ss << fname.c_str();
#   if !defined (_BUILD_FRAME_NO_TITLE)
    titleset(MgrType::MGR_MENU, ss.str());
#   endif
    return true;
}

#if !defined (_BUILD_FRAME_NO_TITLE)
void AppMenu::titleset(MgrType mgrt, std::string const & s)
{
    std::stringstream ss;
    ss << ResManager::stringload(
            ResManager::IndexStringResource::RES_STR_APPTITLENAME
        );
    ss << "v." << AVIEW_FULLVERSION_STRING;
    ss << " r." << AVIEW_SVN_REVISION;

    switch(mgrt)
    {
        case MgrType::MGR_MAIN:
        {
            if (m_app->m_window_event.motion.x > m_menu_rect.w)
            {
                if ((!m_isstop) && (m_ispos))
                {
                    uint32_t x = ((m_scale) ?
                        ((m_app->m_window_event.motion.x - m_menu_rect.w) * m_scale) :
                         (m_app->m_window_event.motion.x - m_menu_rect.w)
                        );
                    uint32_t y = ((m_scale) ?
                        (m_app->m_window_event.motion.y * m_scale) :
                         m_app->m_window_event.motion.y
                        );
                    uint32_t w = ((m_scale) ?
                        (m_app->m_main_rect.w * m_scale) :
                         m_app->m_main_rect.w
                        );
                    uint32_t h = ((m_scale) ?
                        (m_app->m_main_rect.h * m_scale) :
                         m_app->m_main_rect.h
                        );
                    ss << " - " << "( " << w << "x" << h << " )";
                    ss << " - " << "( X: " << x << " Y: " << y << " )";
                }
            }
            else
                return;
            break;
        }
        case MgrType::MGR_MENU:
        {
            if (s.length())
                ss << " - " << s.c_str();
            break;
        }
    }
    SDL_SetWindowTitle(m_app->m_window, ss.str().c_str());
}
#endif

