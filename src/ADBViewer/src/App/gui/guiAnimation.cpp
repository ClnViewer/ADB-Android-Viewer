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

guiAnimation::guiAnimation()
{
}

guiAnimation::~guiAnimation()
{
    cleanup();
}

void guiAnimation::cleanup()
{
    guiBase::ActiveOff();

    if (m_timer.isactive())
        m_timer.stop();

    guiBase::gui.texture = nullptr;

    if ((!m_textures) || (!m_textures_sz))
        return;

    for (uint32_t i = 0U; i < m_textures_sz; i++)
        if (m_textures[i])
            SDL_DestroyTexture(m_textures[i]);

    delete [] m_textures;
    m_textures = nullptr;
}

bool guiAnimation::isinit()
    {
        return !((!m_textures) || (!m_textures_sz));
    }

bool guiAnimation::init(App *app)
    {
        if (!app)
            return false;
        m_app = ((!m_app) ? app : m_app);
        guiBase::gui.texture = nullptr;
        return guiBase::initgui(app);
    }

bool guiAnimation::init(App *app, SDL_Color *bcolor, ResManager::IndexSpriteResource res)
{
    if (!app)
        return false;

    m_app = ((!m_app) ? app : m_app);
    guiBase::gui.texture = nullptr;

    do
    {
        bool isvalid = true;
        SDL_Surface **l_sprites  = nullptr;

        if (
            (!(l_sprites = ResManager::spriteload(res, bcolor, &m_textures_sz))) ||
            (!m_textures_sz)
            )
            break;

        guiBase::gui.rect.w = l_sprites[0]->w;
        guiBase::gui.rect.h = l_sprites[0]->h;

        m_textures = new SDL_Texture*[m_textures_sz]{};

        for (uint32_t i = 0U; i < m_textures_sz; i++)
        {
            if (l_sprites[i])
            {
                if (isvalid)
                    if (!(m_textures[i] = SDL_CreateTextureFromSurface(
                                            guiBase::GetGui<SDL_Renderer>(),
                                            l_sprites[i]
                        )))
                        isvalid = false;

                SDL_FreeSurface(l_sprites[i]);
                l_sprites[i] = nullptr;
            }
        }
        delete [] l_sprites;

        if (!isvalid)
        {
            cleanup();
            return false;
        }
    }
    while (0);

    bool ret = guiBase::initgui(app);
    guiBase::ActiveOff();

    if (ret)
        run();

    return ret;
}

void guiAnimation::stop()
{
    if (m_timer.isactive())
        m_timer.stop();

    guiBase::ActiveOff();
}

void guiAnimation::run()
{
    if ((!m_textures) || (!m_textures_sz) || (m_timer.isactive()))
        return;

    m_key_y = 0U;

    m_timer.loop(
        0U,
        30U,
        0U,
        [=]()
        {
            do
            {
                if (!m_app)
                    break;

                SDL_Rect *r   = guiBase::GetGui<SDL_Rect>();
                SDL_Rect *rw  = m_app->m_appvideo.guiBase::GetGui<SDL_Rect>();
                int32_t   pad = __MENU_W_default,
                          l_key_y = (rw->h - r->h);
                r->x = (pad + m_key_x);
                r->y = l_key_y;
                pad += rw->w;

                for (uint32_t i = 0U; i < m_textures_sz; i++)
                {
                    if (i == (m_textures_sz - 1U))
                        i = 0U;

                    guiBase::ActiveOff();
                    GuiLock(
                        guiBase::gui.texture = m_textures[i];
                        r->x += (5 + m_key_x);
                        r->y = (m_key_y + l_key_y);
                    );

                    if (r->x >= pad)
                        break;

                    if (!AppConfig::instance().cnf_isrun)
                        break;

                    guiBase::ActiveOn();
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(80));

                    if (!AppConfig::instance().cnf_isrun)
                        break;
                }
            }
            while (0);

            guiBase::ActiveOff();
            guiBase::gui.texture = nullptr;
        }
    );
}
