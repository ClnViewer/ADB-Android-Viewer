
#include "../../ADBViewer.h"

bool guiBase::initgui(guiMain *gm) noexcept
    {
        if ((m_guimain) || (!gm))
            return gui.active.load();

        gui.instance = this;
        gui.active = true;

        m_guimain = gm;
        m_guimain->addpool(&gui);
        return gui.active.load();
    }

bool guiBase::event(SDL_Event*, SDL_Point*, const void*)
    {
        return false;
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
