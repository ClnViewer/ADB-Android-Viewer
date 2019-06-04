
#include "../ADBViewer.h"

static void rect_default(ADBDriver::Swipe_s *adbr, SDL_Rect *sdlr)
{
    adbr->x0 = adbr->x1 = static_cast<uint32_t>(sdlr->w / 2);
    adbr->y0 = adbr->y1 = static_cast<uint32_t>(sdlr->h / 2);
}

AppVideo::AppVideo()
    : m_app{nullptr} {}

AppVideo::~AppVideo()
{
    gui.active = false;
    jointh();
}

void AppVideo::jointh()
{
    if (m_thu.joinable())
        m_thu.join();
}

bool AppVideo::init(App *app)
{
    if (!app)
        return false;

    m_app = app;

    gui.rect.h = __H_default;
    gui.rect.w = __W_default;
    gui.rect.x = m_app->m_appmenubar.gui.rect.w;
    gui.rect.y = 0;

    gui.texture = SDL_CreateTexture(
                m_app->m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                __W_default, __H_default
            );

    if (
        (!gui.texture) ||
        (!defscreen())
       )
        return false;

    return initgui(app);
}

bool AppVideo::defscreen()
{
    SDL_Surface *l_image_surface = nullptr;

    do
    {
        if (!(l_image_surface = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_SCREEN
            )))
            break;

        SDL_UpdateTexture(gui.texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
    }
    while (0);

    if (l_image_surface)
    {
        SDL_FreeSurface(l_image_surface);
        return true;
    }
    return false;
}

void AppVideo::stop()
{
    AppConfig::instance().cnf_isstop = true;
    gui.active = false;
    jointh();
    defscreen();
    gui.active = true;
}

void AppVideo::run()
{
    if (!m_app)
        return;

    rect_default(&AppConfig::instance().cnf_adb_rect, &gui.rect);

    if (!AppConfig::instance().cnf_adbinit)
    {
        if (!AppConfig::instance().cnf_adb.InitRemote())
        {
            SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_ADRV
                    ),
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ADBINSTALLBIN
                    ),
                    m_app->m_window
                );
            return;
        }
        AppConfig::instance().cnf_adbinit = true;
    }

    AppConfig::instance().cnf_isstop = false;

    std::thread thu
    {
        [=]()
        {
            try
            {
                (void) AppConfig::instance().cnf_adb.Video(
                        AppConfig::instance().cnf_scale.load(), AppConfig::instance().cnf_compress.load(),
                        [this](std::vector<uint8_t> & v, uint32_t w, uint32_t h)
                            {
                                if (m_app->m_appeditor.isactive())
                                    m_app->m_appeditor.update(w, h, v);

                                bool ret = update(v, w, h);
                                if (ret)
                                    Plugins::AppPluginManager::instance().run(v, w, h);
                                return ret;
                            }
                        );
            }
            catch(std::exception & _ex)
            {
                /*
                SDL_Point offset = { (m_app->m_appmenubar.gui.rect.w + 1), 0 };
                m_app->m_appinfo.draw(
                    _ex.what(),
                    &offset,
                    -1
                    );
                */
                gui.active = false;
                defscreen();
                gui.active = true;
                AppConfig::instance().cnf_isstop = true;
                return;
            }
            catch(...)
            {
                /*
                SDL_Point offset = { (m_app->m_appmenubar.gui.rect.w + 1), 0 };
                m_app->m_appinfo.draw(
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN
                    ),
                    &offset,
                    -1
                    );
                */
                gui.active = false;
                defscreen();
                gui.active = true;
                AppConfig::instance().cnf_isstop = true;
                return;
            }
            AppConfig::instance().cnf_isstop = true;
        }
    };
    m_thu = move(thu);
}

