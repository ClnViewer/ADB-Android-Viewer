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

bool AppTerminalInput::init(App *app, SDL_Rect & irect)
    {
        if (!app)
            return false;

        m_app = app;
        SDL_Point point = { irect.x, irect.y };
        bool ret = false;

        do
        {
            ret = guiTextInputBox::init(
                    app,
                    point,
                    "> ",
                    ResManager::IndexFontResource::RES_FONT_CONSOLAS,
                    ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE,
                    ResManager::IndexColorResource::RES_COLOR_GREEN_CURSOR
            );
            if (!ret)
                break;

            SDL_Rect rect{};
            rect.x = 0;
            rect.y = 0;

            ret = m_icon_editmenu.init(
                    app,
                    rect,
                    ResManager::IndexImageResource::RES_IMG_EDITMENU,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        switch (ev->type)
                        {
                            case SDL_MOUSEMOTION:
                                {
                                    guiBase::PushEvent(ID_CMD_POP_MENU26);
                                    return true;
                                }
                            case SDL_MOUSEBUTTONDOWN:
                                {
                                    int32_t pos = (ev->motion.y - r->y);
#                                   pragma GCC diagnostic push
#                                   pragma GCC diagnostic ignored "-Wpedantic"
                                    switch (pos)
                                    {
                                        case 5   ... 35:
                                            {
                                                (void) guiTextInputBox::getresult("> ");
                                                break;
                                            }
                                        case 40  ... 70:
                                            {
                                                if (guiTextInputBox::isresult())
                                                    AppConfig::instance().cnf_adb.SendTextASCII(
                                                                guiTextInputBox::getresult("> ")
                                                        );
                                                break;
                                            }
                                        default:
                                            break;
                                    }
#                                   pragma GCC diagnostic pop
                                    return true;
                                }
                        }
                        return false;
                    }
                );
        }
        while (0);
        return ret;
    }

bool AppTerminalInput::isenabled()
    {
        return guiTextInputBox::isactive();
    }

void AppTerminalInput::start()
    {
        start(nullptr);
    }

void AppTerminalInput::start(SDL_Point *p)
    {
        if (guiTextInputBox::isactive())
            return;
        guiTextInputBox::begin();

        if (p)
            m_icon_editmenu.On(p);
        else
        {
            SDL_Point point{};
            SDL_GetWindowSize(guiTextInputBox::guiBase::GetGui<SDL_Window>(), &point.x, &point.y);
            point.x = 0;
            point.y -= __EMENU_H_default;
            m_icon_editmenu.On(p);
        }
    }

void AppTerminalInput::stop()
    {
        if (!guiTextInputBox::isactive())
            return;
        guiTextInputBox::end();
        m_icon_editmenu.Off();
    }
