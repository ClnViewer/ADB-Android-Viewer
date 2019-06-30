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

guiMain * guiBase::getgui() const
    {
        return m_guimain;
    }

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
            SDL_DestroyTexture(gui.texture);
        gui.texture = nullptr;
    }
