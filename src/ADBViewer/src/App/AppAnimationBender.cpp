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

bool AppAnimationBender::init(App *app)
{
    if (!AppConfig::instance().cnf_disp_bender)
        return guiAnimation::init(app);
    return guiAnimation::init(app, &m_color, ResManager::IndexSpriteResource::RES_SPRITE_BENDER);
}

bool AppAnimationBender::isenabled()
    {
        return guiAnimation::guiBase::IsActive();
    }

void AppAnimationBender::stop()
{
    if (!AppConfig::instance().cnf_disp_bender)
        return;
    guiAnimation::stop();
}

void AppAnimationBender::run()
{
    if (!AppConfig::instance().cnf_disp_bender)
    {
        if (guiAnimation::isinit())
            guiAnimation::stop();
        return;
    }
    guiAnimation::run();
}

bool AppAnimationBender::uevent(SDL_Event *ev, const void *instance)
    {
        AppAnimationBender *aab = static_cast<AppAnimationBender*>(
                const_cast<void*>(instance)
            );

        if (!aab)
            return false;

        if (ev->user.code != ID_CMD_POP_MENU15)
            return false;

        if (AppConfig::instance().cnf_disp_bender.load())
        {
            if (!aab->guiAnimation::isinit())
                if (!aab->guiAnimation::init(
                            aab->guiAnimation::m_app,
                            &aab->m_color,
                            ResManager::IndexSpriteResource::RES_SPRITE_BENDER
                        )
                    )
                    AppConfig::instance().cnf_disp_bender = false;

            if (AppConfig::instance().cnf_disp_bender.load())
                aab->stop();
        }
        else if (aab->isinit())
            aab->stop();

        AddMessageQueue(
            ResManager::stringload(
                ((AppConfig::instance().cnf_disp_bender.load()) ?
                    ResManager::IndexStringResource::RES_STR_BENDER_ON :
                    ResManager::IndexStringResource::RES_STR_BENDER_OFF
                ),
                AppConfig::instance().cnf_lang
            ),
            10U,
            -1
        );
        return true;
    }

bool AppAnimationBender::event(SDL_Event *ev, const void *instance)
{
    AppAnimationBender *aab = static_cast<AppAnimationBender*>(
            const_cast<void*>(instance)
        );

    if (!aab)
        return false;

    if ((ev->type == SDL_RENDER_TARGETS_RESET) || (ev->type == SDL_RENDER_DEVICE_RESET))
    {
        if (aab->guiAnimation::guiBase::IsActive())
        {
            aab->guiBase::ActiveOff();
            aab->guiBase::gui.texture = nullptr;
        }
        return false;
    }
    if (
        (!AppConfig::instance().cnf_isstop) ||
        (!aab->guiAnimation::guiBase::IsActive())
       )
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button == SDL_BUTTON_LEFT)
            {
                SDL_Rect *r = aab->guiAnimation::guiBase::GetGui<SDL_Rect>();
                if (!aab->guiAnimation::guiBase::IsRegion(ev, r))
                    return false;

                /// Bender speak..
                if (
                    (ev->motion.x >=  r->x) &&
                    (ev->motion.x <= (r->x + r->w)) &&
                    (ev->motion.y >=  r->y) &&
                    (ev->motion.y <= (r->y + r->h))
                    )
                {
                    AddMessageQueue(
                        ResManager::speechrandom(),
                        10U,
                        (__LINE__ + ev->motion.x)
                    );
                    return true;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            switch (ev->key.keysym.sym)
            {
                case SDLK_UP:
                case SDLK_RIGHT:
                {
                    aab->guiAnimation::m_key_y -= 5;
                    return true;
                }
                case SDLK_DOWN:
                case SDLK_LEFT:
                {
                    aab->guiAnimation::m_key_y += 5;
                    return true;
                }
            }
            break;
        }
    }
    return false;
}
