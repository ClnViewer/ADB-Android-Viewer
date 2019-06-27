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

int32_t guiMain::initm(SDL_Point *p)
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            return 3;

        if (TTF_Init() < 0)
            return 4;

#       if defined (_BUILD_FRAME_NO_TITLE)
        uint32_t wflag = SDL_WINDOW_BORDERLESS;
#       else
        uint32_t wflag = 0U;
#       endif

        if (!(m_window = SDL_CreateWindow(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_APPFULLNAME,
                    AppConfig::instance().cnf_lang
                ),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                p->x, p->y,
                wflag
            )))
            return 1;

        if (!(m_renderer = SDL_CreateRenderer(
                m_window,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            )))
            return 2;

        AppConfig::instance().cnf_uevent = SDL_RegisterEvents(1);
        if (AppConfig::instance().cnf_uevent == ((uint32_t)-1))
            return 3;

        return 0;
    }

void guiMain::addpool(guiRenderer_s *gr) noexcept
    {
        gr->id = m_guipool.size();
        m_guipool.push_back(gr);
    }

void guiMain::removepool(int32_t id) noexcept
    {
        auto grr = find_if(
                    m_guipool.begin(),
                    m_guipool.end(),
                    [=](guiRenderer_s *gr)
                    {
                        if (!gr)
                            return false;
                        return gr->id == id;
                    }
            );
        if (grr != m_guipool.end())
            m_guipool.erase(grr);
    }

void guiMain::draw()
    {
        if (!m_renderer)
            return;

        SDL_RenderClear(m_renderer);
        for (auto &gr : m_guipool)
            if ((gr) && (gr->active) && (gr->texture))
            {
                guiBase *gb = static_cast<guiBase*>(
                                const_cast<void*>(gr->instance)
                                );
                std::lock_guard<std::mutex> l_lock(gb->m_lock);
                SDL_RenderCopy(m_renderer, gr->texture, nullptr, &gr->rect);
            }

        SDL_RenderPresent(m_renderer);
    }

void guiMain::events(SDL_Event *ev)
    {
        for (auto &gr : m_guipool)
            if ((gr) && (gr->active) && (gr->instance))
            {
                guiBase *gb = static_cast<guiBase*>(
                                const_cast<void*>(gr->instance)
                                );
                if ((gb) && (gb->event(ev, gr->instance)))
                    break;
            }
    }

guiMain::guiMain()
     : m_renderer(nullptr), m_window(nullptr), m_event{} {}

guiMain::~guiMain()
    {
        if (m_renderer)
            SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;

        if (m_window)
            SDL_DestroyWindow(m_window);
        m_window = nullptr;

        if (TTF_WasInit())
            TTF_Quit();
        SDL_Quit();
    }
