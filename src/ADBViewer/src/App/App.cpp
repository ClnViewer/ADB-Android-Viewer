
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
    m_base.gui.rect.w = __W_default;
    m_base.gui.rect.h = __H_default;
    m_base.gui.rect.x = m_appmenu.gui.rect.w;
    m_base.gui.rect.y = 0;
    m_adb_rect.t  = 100U;
    m_pinput = { 50, (__H_default - 40) };

    switch (initm(__W_default + m_appmenu.gui.rect.w, __H_default))
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
    m_base.gui.texture = SDL_CreateTexture(
                m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                __W_default, __H_default
            );

    if (
        (!m_base.gui.texture) ||
        (!m_base.initgui(this)) || /// this main screen
        (!m_info.init(
                this,
                ResManager::IndexFontResource::RES_FONT_16704,
                ResManager::IndexColorResource::RES_COLOR_GREEN_BLACK
            )) ||                /// info-help left rectangle
        (!m_input.init(
                this,
                ResManager::IndexFontResource::RES_FONT_FREESANS,
                ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE
            )) ||                /// keyboard input rectangle
        (!m_appmenu.init(this))  /// this menu screen
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

    if (SDL_SetWindowHitTest(m_window, f_hitTest, &m_appmenu.gui.rect) < 0)
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
    if (m_thu.joinable())
        m_thu.join();

    //m_base.~guiBase();
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

    SDL_UpdateTexture(m_base.gui.texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
    SDL_FreeSurface(l_image_surface);
}

void App::run()
{
    rect_default(&m_adb_rect, &m_base.gui.rect);

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
    m_appmenu.m_isrun = true;
    while (m_appmenu.m_isrun.load())
    {
        while (SDL_PollEvent(&m_event) > 0)
        {
            if ((!m_appmenu.m_isstop) && (m_input.status()))
            {
                if (m_input.event(&m_event, &m_pinput))
                    break;
            }
            switch(m_event.type)
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

                        switch (m_event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                                {
                                    ADBDriver::Tap_s t =
                                    {
                                        ((m_event.motion.x - m_appmenu.gui.rect.w) *
                                            static_cast<int32_t>(m_appmenu.m_scale.load())),
                                        (m_event.motion.y *
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

                        m_appmenu.infoset(MgrType::MGR_MAIN, "", -1);
                        break;
                    }

                case SDL_KEYDOWN:
                    {
                        switch (m_event.key.keysym.sym)
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
                            case SDLK_RETURN:
                            case SDLK_RETURN2:
                                {
                                    if (m_appmenu.m_isstop)
                                        break;

                                    if (!m_input.status())
                                    {
                                        m_input.begin(
                                            ResManager::stringload(
                                                ResManager::IndexStringResource::RES_STR_ENTER_TEXT
                                            ),
                                            &m_pinput
                                        );
                                    }
                                    else
                                    {
                                        if (m_input.isresult())
                                            m_adb.SendTextASCII(m_input.getresult());
                                            /// No UTF8 !! Cyrillic and other national language
                                    }
                                    break;
                                }
                            case SDLK_LEFT:
                                {
                                    m_adb_rect.x1 = std::min((m_adb_rect.x1 + 30), m_base.gui.rect.w);
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
                                    m_adb_rect.y1 = std::min((m_adb_rect.y1 + 30), m_base.gui.rect.h);
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
                                    m_adb_rect.x1 = static_cast<uint32_t>(m_base.gui.rect.w / 2);
                                    m_adb_rect.y1 = static_cast<uint32_t>(m_base.gui.rect.h / 2);
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
                                    m_adb_rect.y1 = static_cast<uint32_t>(m_base.gui.rect.h);
                                    m_adb.Click(&m_adb_rect);
                                    m_adb_rect.y0 = m_adb_rect.y1;
                                    break;
                                }
                            default:
                                {
                                    m_adb.SendSpecialKey(
                                        GameDev::ADBDriver::KeysType::KEYS_SDL,
                                        m_event.key.keysym.scancode
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

    m_base.gui.active = true;
    void *pix = nullptr;
    int32_t pitch = 0,
            iw = static_cast<int32_t>(w),
            ih = static_cast<int32_t>(h);

    if (
        (m_base.gui.rect.w != iw) ||
        (m_base.gui.rect.h != ih)
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

        std::swap(m_base.gui.texture, t_texture);
        SDL_DestroyTexture(t_texture);

        m_base.gui.rect.w = iw;
        m_base.gui.rect.h = ih;
        m_base.gui.rect.x = m_appmenu.gui.rect.w;
        m_base.gui.rect.y = 0;

        SDL_SetWindowSize(
            m_window,
            (iw + m_appmenu.gui.rect.w),
            ih
            );
    }

    SDL_LockTexture(m_base.gui.texture, nullptr, &pix, &pitch);
    if ((!pix) || (!pitch))
        return false;

    ::memcpy(pix, &v[0], v.size());

    SDL_UnlockTexture(m_base.gui.texture);
    return ((m_appmenu.m_isstop.load()) ? false : ((m_appmenu.m_isrun.load()) ? true : false));
}
