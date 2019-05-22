
#include "../ADBViewer.h"

static void rect_default(ADBDriver::Swipe_s *adbr, SDL_Rect *sdlr)
{
    adbr->x0 = adbr->x1 = static_cast<uint32_t>(sdlr->w / 2);
    adbr->y0 = adbr->y1 = static_cast<uint32_t>(sdlr->h / 2);
}

static SDL_HitTestResult SDLCALL f_hitTest(SDL_Window *win, const SDL_Point *pt, void *data)
{
    SDL_Rect *rmenu = static_cast<SDL_Rect*>(data);
    if (
        (pt->x <= rmenu->w) &&
        (pt->y >= __MENU_H_dragable)
       )
        return SDL_HITTEST_DRAGGABLE;
    return SDL_HITTEST_NORMAL;
}

App::App()
    : m_adb_rect({}), m_adbinit(false)
{
    gui.rect.w = __W_default;
    gui.rect.h = __H_default;
    gui.rect.x = m_appmenubar.gui.rect.w;
    gui.rect.y = 0;
    m_adb_rect.t  = 100U;
    m_pinput = { 50, (__H_default - 40) };

    switch (initm(__W_default + m_appmenubar.gui.rect.w, __H_default))
    {
        case 1:
        case 2:
        case 3:
        case 4:
            {
                SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_APP
                    ),
                    SDL_GetError(),
                    nullptr
                );
                return;
            }
        default:
            {
                break;
            }
    }

    /// initiate GUI instance
    gui.texture = SDL_CreateTexture(
                m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                __W_default, __H_default
            );

    if (
        (!gui.texture)   ||           /// this main texture
        (!initgui(this)) ||           /// this main GUI screen
        (!m_appmenubar.init(this)) || /// this menu screen
        (!m_info.init(
                this,
                ResManager::IndexFontResource::RES_FONT_16704,
                ResManager::IndexColorResource::RES_COLOR_GREEN_BLACK
            )) ||                /// info-help left rectangle
        (!m_input.init(
                this,
                ResManager::IndexFontResource::RES_FONT_FREESANS,
                ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE
            ))                   /// keyboard input rectangle
    )
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP
            ),
            SDL_GetError(),
            m_window
        );
        return;
    }

    SDL_Surface *l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_LOGO
                );
    if (!l_image_surface)
        return;

    SDL_SetColorKey(l_image_surface, SDL_TRUE, SDL_MapRGB(l_image_surface->format, 255, 255, 255));
    SDL_SetWindowIcon(m_window, l_image_surface);
    SDL_FreeSurface(l_image_surface);

    if (SDL_SetWindowHitTest(m_window, f_hitTest, &m_appmenubar.gui.rect) < 0)
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP
            ),
            SDL_GetError(),
            m_window
        );
        return;
    }

    logo();
}

App::~App()
{
    jointh();
}

void App::jointh()
{
    if (m_thu.joinable())
        m_thu.join();
}

void App::logo()
{
    SDL_Surface *l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_SCREEN
                );
    if (!l_image_surface)
        return;

    SDL_UpdateTexture(gui.texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
    SDL_FreeSurface(l_image_surface);
}

void App::run()
{
    rect_default(&m_adb_rect, &gui.rect);

    if (!m_adbinit)
    {
        if (!m_adb.InitRemote())
        {
            SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_ADRV
                    ),
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ADBINSTALLBIN
                    ),
                    m_window
                );
            return;
        }
        m_adbinit = true;
    }

    std::thread thu
    {
        [=]()
        {
            try
            {
                (void) m_adb.Video(
                        AppConfig::instance().cnf_scale.load(), AppConfig::instance().cnf_compress.load(),
                        [this](std::vector<uint8_t> & v, uint32_t w, uint32_t h)
                            {
                                return update(v, w, h);
                            }
                        );
            }
            catch(std::exception & _ex)
            {
                SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_ADRV
                    ),
                    _ex.what(),
                    m_window
                );
                return;
            }
            catch(...)
            {
                SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_ADRV
                    ),
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN
                    ),
                    m_window
                );
                return;
            }
        }
    };
    m_thu = move(thu);
    return;
}

void App::loop()
{
    AppConfig::instance().cnf_isrun = true;
    while (AppConfig::instance().cnf_isrun.load())
    {
        while (SDL_PollEvent(&m_event) > 0)
            events(&m_event, &m_pinput);

        if (!AppConfig::instance().cnf_isrun.load())
            break;

        draw();
    }
}

