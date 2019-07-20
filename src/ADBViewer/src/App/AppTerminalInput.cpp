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

bool AppTerminalInput::init(App *app, AppTerminalPage *atp, std::function<void(std::string&)> f)
    {
        if ((!app) || (!atp))
            return false;

        m_app = app;
        m_func = f;
        m_page = atp;

        bool ret = false;
        do
        {
            ret = guiTextInputBox::init(
                    app,
                    m_page->c_p_input,
                    m_page->getprompt(),
                    ResManager::IndexFontResource::RES_FONT_CONSOLAS,
                    ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE,
                    ResManager::IndexColorResource::RES_COLOR_GREEN_CURSOR
            );
            if (!ret)
                break;

            ret = m_icon_editmenu.init(
                    app,
                    m_page->btn_r_edit,
                    ResManager::IndexImageResource::RES_IMG_EDITMENU,
                    [=](SDL_Event *ev, SDL_Rect *r)
                    {
                        switch (ev->type)
                        {
                            case SDL_MOUSEMOTION:
                                {
                                    guiBase::PushEvent(ID_CMD_POP_MENU27);
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
                                                if (!AppConfig::instance().cnf_term_cmd.size())
                                                    break;

                                                if (AppConfig::instance().cnf_term_cmd_idx >= AppConfig::instance().cnf_term_cmd.size())
                                                    AppConfig::instance().cnf_term_cmd_idx = 0U;
                                                settext(
                                                    AppConfig::instance().cnf_term_cmd[AppConfig::instance().cnf_term_cmd_idx]
                                                );
                                                AppConfig::instance().cnf_term_cmd_idx++;
                                                break;
                                            }
                                        case 41  ... 71:
                                            {
                                                (void) guiTextInputBox::getresult(m_page->getprompt());
                                                break;
                                            }
                                        case 78  ... 107:
                                            {
                                                if (m_func)
                                                {
                                                    if (guiTextInputBox::isresult())
                                                    {
                                                        std::string s = guiTextInputBox::getresult(m_page->getprompt());
                                                        if (!s.empty())
                                                            m_func(s);
                                                    }
                                                }
                                                else
                                                    (void) guiTextInputBox::getresult(m_page->getprompt());
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
        if (guiTextInputBox::isactive())
            return;
        guiTextInputBox::begin();

        SDL_Point point;
        point.x = m_page->btn_r_edit.x;
        point.y = m_page->btn_r_edit.y;
        m_icon_editmenu.On(&point);
    }

void AppTerminalInput::stop()
    {
        if (!guiTextInputBox::isactive())
            return;
        guiTextInputBox::end();
        m_icon_editmenu.Off();
    }
