#pragma once

#include <SDL2/SDL.h>

class guiBase
{
public:
    //
    guiRenderer_s gui;

    bool initgui(guiMain *gm) noexcept
    {
        if ((m_guimain) || (!gm))
            return gui.active.load();

        m_guimain = gm;
        gui.id = m_guimain->addpool(&gui);
        gui.active = true;
        return gui.active.load();
    }
    guiMain * getgui() const
    {
        return m_guimain;
    }
    guiBase()
     :  gui{}, m_guimain(nullptr)
    {
        gui.active = false;
        gui.texture = nullptr;
    }
    ~guiBase()
    {
        if (m_guimain)
            m_guimain->removepool(gui.id);

        if (gui.texture)
            SDL_DestroyTexture(gui.texture);
        gui.texture = nullptr;
    }

private:
    //
    guiMain *m_guimain;
};
