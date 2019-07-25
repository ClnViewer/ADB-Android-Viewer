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

#include "../../ADBViewer.h"

AppBrowser::AppBrowser() :
    m_enable(false) {}

bool AppBrowser::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = nullptr;
        guiBase::gui.texture = nullptr;

        bool ret = guiBase::initgui(app);
        do
        {
            if (!ret)
                break;

            ret = m_page.init(guiBase::GetGui<SDL_Window>());
            if (!ret)
                break;

            /// other control initialize
            ret = m_icon_menu4.init(
                    app,
                    m_page.menu4,
                    &m_color,
                    ResManager::IndexSpriteResource::RES_SPRITE_BROWSER_MENU4,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        return mevent4(ev, r);
                    }
                );
            if (!ret)
                break;

            ret = m_icon_menu9.init(
                    app,
                    m_page.menu9,
                    &m_color,
                    ResManager::IndexSpriteResource::RES_SPRITE_BROWSER_MENU9,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        return mevent9(ev, r);
                    }
                );
            if (!ret)
                break;

            ret = m_list.init(app, &m_page);

        }
        while (0);
        return ret;
    }

bool AppBrowser::tinit(SDL_Texture **texture)
    {
        GuiLock(
            if ((texture) && (*texture))
                *texture = nullptr;
        );
        return true;
    }

bool AppBrowser::evresize(SDL_Texture **texture)
    {
        return tinit(texture);
    }

bool AppBrowser::isenabled() const
    {
        return m_enable.load();
    }

void AppBrowser::resizewin(int32_t w)
    {
        SDL_Point point{};
        SDL_GetWindowSize(
            guiBase::GetGui<SDL_Window>(),
            &point.x,
            &point.y
        );
        SDL_SetWindowSize(
            guiBase::GetGui<SDL_Window>(),
            w,
            ((point.y) ? point.y : AppConfig::instance().cnf_disp_point.y)
        );
    }

void AppBrowser::runselect(bool isclose)
    {
        std::lock_guard<std::mutex> l_lock(m_lock);

        if (m_enable.load())
        {
            m_enable = false;

            if (!isclose)
                resizewin(AppConfig::instance().cnf_disp_point.x);

            m_icon_menu4.Off();
            m_icon_menu9.Off();
            m_list.stop();
        }
        else if (!isclose)
        {
            m_page.init(guiBase::GetGui<SDL_Window>());

            if ((m_page.rbase.w - AppConfig::instance().cnf_disp_point.x) < m_page.minsize.x)
            {
                AddMessageQueue(
                    ResManager::stringload(
                        ResManager::IndexStringResource::RES_STR_SMALL_BROWSER,
                        AppConfig::instance().cnf_lang
                    ),
                    10U,
                    -1
                );
                return;
            }

            m_enable = true;
            resizewin(m_page.rbase.w);
            SDL_Point point{};
            m_icon_menu4.On(&(point = m_page.getoffset(m_page.menu4)));
            m_icon_menu9.On(&(point = m_page.getoffset(m_page.menu9)));
            m_list.start();
        }
    }

bool AppBrowser::uevent(SDL_Event *ev, const void *instance)
    {
        AppBrowser *apb = static_cast<AppBrowser*>(
                const_cast<void*>(instance)
            );

        if (!apb)
            return false;

        switch (ev->user.code)
        {
            case ID_CMD_POP_MENU52:
                {
                    apb->runselect(false);
                    return true;
                }
            case ID_CMD_POP_MENU53:
                {
                    apb->runselect(true);
                    return true;
                }
        }
        return false;
    }

bool AppBrowser::event(SDL_Event *ev, const void *instance)
{
    AppBrowser *apb = static_cast<AppBrowser*>(
                const_cast<void*>(instance)
            );

    if (!apb)
         return false;

    if (!m_enable.load())
        return false;

    auto istate = apb->m_app->state();
    if ((istate[App::AppStateType::STATE_APP_STOP]) ||
        (istate[App::AppStateType::STATE_APP_EDIT]) ||
        (istate[App::AppStateType::STATE_APP_INPUT]))
        return false;

    if (!guiBase::IsFocus(&apb->m_page.rbase))
        return false;

    switch (ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button != SDL_BUTTON_LEFT)
                break;
            if (!guiBase::IsFocus(&apb->m_page.list_rdst))
                break;

            apb->m_list.lineSelect(ev);
            return true;
        }
        case SDL_KEYDOWN:
            {
                if (ev->key.keysym.mod != KMOD_NONE)
                    for (auto & m : AppConfig::instance().cnf_keymod_disabled)
                        if (ev->key.keysym.mod & m)
                            return false;

                switch (ev->key.keysym.sym)
                {
                    case SDLK_UP:
                        {
                            apb->m_list.lineUp();
                            return true;
                        }
                    case SDLK_DOWN:
                        {
                            apb->m_list.lineDown();
                            return true;
                        }
                    case SDLK_PAGEUP:
                        {
                            apb->m_list.pageUp();
                            return true;
                        }
                    case SDLK_PAGEDOWN:
                        {
                            apb->m_list.pageDown();
                            return true;
                        }
                    case SDLK_HOME:
                        {
                            apb->m_list.pageHome();
                            return true;
                        }
                    case SDLK_END:
                        {
                            apb->m_list.pageEnd();
                            return true;
                        }
                }
                break;
            }
    }
    return false;
}

