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

AppMessageBar::AppMessageBar()
    {
    }

AppMessageBar::~AppMessageBar()
    {
    }

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

void AppMessageBar::clear()
    {
        guiStaticText::clear();
        guiStaticText::ActiveOn();
    }

bool AppMessageBar::evresize(SDL_Texture**)
    {
        clear();
        return false;
    }

bool AppMessageBar::event(SDL_Event *ev, const void *instance)
{
    do
    {
        AppMessageBar *amb = static_cast<AppMessageBar*>(
                    const_cast<void*>(instance)
                );

        if (!amb)
            break;

        if (
            (ev->type == SDL_RENDER_TARGETS_RESET) ||
            (ev->type == SDL_RENDER_DEVICE_RESET)
            )
        {
            (void) amb->evresize(&amb->guiStaticText::gui.texture);
            break;
        }
        if (
            (ev->type != AppConfig::instance().cnf_uevent) ||
            (ev->user.code != ID_CMD_MSGBAR)
            )
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
                    amb->clear();
                }
            );
    }
    while (0);

    return false;
}


