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

bool AppMessageBar::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        bool ret = guiStaticText::init(
                app,
                ResManager::IndexFontResource::RES_FONT_16704,
                ResManager::IndexColorResource::RES_COLOR_GREEN_BLACK
                );
        guiStaticText::ActiveOn();
        return ret;
}

void AppMessageBar::clean()
    {
        guiStaticText::clear();
        guiStaticText::ActiveOn();
    }

bool AppMessageBar::evresize(SDL_Texture**)
    {
        clean();
        return false;
    }

void AppMessageBar::PrintInfo(MgrType mgrt, std::string const & s, int32_t id, SDL_Event *ev)
    {
    std::stringstream ss;
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
#   else
    ss << ResManager::stringload(
            ResManager::IndexStringResource::RES_STR_APPTITLENAME,
            AppConfig::instance().cnf_lang
        );
    ss << "v." << AVIEW_FULLVERSION_STRING;
    ss << " r." << AVIEW_SVN_REVISION;
#   endif

    switch(mgrt)
    {
        case MgrType::MGR_MAIN:
        {
            SDL_Rect *r = m_app->m_appvideo.guiBase::GetGui<SDL_Rect>();
            SDL_Point point_img_menu = ResManager::imagesize(
                    ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
            );

            if (ev->motion.x > point_img_menu.x)
            {
                if ((!AppConfig::instance().cnf_isstop) && (AppConfig::instance().cnf_ispos))
                {
                    uint32_t x = ((AppConfig::instance().cnf_disp_ratio) ?
                        ((ev->motion.x - point_img_menu.x) * AppConfig::instance().cnf_disp_ratio) :
                         (ev->motion.x - point_img_menu.x)
                        );
                    uint32_t y = ((AppConfig::instance().cnf_disp_ratio) ?
                        (ev->motion.y * AppConfig::instance().cnf_disp_ratio) :
                         ev->motion.y
                        );
                    uint32_t w = ((AppConfig::instance().cnf_disp_ratio) ?
                        (r->w * AppConfig::instance().cnf_disp_ratio) :
                         r->w
                        );
                    uint32_t h = ((AppConfig::instance().cnf_disp_ratio) ?
                        (r->h * AppConfig::instance().cnf_disp_ratio) :
                         r->h
                        );
#                   if !defined (_BUILD_FRAME_NO_TITLE)
                    ss << " - ";
#                   endif
                    ss << "( " << w << "x" << h << " )";
                    ss << " - " << "( X: " << x << " Y: " << y << " )";
                }
#               if defined (_BUILD_FRAME_NO_TITLE)
                else
                {
                    m_app->m_appmsgbar.clean();
                    return;
                }
#               endif
            }
            else
            {
                /*
#               if defined (_BUILD_FRAME_NO_TITLE)
                m_app->m_appmsgbar.clear();
#               endif
                */
                return;
            }
            break;
        }
        case MgrType::MGR_MENU:
        {
#           if defined (_BUILD_FRAME_NO_TITLE)
            if (s.empty())
            {
                m_app->m_appmsgbar.clean();
                return;
            }
            ss << s.c_str();
#           else
            if (!s.empty())
                ss << " - " << s.c_str();
#           endif
            break;
        }
        default:
            {
                /*
#               if defined (_BUILD_FRAME_NO_TITLE)
                m_app->m_appmsgbar.clear();
#               endif
                */
                return;
            }
    }
#   if defined (_BUILD_FRAME_NO_TITLE)
    ss << "  ";
    AddMessageQueue(
        ss.str(),
        3U,
        id
    );
#   else
    settitle(ss.str());
#   endif
    }

void AppMessageBar::settitle(std::string const & s)
    {
        SDL_SetWindowTitle(guiBase::GetGui<SDL_Window>(), s.c_str());
    }

bool AppMessageBar::uevent(SDL_Event *ev, const void *instance)
{
    do
    {
        AppMessageBar *amb = static_cast<AppMessageBar*>(
                    const_cast<void*>(instance)
                );

        if (!amb)
            break;

        if (ev->user.code != ID_CMD_MSGBAR)
            break;

        if (!AppMessageQueue::instance().checkData())
            break;

        AppMessageQueue::DataMessageQueue dataq{};

        if (!AppMessageQueue::instance().getData(dataq))
            break;

        amb->guiStaticText::draw(dataq.s, &dataq.cord, dataq.id);
        amb->guiStaticText::guiBase::ActiveOn();

        if (amb->m_timer.isactive())
            amb->m_timer.stop();

        if (dataq.tm)
            amb->m_timer.once(
                dataq.tm,
                0U,
                [=]()
                {
                    amb->clean();
                }
            );
        return true;
    }
    while (0);

    return false;
}

bool AppMessageBar::event(SDL_Event *ev, const void *instance)
    {
        AppMessageBar *amb = static_cast<AppMessageBar*>(
                    const_cast<void*>(instance)
                );

        if (!amb)
            return false;

        if ((ev->type == SDL_RENDER_TARGETS_RESET) || (ev->type == SDL_RENDER_DEVICE_RESET))
            (void) amb->evresize(&amb->guiStaticText::gui.texture);
        return false;
    }
