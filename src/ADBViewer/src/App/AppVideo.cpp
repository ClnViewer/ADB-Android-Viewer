
#include "../ADBViewer.h"

static void rect_default(ADBDriver::Swipe_s *adbr, SDL_Rect *sdlr)
{
    adbr->x0 = adbr->x1 = static_cast<uint32_t>(sdlr->w / 2);
    adbr->y0 = adbr->y1 = static_cast<uint32_t>(sdlr->h / 2);
}

AppVideo::AppVideo()
    : m_app{nullptr} { guiBase::gui.rect = {}; }

AppVideo::~AppVideo()
{
    guiBase::ActiveOff();
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
    guiBase::gui.texture = nullptr;

    return initgui(app);
}

bool AppVideo::tinit(SDL_Texture **texture)
    {
        guiBase::gui.rect.w = AppConfig::instance().cnf_disp_point.x - __MENU_W_default;
        guiBase::gui.rect.h = AppConfig::instance().cnf_disp_point.y;
        guiBase::gui.rect.x = __MENU_W_default;
        guiBase::gui.rect.y = 0;

        SDL_Texture *l_texture = SDL_CreateTexture(
                m_app->m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                guiBase::gui.rect.w,
                guiBase::gui.rect.h
            );

        if (!l_texture)
            return false;

        GuiLock(
            std::swap(*texture, l_texture);
        );

        if (l_texture)
            SDL_DestroyTexture(l_texture);

        if (!defscreen(texture))
            return false;
        return true;
    }

bool AppVideo::evresize(SDL_Texture **texture)
    {
        (void) tinit(texture);
        return false;
    }

bool AppVideo::defscreen()
    {
        return defscreen(&guiBase::gui.texture);
    }

bool AppVideo::defscreen(SDL_Texture **texture)
{
    SDL_Color   *l_bgcolor = nullptr;
    SDL_Surface *l_image_surface_img = nullptr;
    SDL_Surface *l_image_surface_bg = nullptr;
    SDL_Rect rbg{};

    do
    {
        GuiLock(
            if ((!texture) || (!*texture))
                break;
        );

        if (!(l_image_surface_img = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_SCREEN
            )))
            break;

        if (
            (guiBase::gui.rect.w > l_image_surface_img->w) ||
            (guiBase::gui.rect.h > l_image_surface_img->h)
            )
        {
            rbg.w = guiBase::gui.rect.w;
            rbg.h = guiBase::gui.rect.h;

            if (!(l_image_surface_bg = SDL_CreateRGBSurface(
                    0U,
                    rbg.w,
                    rbg.h,
                    24,
#                   if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#                   else
                    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#                   endif
                )))
                break;

            if (!(l_bgcolor = ResManager::colorload(
                            ResManager::IndexColorResource::RES_COLOR_GREEN_BG
                )))
                break;

            SDL_FillRect(l_image_surface_bg,
                     &rbg,
                     SDL_MapRGB(
                            l_image_surface_bg->format,
                            l_bgcolor[0].r,
                            l_bgcolor[0].g,
                            l_bgcolor[0].b
                        )
                );

            guiBase::ActiveOff();
            GuiLock(
                SDL_UpdateTexture(*texture, &rbg, l_image_surface_bg->pixels, l_image_surface_bg->pitch);
            );

            rbg.x = ((guiBase::gui.rect.w - l_image_surface_img->w) / 2);
            rbg.y = ((guiBase::gui.rect.h - l_image_surface_img->h) / 2);
        }

        rbg.w = ((guiBase::gui.rect.w < l_image_surface_img->w) ? guiBase::gui.rect.w : l_image_surface_img->w);
        rbg.h = ((guiBase::gui.rect.h < l_image_surface_img->h) ? guiBase::gui.rect.h : l_image_surface_img->h);

        GuiLock(
            SDL_UpdateTexture(*texture, &rbg, l_image_surface_img->pixels, l_image_surface_img->pitch);
        );
    }
    while (0);

    if (l_image_surface_img)
    {
        SDL_FreeSurface(l_image_surface_img);
        if (l_image_surface_bg)
            SDL_FreeSurface(l_image_surface_bg);

        guiBase::ActiveOn();
        return true;
    }
    return false;
}

