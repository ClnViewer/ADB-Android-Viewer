
#include "../../ADBViewer.h"

bool guiBase::initgui(guiMain *gm) noexcept
    {
        if ((m_guimain) || (!gm))
            return guiBase::IsActive();

        gui.instance = this;
        gui.active = true;

        /// virtual, create in all method
        if (!tinit(&gui.texture))
            return false;

        m_guimain = gm;
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
