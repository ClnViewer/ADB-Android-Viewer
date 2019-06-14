
#include "../ADBViewer.h"

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
{
    AppConfig::instance().init();
    AppMessageQueue::instance().init();

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
                        ResManager::IndexStringResource::RES_STR_ERR_APP,
                        AppConfig::instance().cnf_lang
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

    /// Instance initiation by step of renderer (Overwrite prevision screen region)
    if (
        (!m_appvideo.init(this))   || /// this main GUI screen
        (!m_appani.init(this))     || /// this animation default GUI screen
        (!m_appmenubar.init(this)) || /// this menu BAR screen
        (!m_appmenupop.init(this)) || /// this menu POPUP event
        (!m_appeditor.init(this))  || /// this editor GUI screen
        (!m_appmsgbar.init(this))  || /// this message BAR box
        (!m_appinput.init(
                this,
                ResManager::IndexFontResource::RES_FONT_FREESANS,
                ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE
            ))                        /// keyboard input rectangle
    )
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
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
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
            ),
            SDL_GetError(),
            m_window
        );
        return;
    }

#   if defined(OS_WIN_EVENT)
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
#   endif

    /// event call only
    gui.texture = nullptr;
    (void) initgui(this);

    Plugins::AppPluginManager::instance().init();
}

App::~App()
{
}

void App::loop()
{
    AppConfig::instance().cnf_isrun = true;
    while (AppConfig::instance().cnf_isrun.load())
    {
        while (SDL_PollEvent(&m_event) > 0)
            events(&m_event);

        if (!AppConfig::instance().cnf_isrun.load())
            break;

        draw();
    }
}

bool App::event(SDL_Event *ev, const void *instance)
{
    App *app = static_cast<App*>(
                const_cast<void*>(instance)
            );

    switch(ev->type)
    {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
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

            if (ev->button.button == SDL_BUTTON_RIGHT)
            {
                app->m_appmenupop.show();
                return true;
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

                    if (!app->m_appinput.isactive())
                    {
                        app->m_appinput.begin(
                            ResManager::stringload(
                                ResManager::IndexStringResource::RES_STR_ENTER_TEXT,
                                AppConfig::instance().cnf_lang
                            )
                        );
                    }
                    else
                    {
                        if (app->m_appinput.isresult())
                            AppConfig::instance().cnf_adb.SendTextASCII(app->m_appinput.getresult());
                            /// No UTF8 !! Cyrillic and other national language
                    }
                    return true;
                }
                default:
                    break;
            }
        }
    }
    return false;
}