void AppVideo::stop()
{
    if (AppConfig::instance().cnf_isstop)
        return;
    AppConfig::instance().cnf_isstop = true;
    guiBase::ActiveOff();
    jointh();
    defscreen();
    m_app->m_appabender.run();
    guiBase::ActiveOn();
}

void AppVideo::run()
{
    if (!m_app)
        return;

    if (!AppConfig::instance().cnf_isstop)
        return;

    rect_default(&AppConfig::instance().cnf_adb_rect, &gui.rect);
    m_app->m_appabender.stop();

    if (!AppConfig::instance().cnf_adbinit)
    {
        if (!AppConfig::instance().cnf_adb.InitRemote())
        {
            SDL_ShowSimpleMessageBox(
                    0,
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_ADRV,
                        AppConfig::instance().cnf_lang
                    ),
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ADBINSTALLBIN,
                        AppConfig::instance().cnf_lang
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
                        AppConfig::instance().cnf_disp_ratio.load(), AppConfig::instance().cnf_compress.load(),
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
                AddMessageQueue(
                    _ex.what(),
                    10U,
                    -1
                );
                guiBase::ActiveOff();
                defscreen();
                guiBase::ActiveOn();
                AppConfig::instance().cnf_isstop = true;
                return;
            }
            catch(...)
            {
                AddMessageQueue(
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN,
                        AppConfig::instance().cnf_lang
                    ),
                    10U,
                    -1
                );
                guiBase::ActiveOff();
                defscreen();
                guiBase::ActiveOn();
                AppConfig::instance().cnf_isstop = true;
                return;
            }
            AppConfig::instance().cnf_isstop = true;
        }
    };
    m_thu = move(thu);
}

bool AppVideo::update(std::vector<uint8_t> & v, uint32_t w, uint32_t h)
{
    if ((!m_app) || (!v.size()))
        return AppConfig::instance().cnf_isrun.load();

    do
    {
        const int32_t
                iw = static_cast<int32_t>(w),
                ih = static_cast<int32_t>(h);

        if (!guiBase::gui.texture)
            return false;

        guiBase::ActiveOn();

        if ((guiBase::gui.rect.w != iw) || (guiBase::gui.rect.h != ih))
        {
            AppConfig::instance().SetDisplaySize(w, h);

            SDL_SetWindowSize(
                m_app->m_window,
                AppConfig::instance().cnf_disp_point.x,
                AppConfig::instance().cnf_disp_point.y
            );

            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }

        GuiLock(
            void   *pix = nullptr;
            int32_t pitch = 0;
            SDL_LockTexture(guiBase::gui.texture, nullptr, &pix, &pitch);
            if ((!pix) || (!pitch))
                return false;
            ::memcpy(pix, &v[0], v.size());
            SDL_UnlockTexture(guiBase::gui.texture);
        );
    }
    while (0);

    return ((AppConfig::instance().cnf_isstop.load()) ?
            false : AppConfig::instance().cnf_isrun.load()
        );
}

bool AppVideo::event(SDL_Event *ev, const void *instance)
{
    AppVideo *apv = static_cast<AppVideo*>(
                const_cast<void*>(instance)
            );

    if (
        (!apv) ||
        (apv->m_app->m_appinput.isactive())
        )
        return false;

    if (ev->type == AppConfig::instance().cnf_uevent)
    {
        switch(ev->user.code)
        {
            /// Restart connect to device event
            case ID_CMD_POP_MENU99:
                {
                    apv->stop();
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    apv->run();
                    return true;
                }
            /// MenuBar device connect event
            case ID_CMD_POP_MENU97:
                {
                    apv->run();
                    return true;
                }
            /// MenuBar device disconnect event
            case ID_CMD_POP_MENU98:
                {
                    apv->stop();
                    return true;
                }
        }
    }

    switch(ev->type)
    {
        case SDL_RENDER_TARGETS_RESET:
        case SDL_RENDER_DEVICE_RESET:
            {
                (void) apv->evresize(&apv->gui.texture);
                return false;
            }
    }

    if (AppConfig::instance().cnf_isstop)
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
                                static_cast<int32_t>(AppConfig::instance().cnf_disp_ratio.load())),
                        (ev->motion.y *
                                static_cast<int32_t>(AppConfig::instance().cnf_disp_ratio.load()))
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