bool AppVideo::update(std::vector<uint8_t> & v, uint32_t w, uint32_t h) noexcept
{
    if ((!m_app) || (!v.size()))
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
                m_app->m_renderer,
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
        gui.rect.x = m_app->m_appmenubar.gui.rect.w;
        gui.rect.y = 0;

        SDL_SetWindowSize(
            m_app->m_window,
            (iw + m_app->m_appmenubar.gui.rect.w),
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

bool AppVideo::event(SDL_Event *ev, const void *instance)
{
    AppVideo *apv = static_cast<AppVideo*>(
                const_cast<void*>(instance)
            );

    if (
        (!apv) ||
        (AppConfig::instance().cnf_isstop) ||
        (apv->m_app->m_appinput.isactive())
        )
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (
                (AppConfig::instance().cnf_isstop) ||
                (apv->m_app->m_appeditor.isactive()) ||
                (ev->motion.x <= apv->m_app->m_appmenubar.gui.rect.w)
               )
                break;

            switch (ev->button.button)
            {
                case SDL_BUTTON_LEFT:
                {
                    ADBDriver::Tap_s t =
                    {
                        ((ev->motion.x - apv->m_app->m_appmenubar.gui.rect.w) *
                                static_cast<int32_t>(AppConfig::instance().cnf_scale.load())),
                        (ev->motion.y *
                                static_cast<int32_t>(AppConfig::instance().cnf_scale.load()))
                    };
                    AppConfig::instance().cnf_adb.Click(&t);
                    return true;
                }
                default:
                    break;
            }
            break;
        }

        case SDL_KEYDOWN:
        {
            ///
            /// Swipe from key:
            /// SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN,
            /// SDLK_HOME, SDLK_PAGEDOWN, SDLK_PAGEUP
            ///

            if (
                (AppConfig::instance().cnf_isstop) ||
                (apv->m_app->m_appinput.isactive())
               )
                break;

            switch (ev->key.keysym.sym)
            {
                case SDLK_LEFT:
                {
                    AppConfig::instance().cnf_adb_rect.x1 = std::min((AppConfig::instance().cnf_adb_rect.x1 + 30), apv->gui.rect.w);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.x0 = AppConfig::instance().cnf_adb_rect.x1;
                    return true;
                }
                case SDLK_RIGHT:
                {
                    AppConfig::instance().cnf_adb_rect.x1 = std::max((AppConfig::instance().cnf_adb_rect.x1 - 30), 0);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.x0 = AppConfig::instance().cnf_adb_rect.x1;
                    return true;
                }
                case SDLK_UP:
                {
                    AppConfig::instance().cnf_adb_rect.y1 = std::min((AppConfig::instance().cnf_adb_rect.y1 + 30), apv->gui.rect.h);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.y0 = AppConfig::instance().cnf_adb_rect.y1;
                    return true;
                }
                case SDLK_DOWN:
                {
                    AppConfig::instance().cnf_adb_rect.y1 = std::max((AppConfig::instance().cnf_adb_rect.y1 - 30), 0);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.y0 = AppConfig::instance().cnf_adb_rect.y1;
                    return true;
                }
                case SDLK_HOME:
                {
                    AppConfig::instance().cnf_adb_rect.x1 = static_cast<uint32_t>(apv->gui.rect.w / 2);
                    AppConfig::instance().cnf_adb_rect.y1 = static_cast<uint32_t>(apv->gui.rect.h / 2);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.x0 = AppConfig::instance().cnf_adb_rect.x1;
                    AppConfig::instance().cnf_adb_rect.y0 = AppConfig::instance().cnf_adb_rect.y1;
                    return true;
                }
                case SDLK_PAGEDOWN:
                {
                    AppConfig::instance().cnf_adb_rect.y1 = 0;
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.y0 = AppConfig::instance().cnf_adb_rect.y1;
                    return true;
                }
                case SDLK_PAGEUP:
                {
                    AppConfig::instance().cnf_adb_rect.y1 = static_cast<uint32_t>(apv->gui.rect.h);
                    AppConfig::instance().cnf_adb.Click(&AppConfig::instance().cnf_adb_rect);
                    AppConfig::instance().cnf_adb_rect.y0 = AppConfig::instance().cnf_adb_rect.y1;
                    return true;
                }
                default:
                {
                    AppConfig::instance().cnf_adb.SendSpecialKey(
                        GameDev::ADBDriver::KeysType::KEYS_SDL,
                            ev->key.keysym.scancode
                    );
                    return true;
                }
            }
            break;
        }

        default:
            break;
    }
    return false;
}
