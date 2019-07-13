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

bool AppTerminal::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = nullptr;
        guiBase::gui.texture = nullptr;

        bool ret = guiBase::initgui(app);
        if (ret)
        {
            do
            {
                ret = m_page.init(guiBase::GetGui<SDL_Window>());
                if (!ret)
                    break;

                ret = m_icon_close.init(
                        app,
                        m_page.btn_r_close,
                        ResManager::IndexImageResource::RES_IMG_TERMCLOSE,
                        [=](SDL_Event *ev, SDL_Rect *r)
                        {
                            switch (ev->type)
                            {
                                case SDL_MOUSEMOTION:
                                    {
                                        AddMessageQueue(
                                            ResManager::stringload(
                                                ResManager::IndexStringResource::RES_STR_CLOSE_TERM,
                                                AppConfig::instance().cnf_lang
                                            ),
                                            5U,
                                            (__LINE__ + 1000)
                                        );
                                        guiBase::PushEvent(ID_CMD_POP_MENU26);
                                        return true;
                                    }
                                case SDL_MOUSEBUTTONDOWN:
                                    {
                                        guiBase::PushEvent(ID_CMD_POP_MENU24);
                                        return true;
                                    }
                            }
                            return false;
                        }
                    );
                if (!ret)
                    break;

                ret = m_toutput.init(app, &m_page);
                if (!ret)
                    break;

                ret = m_tinput.init(
                        app,
                        &m_page,
                        [=](std::string & s)
                        {
                            adbsend(s);
                        }
                    );
            }
            while (0);
        }
        return ret;
    }

bool AppTerminal::tinit(SDL_Texture **texture)
    {
        GuiLock(
            if ((texture) && (*texture))
                SDL_DestroyTexture(*texture);
            *texture = nullptr;
        );
        return true;
    }

bool AppTerminal::evresize(SDL_Texture **texture)
    {
        return tinit(texture);
    }

bool AppTerminal::isenabled() const
    {
        return m_enable.load();
    }

void AppTerminal::adbsend(std::string & s)
    {
        std::string ss;

        do
        {
            if (!s.empty())
            {
                ss = s;
                break;
            }
            if (m_tinput.guiTextInputBox::isresult())
                ss = m_tinput.guiTextInputBox::getresult(m_page.getprompt());
            else
                (void) m_tinput.guiTextInputBox::getresult(m_page.getprompt());
        }
        while (0);

        if (ss.empty())
            return;

        std::string so;
        m_toutput.draw_cmd(ss);

        if (AppConfig::instance().cnf_adb.SendToShell(ss, so))
            if (!so.empty())
                m_toutput.draw_txt(so);
    }

void AppTerminal::runselect()
    {
        std::lock_guard<std::mutex> l_lock(m_lock);

        if (m_enable.load())
        {
            m_enable = false;
            m_tinput.stop();
            m_toutput.stop();
            m_icon_close.Off();

            SDL_SetWindowSize(
                guiBase::GetGui<SDL_Window>(),
                AppConfig::instance().cnf_disp_point.x,
                AppConfig::instance().cnf_disp_point.y
            );
        }
        else
        {
            if ((m_page.rbase.h - AppConfig::instance().cnf_disp_point.y) < 64)
            {
                return;
            }

            m_enable = true;
            m_page.init(guiBase::GetGui<SDL_Window>());
            m_tinput.guiTextInputBox::setcord(m_page.c_p_input);

            SDL_SetWindowSize(
                guiBase::GetGui<SDL_Window>(),
                m_page.rbase.w,
                m_page.rbase.h
            );

            m_tinput.start();
            m_toutput.start();
            m_icon_close.On();
        }
    }

bool AppTerminal::uevent(SDL_Event *ev, const void *instance)
    {
        AppTerminal *apt = static_cast<AppTerminal*>(
                const_cast<void*>(instance)
            );

        if (!apt)
            return false;

        if (ev->user.code != ID_CMD_POP_MENU24)
            return false;

        apt->runselect();
        return true;
    }

bool AppTerminal::event(SDL_Event *ev, const void *instance)
{
    AppTerminal *apt = static_cast<AppTerminal*>(
                const_cast<void*>(instance)
            );

    if (!apt)
         return false;

    if (!m_enable.load())
        return false;

    auto istate = apt->m_app->state();
    if ((istate[App::AppStateType::STATE_APP_STOP]) ||
        (istate[App::AppStateType::STATE_APP_EDIT]) ||
        (istate[App::AppStateType::STATE_APP_INPUT]))
        return false;

    /// todo check input & otput screen size
    if (apt->guiBase::IsRegion(ev, apt->m_app->m_appvideo.guiBase::GetGui<SDL_Rect>()))
        return false;

    switch (ev->type)
    {
        case SDL_KEYDOWN:
            {
                if (ev->key.keysym.mod != KMOD_NONE)
                    for (auto & m : AppConfig::instance().cnf_keymod_alt)
                        if (ev->key.keysym.mod & m)
                            return false;

                switch (ev->key.keysym.sym)
                {
                    /// Text input panel end input (CTRL + ENTER) or (ENTER NUMPAD)
                    case SDLK_RETURN:
                    case SDLK_RETURN2:
                        {
                            std::string s;
                            apt->adbsend(s);
                            return true;
                        }
                    case SDLK_PAGEDOWN:
                        {
                            m_toutput.pageDown();
                            return true;
                        }
                    case SDLK_PAGEUP:
                        {
                            m_toutput.pageUp();
                            return true;
                        }
                }
                return apt->m_tinput.guiTextInputBox::eventcb(ev);
            }
        case SDL_TEXTINPUT:
            {
                return apt->m_tinput.guiTextInputBox::eventcb(ev);
            }

    }
    return false;
}