bool App::event(SDL_Event *ev, SDL_Point *pt, const void *instance)
{
    App *app = (App*)instance;

    switch(ev->type)
    {
        case SDL_QUIT:
        {
            AppConfig::instance().cnf_isrun = false;
            return true;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            if (
                (AppConfig::instance().cnf_isstop) ||
                (ev->motion.x <= app->m_appmenubar.gui.rect.w)
               )
                break;

            switch (ev->button.button)
            {
                case SDL_BUTTON_LEFT:
                {
                    ADBDriver::Tap_s t =
                    {
                        ((ev->motion.x - app->m_appmenubar.gui.rect.w) *
                                static_cast<int32_t>(AppConfig::instance().cnf_scale.load())),
                        (ev->motion.y *
                                static_cast<int32_t>(AppConfig::instance().cnf_scale.load()))
                    };
                    app->m_adb.Click(&t);
                    return true;
                }
                case SDL_BUTTON_RIGHT:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEMOTION:
        {
            if (
                (AppConfig::instance().cnf_isstop) ||
                (ev->motion.x <= app->m_appmenubar.gui.rect.w)
               )
                break;

            app->m_appmenubar.infoset(MgrType::MGR_MAIN, "", -1, ev);
            return true;
        }

        case SDL_KEYDOWN:
        {
            switch (ev->key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    if (AppConfig::instance().cnf_isstop)
                    {
                        AppConfig::instance().cnf_isrun = false;
                        return true;
                    }
                    break;
                }
                case SDLK_RETURN:
                case SDLK_RETURN2:
                {
                    if (AppConfig::instance().cnf_isstop)
                        break;

                    if (!app->m_input.isactive())
                    {
                        app->m_input.begin(
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ENTER_TEXT
                            ),
                            &app->m_pinput
                        );
                    }
                    else
                    {
                        if (app->m_input.isresult())
                            app->m_adb.SendTextASCII(app->m_input.getresult());
                            /// No UTF8 !! Cyrillic and other national language
                    }
                    return true;
                }
                default:
                    break;
            }

            ///
            /// Swipe from key:
            /// SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN,
            /// SDLK_HOME, SDLK_PAGEDOWN, SDLK_PAGEUP
            ///

            if (
                (AppConfig::instance().cnf_isstop) ||
                (app->m_input.isactive())
               )
                break;

            switch (ev->key.keysym.sym)
            {
                case SDLK_LEFT:
                {
                    app->m_adb_rect.x1 = std::min((app->m_adb_rect.x1 + 30), app->gui.rect.w);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.x0 = app->m_adb_rect.x1;
                    return true;
                }
                case SDLK_RIGHT:
                {
                    app->m_adb_rect.x1 = std::max((app->m_adb_rect.x1 - 30), 0);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.x0 = app->m_adb_rect.x1;
                    return true;
                }
                case SDLK_UP:
                {
                    app->m_adb_rect.y1 = std::min((app->m_adb_rect.y1 + 30), app->gui.rect.h);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.y0 = app->m_adb_rect.y1;
                    return true;
                }
                case SDLK_DOWN:
                {
                    app->m_adb_rect.y1 = std::max((app->m_adb_rect.y1 - 30), 0);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.y0 = app->m_adb_rect.y1;
                    return true;
                }
                case SDLK_HOME:
                {
                    app->m_adb_rect.x1 = static_cast<uint32_t>(app->gui.rect.w / 2);
                    app->m_adb_rect.y1 = static_cast<uint32_t>(app->gui.rect.h / 2);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.x0 = app->m_adb_rect.x1;
                    app->m_adb_rect.y0 = app->m_adb_rect.y1;
                    return true;
                }
                case SDLK_PAGEDOWN:
                {
                    app->m_adb_rect.y1 = 0;
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.y0 = app->m_adb_rect.y1;
                    return true;
                }
                case SDLK_PAGEUP:
                {
                    app->m_adb_rect.y1 = static_cast<uint32_t>(app->gui.rect.h);
                    app->m_adb.Click(&app->m_adb_rect);
                    app->m_adb_rect.y0 = app->m_adb_rect.y1;
                    return true;
                }
                default:
                {
                    app->m_adb.SendSpecialKey(
                        GameDev::ADBDriver::KeysType::KEYS_SDL,
                            ev->key.keysym.scancode
                    );
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

bool App::update(std::vector<uint8_t> & v, uint32_t w, uint32_t h) noexcept
{
    if (!v.size())
        return AppConfig::instance().cnf_isrun.load();

    gui.active = true;
    void *pix = nullptr;
    int32_t pitch = 0,
            iw = static_cast<int32_t>(w),
            ih = static_cast<int32_t>(h);

    if (
        (gui.rect.w != iw) ||
        (gui.rect.h != ih)
       )
    {
        SDL_Texture *t_texture = SDL_CreateTexture(
                m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                iw, ih
            );
        if (!t_texture)
            return false;

        std::swap(gui.texture, t_texture);
        SDL_DestroyTexture(t_texture);

        gui.rect.w = iw;
        gui.rect.h = ih;
        gui.rect.x = m_appmenubar.gui.rect.w;
        gui.rect.y = 0;

        SDL_SetWindowSize(
            m_window,
            (iw + m_appmenubar.gui.rect.w),
            ih
            );
    }

    SDL_LockTexture(gui.texture, nullptr, &pix, &pitch);
    if ((!pix) || (!pitch))
        return false;

    ::memcpy(pix, &v[0], v.size());

    SDL_UnlockTexture(gui.texture);
    return ((AppConfig::instance().cnf_isstop.load()) ? false : ((AppConfig::instance().cnf_isrun.load()) ? true : false));
}
