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

bool guiBase::initgui(guiMain *gm) noexcept
    {
        if ((m_guimain) || (!gm))
            return guiBase::IsActive();

        gui.instance = this;
        gui.active = true;
        m_guimain = gm;

        /// virtual, create in all method
        if (!tinit(&gui.texture))
            return false;

        m_guimain->addpool(&gui);
        return gui.active.load();
    }

bool guiBase::tinit(SDL_Texture **texture)
    {
        /// virtual, default GUI disabled, EVENT enable
        *texture = nullptr;
        return true;
    }

bool guiBase::evresize(SDL_Texture**)
    {
        /// virtual, default resize GUI from EVENT call
        return false;
    }

bool guiBase::event(SDL_Event *ev, const void*)
    {
        if (
            (ev->type == SDL_RENDER_TARGETS_RESET) ||
            (ev->type == SDL_RENDER_DEVICE_RESET)
            )
            return evresize(&gui.texture);
        return false;
    }

bool guiBase::uevent(SDL_Event *ev, const void*)
    {
        return false;
    }

void guiBase::ActiveOn()
    {
        if (!gui.active)
            gui.active = true;
    }

void guiBase::ActiveOff()
    {
        if (gui.active)
            gui.active = false;
    }

bool guiBase::IsActive()
    {
        return gui.active.load();
    }

bool guiBase::IsRegion(SDL_Event *ev)
    {
        return IsRegion(ev, nullptr);
    }

bool guiBase::IsRegion(SDL_Event *ev, SDL_Rect *irect)
    {
        SDL_Rect *rect = ((!irect) ? &gui.rect : irect);

        if ((!rect->w) || (!rect->h))
            return false;
        if (
            ((ev->motion.x >= rect->x) && (ev->motion.x <= (rect->x + rect->w))) &&
            ((ev->motion.y >= rect->y) && (ev->motion.y <= (rect->y + rect->h)))
           )
            return true;
        return false;
    }

void guiBase::PushEvent(int32_t id)
    {
        SDL_Event cmdEvent{};
        cmdEvent.type = AppConfig::instance().cnf_uevent;
        cmdEvent.user.code = id;
        SDL_PushEvent(&cmdEvent);
    }

template<typename T>
T * guiBase::GetGui()
    {
        if constexpr (std::is_same<T, SDL_Window>::value)
            return m_guimain->m_window;
        else if constexpr (std::is_same<T, SDL_Renderer>::value)
            return m_guimain->m_renderer;
        else if constexpr (std::is_same<T, SDL_Rect>::value)
            return &gui.rect;
        else if constexpr (std::is_same<T, guiRenderer_s>::value)
            return &gui;
        else if constexpr (std::is_same<T, guiMain>::value)
            return m_guimain;
        else
            return nullptr;
    }

template guiMain * guiBase::GetGui<guiMain>();
template SDL_Rect * guiBase::GetGui<SDL_Rect>();
template SDL_Window * guiBase::GetGui<SDL_Window>();
template SDL_Renderer * guiBase::GetGui<SDL_Renderer>();
template guiRenderer_s * guiBase::GetGui<guiRenderer_s>();

guiBase::guiBase()
     :  gui{}, m_guimain(nullptr)
    {
        gui.active = false;
        gui.texture = nullptr;
    }

guiBase::~guiBase()
    {
        if (m_guimain)
            m_guimain->removepool(gui.id);

        if (gui.texture)
        {
            std::lock_guard<std::mutex> l_lock(m_lock);
            SDL_DestroyTexture(gui.texture);
        }
        gui.texture = nullptr;
    }
