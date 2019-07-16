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

static void rect_default(ADBDriver::Swipe_s *adbr, SDL_Rect *sdlr)
{
    adbr->x0 = adbr->x1 = static_cast<uint32_t>(sdlr->w / 2);
    adbr->y0 = adbr->y1 = static_cast<uint32_t>(sdlr->h / 2);
}

AppVideo::AppVideo()
    : m_app{nullptr} { guiBase::gui.rdst = {}; }

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
    guiBase::gui.rdst = {};
    guiBase::gui.rsrc = nullptr;
    guiBase::gui.texture = nullptr;

    return guiBase::initgui(app);
}

bool AppVideo::tinit(SDL_Texture **texture)
    {
        SDL_Rect *r = guiBase::GetGui<SDL_Rect>();
        SDL_Point point_img_menu = ResManager::imagesize(
            ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
        );

        r->w = AppConfig::instance().cnf_disp_point.x - point_img_menu.x;
        r->h = AppConfig::instance().cnf_disp_point.y;
        r->x = point_img_menu.x;
        r->y = 0;

        SDL_Texture *l_texture = SDL_CreateTexture(
                guiBase::GetGui<SDL_Renderer>(),
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                r->w,
                r->h
            );

        if (!l_texture)
        {
            SDLErrorMessageQueue();
            return false;
        }

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

        SDL_Rect *r = guiBase::GetGui<SDL_Rect>();

        if (!(l_image_surface_img = ResManager::imageload(
                    ResManager::IndexImageResource::RES_IMG_SCREEN
            )))
            break;

        if (
            (r->w > l_image_surface_img->w) ||
            (r->h > l_image_surface_img->h)
            )
        {
            rbg.w = r->w;
            rbg.h = r->h;

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

            rbg.x = ((r->w - l_image_surface_img->w) / 2);
            rbg.y = ((r->h - l_image_surface_img->h) / 2);
        }

        rbg.w = ((r->w < l_image_surface_img->w) ? r->w : l_image_surface_img->w);
        rbg.h = ((r->h < l_image_surface_img->h) ? r->h : l_image_surface_img->h);

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
    m_app->flushicon(3);
}

void AppVideo::run()
{
    if (!m_app)
        return;

    if (!AppConfig::instance().cnf_isstop)
        return;

    rect_default(&AppConfig::instance().cnf_adb_rect, &guiBase::gui.rdst);
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
                    guiBase::GetGui<SDL_Window>()
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
                        AppConfig::instance().cnf_disp_ratio.load(),
                        AppConfig::instance().cnf_disp_rotate.load(),
                        AppConfig::instance().cnf_compress.load(),
                        [this](std::vector<uint8_t> & v, uint32_t w, uint32_t h)
                            {
                                if (m_app->m_appeditor.isenabled())
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
    m_app->flushicon(3);
}

bool AppVideo::update(std::vector<uint8_t> & v, uint32_t w, uint32_t h)
{
    if ((!m_app) || (!v.size()))
        return AppConfig::instance().cnf_isrun.load();

    do
    {
        const int32_t iw = static_cast<int32_t>(w),
                      ih = static_cast<int32_t>(h);
        SDL_Rect     *r  = guiBase::GetGui<SDL_Rect>();

        if (!guiBase::gui.texture)
            return false;

        guiBase::ActiveOn();

        if ((r->w != iw) || (r->h != ih))
        {
            AppConfig::instance().SetDisplaySize(w, h);

            SDL_SetWindowSize(
                guiBase::GetGui<SDL_Window>(),
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

bool AppVideo::uevent(SDL_Event *ev, const void *instance)
    {
        AppVideo *apv = static_cast<AppVideo*>(
                const_cast<void*>(instance)
            );

        if (!apv)
            return false;

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
        return false;
    }

bool AppVideo::event(SDL_Event *ev, const void *instance)
{
    AppVideo *apv = static_cast<AppVideo*>(
                const_cast<void*>(instance)
            );

    if (!apv)
        return false;

    if ((ev->type == SDL_RENDER_TARGETS_RESET) || (ev->type == SDL_RENDER_DEVICE_RESET))
    {
        (void) apv->evresize(&apv->guiBase::gui.texture);
        return false;
    }

    auto istate = apv->m_app->state();
    bool isexit = ((istate[App::AppStateType::STATE_APP_STOP]) ||
        (istate[App::AppStateType::STATE_APP_INPUT]) ||
        (istate[App::AppStateType::STATE_APP_EDIT])  ||
        (istate[App::AppStateType::STATE_APP_TERM]));

    if (ev->type == SDL_MOUSEMOTION)
    {
        if (!apv->guiBase::IsRegion(ev, apv->guiBase::GetGui<SDL_Rect>()))
            return false;

        apv->guiBase::PushEvent(ID_CMD_POP_MENU26);
        if ((AppConfig::instance().cnf_ispos) && (!isexit))
            apv->m_app->m_appmsgbar.PrintInfo(MgrType::MGR_MAIN, "", -1, ev);
        return true;
    }

    if (isexit)
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button != SDL_BUTTON_LEFT)
                break;

            if (!apv->guiBase::IsRegion(ev, apv->guiBase::GetGui<SDL_Rect>()))
                break;

            SDL_Point point_img_menu = ResManager::imagesize(
                ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
            );

            ADBDriver::Tap_s t =
            {
                ((ev->motion.x - point_img_menu.x) *
                        static_cast<int32_t>(AppConfig::instance().cnf_disp_ratio.load())),
                (ev->motion.y *
                        static_cast<int32_t>(AppConfig::instance().cnf_disp_ratio.load()))
            };
            AppConfig::instance().cnf_adb.Click(&t);
            return true;
        }

        case SDL_KEYDOWN:
        {
            /// Swipe from key:
            /// Enabled only system key:
            /// SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN,
            /// SDLK_HOME, SDLK_PAGEDOWN, SDLK_PAGEUP
            ///

            if (ev->key.keysym.mod != KMOD_NONE)
                for (auto & m : AppConfig::instance().cnf_keymod_disabled)
                    if (ev->key.keysym.mod & m)
                        return false;

            SDL_Rect *r = apv->guiBase::GetGui<SDL_Rect>();

            switch (ev->key.keysym.sym)
            {
                case SDLK_RETURN:
                {
                    AppConfig::instance().cnf_adb.SendSpecialKey(
                            GameDev::ADBDriver::KeysType::KEYS_SDL,
                            SDL_SCANCODE_KP_ENTER // SDL_SCANCODE_RETURN
                    );
                    return true;
                }
                case SDLK_LEFT:
                {
                    AppConfig::instance().cnf_adb_rect.x1 = std::min((AppConfig::instance().cnf_adb_rect.x1 + 30), r->w);
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
                    AppConfig::instance().cnf_adb_rect.y1 = std::min((AppConfig::instance().cnf_adb_rect.y1 + 30), r->h);
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
                    AppConfig::instance().cnf_adb_rect.x1 = static_cast<uint32_t>(r->w / 2);
                    AppConfig::instance().cnf_adb_rect.y1 = static_cast<uint32_t>(r->h / 2);
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
                    AppConfig::instance().cnf_adb_rect.y1 = static_cast<uint32_t>(r->h);
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
