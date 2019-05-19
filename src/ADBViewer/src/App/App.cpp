
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
    m_main_rect.w = __W_default;
    m_main_rect.h = __H_default;
    m_main_rect.x = m_appmenu.m_menu_rect.w;
    m_main_rect.y = 0;
    m_adb_rect.t  = 100U;
#   if defined (_BUILD_FRAME_NO_TITLE)
    uint32_t wflag = SDL_WINDOW_BORDERLESS;
#   else
    uint32_t wflag = 0U;
#   endif

    m_window = SDL_CreateWindow(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_APPFULLNAME
                ),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                __W_default + m_appmenu.m_menu_rect.w, __H_default,
                wflag
            );
    if (!m_window)
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

    m_window_renderer = SDL_CreateRenderer(
                m_window,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );

    if (!m_window_renderer)
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

    if (!m_appmenu.init(this))
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

    m_texture = SDL_CreateTexture(
                m_window_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                __W_default, __H_default
            );

    if (!m_texture)
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

    if (SDL_SetWindowHitTest(m_window, f_hitTest, &m_appmenu.m_menu_rect) < 0)
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

    screen();
}

App::~App()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_window_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    if (m_thu.joinable())
        m_thu.join();
}

void App::jointh()
{
    if (m_thu.joinable())
        m_thu.join();
}

void App::screen()
{
    SDL_Surface *l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_SCREEN
                );
    if (!l_image_surface)
        return;

    SDL_UpdateTexture(m_texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
    SDL_FreeSurface(l_image_surface);
}

void App::run()
{
    rect_default(&m_adb_rect, &m_main_rect);

    if (!m_adbinit)
    {
        if (!m_adb.InitRemote())
        {
            SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_APP
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
                        m_appmenu.m_scale.load(), m_appmenu.m_compress.load(),
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
                        ResManager::IndexStringResource::RES_STR_ERR_APP
                    ),
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ADBINSTALLBIN
                    ),
                    m_window
                );

                SDL_ShowSimpleMessageBox(0, "ADB driver error:", _ex.what(), m_window);
                return;
            }
            catch(...)
            {
                SDL_ShowSimpleMessageBox(0, "ADB driver error:", "unknown error", m_window);
                return;
            }
        }
    };
    m_thu = move(thu);
    return;
}

void App::loop()
{
    m_appmenu.m_isrun = true;
    while(m_appmenu.m_isrun.load())
    {
        while(SDL_PollEvent(&m_window_event) > 0)
        {
            switch(m_window_event.type)
            {
                case SDL_QUIT:
                    {
                        m_appmenu.m_isrun = false;
                        break;
                    }

                case SDL_MOUSEBUTTONDOWN:
                    {
                        if (m_appmenu.mouse())
                            break;

                        switch (m_window_event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                                {
                                    ADBDriver::Tap_s t =
                                    {
                                        ((m_window_event.motion.x - m_appmenu.m_menu_rect.w) *
                                            static_cast<int32_t>(m_appmenu.m_scale.load())),
                                        (m_window_event.motion.y *
                                            static_cast<int32_t>(m_appmenu.m_scale.load()))
                                    };
                                    m_adb.Click(&t);
                                    break;
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
                        if (m_appmenu.mouse())
                            break;

                        m_appmenu.infoset(MgrType::MGR_MAIN, "");
                        break;
                    }

                case SDL_KEYDOWN:
                    {
                        switch (m_window_event.key.keysym.sym)
                        {
                            ///
                            /// Swipe from key
                            ///
                            case SDLK_ESCAPE:
                                {
                                    if (m_appmenu.m_isstop)
                                        m_appmenu.m_isrun = false;
                                    break;
                                }
                            case SDLK_LEFT:
                                {
                                    m_adb_rect.x1 = std::min((m_adb_rect.x1 + 30), m_main_rect.w);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.x0 = m_adb_rect.x1;
                                    break;
                                }
                            case SDLK_RIGHT:
                                {
                                    m_adb_rect.x1 = std::max((m_adb_rect.x1 - 30), 0);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.x0 = m_adb_rect.x1;
                                    break;
                                }
                            case SDLK_UP:
                                {
                                    m_adb_rect.y1 = std::min((m_adb_rect.y1 + 30), m_main_rect.h);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            case SDLK_DOWN:
                                {
                                    m_adb_rect.y1 = std::max((m_adb_rect.y1 - 30), 0);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            case SDLK_HOME:
                                {
                                    m_adb_rect.x1 = static_cast<uint32_t>(m_main_rect.w / 2);
                                    m_adb_rect.y1 = static_cast<uint32_t>(m_main_rect.h / 2);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.x0 = m_adb_rect.x1;
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            case SDLK_PAGEDOWN:
                                {
                                    m_adb_rect.y1 = 0;
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            case SDLK_PAGEUP:
                                {
                                    m_adb_rect.y1 = static_cast<uint32_t>(m_main_rect.h);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            default:
                                {
                                    m_adb.SendSpecialKey(
                                        GameDev::ADBDriver::KeysType::KEYS_SDL,
                                        m_window_event.key.keysym.scancode
                                        );
                                    break;
                                }
                        }
                        break;
                    }
            }
        }
        draw();
    }
}

bool App::update(std::vector<uint8_t> & v, uint32_t w, uint32_t h) noexcept
{
    if (!v.size())
        return m_appmenu.m_isrun.load();

    void *pix = nullptr;
    int32_t pitch = 0,
            iw = static_cast<int32_t>(w),
            ih = static_cast<int32_t>(h);

    if (
        (m_main_rect.w != iw) ||
        (m_main_rect.h != ih)
       )
    {
        SDL_Texture *t_texture = SDL_CreateTexture(
                m_window_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                iw, ih
            );
        if (!t_texture)
            return false;

        std::swap(m_texture, t_texture);
        SDL_DestroyTexture(t_texture);

        m_main_rect.w = iw;
        m_main_rect.h = ih;
        m_main_rect.x = m_appmenu.m_menu_rect.w;
        m_main_rect.y = 0;

        SDL_SetWindowSize(
            m_window,
            (iw + m_appmenu.m_menu_rect.w),
            ih
            );
    }

    SDL_LockTexture(m_texture, nullptr, &pix, &pitch);
    if ((!pix) || (!pitch))
        return false;

    ::memcpy(pix, &v[0], v.size());

    SDL_UnlockTexture(m_texture);
    return ((m_appmenu.m_isstop.load()) ? false : ((m_appmenu.m_isrun.load()) ? true : false));
}

void App::draw()
{
    SDL_RenderClear(m_window_renderer);
    SDL_RenderCopy(m_window_renderer, m_texture, nullptr, &m_main_rect);
    SDL_RenderCopy(m_window_renderer, m_appmenu.m_menu_texture, nullptr, &m_appmenu.m_menu_rect);
    if (m_appmenu.m_info_texture)
        SDL_RenderCopy(m_window_renderer, m_appmenu.m_info_texture, NULL, &m_appmenu.m_info_rect);

    SDL_RenderPresent(m_window_renderer);
}
