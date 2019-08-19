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

bool AppInputText::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        return guiTextInputBox::init(
                app,
                AppConfig::instance().cnf_input_point,
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_ENTER_TEXT,
                    AppConfig::instance().cnf_lang
                ),
                ResManager::IndexFontResource::RES_FONT_CONSOLAS,
                ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE,
                ResManager::IndexColorResource::RES_COLOR_GREEN_CURSOR
            );
    }

bool AppInputText::isenabled()
    {
        return guiTextInputBox::isactive();
    }

bool AppInputText::runselect(bool ismod, int32_t ctrl)
    {
        if (guiTextInputBox::isactive())
        {
            if (guiTextInputBox::isresult())
                AppConfig::instance().cnf_adb.SendTextT<std::string>(guiTextInputBox::getresult());

            guiTextInputBox::end();
            guiBase::PushEvent(ID_CMD_POP_MENU102);
            return true;
        }
        else
        {
            if (
                (ctrl == -1) ||
                ((ismod) && (ctrl == SDLK_RETURN))
                )
            {
                guiTextInputBox::begin();
                guiBase::PushEvent(ID_CMD_POP_MENU102);
                return true;
            }
        }
        return false;
    }

bool AppInputText::uevent(SDL_Event *ev, const void *instance)
    {
        AppInputText *ait = static_cast<AppInputText*>(
                const_cast<void*>(instance)
            );

        if ((!ait) || (AppConfig::instance().cnf_isstop))
            return false;

        switch(ev->user.code)
        {
            /// Input text box start
            case ID_CMD_POP_MENU100:
            /// Input text box stop
            case ID_CMD_POP_MENU101:
                return ait->runselect(false, -1);
        }
        return false;
    }

bool AppInputText::event(SDL_Event *ev, const void *instance)
{
    AppInputText *ait = static_cast<AppInputText*>(
                const_cast<void*>(instance)
            );

    if (!ait)
        return false;

    auto istate = ait->m_app->state();
    if ((istate[App::AppStateType::STATE_APP_STOP]) ||
        (istate[App::AppStateType::STATE_APP_EDIT]) ||
        (istate[App::AppStateType::STATE_APP_TERM]))
        return false;

    //if (!ait->guiBase::IsRegion(ev, ait->m_app->m_appvideo.guiBase::GetGui<SDL_Rect>()))
        //return false;

    switch(ev->type)
    {
        case SDL_KEYDOWN:
        {
            if (ev->key.keysym.mod != KMOD_NONE)
                for (auto & m : AppConfig::instance().cnf_keymod_alt)
                    if (ev->key.keysym.mod & m)
                        return false;

            switch (ev->key.keysym.sym)
            {
                /// Text input panel (CTRL + ENTER) or (ENTER NUMPAD)
                case SDLK_RETURN:
                case SDLK_RETURN2:
                    {
                        if (ait->runselect(
                                    (SDL_GetModState() & KMOD_CTRL),
                                    ((ev->key.keysym.sym == SDLK_RETURN) ? SDLK_RETURN : -1)
                                ))
                            return true;
                        break;
                    }
                case SDLK_ESCAPE:
                    {
                        if (ait->guiTextInputBox::isactive())
                        {
                            ait->guiTextInputBox::end();
                            ait->guiBase::PushEvent(ID_CMD_POP_MENU102);
                            return true;
                        }
                        break;
                    }
            }
            if (!ait->guiTextInputBox::isactive())
                break;
            return ait->guiTextInputBox::eventcb(ev);
        }
        case SDL_TEXTINPUT:
        {
            if (!ait->guiTextInputBox::isactive())
                break;
            return ait->guiTextInputBox::eventcb(ev);
        }
    }
    return false;
}

