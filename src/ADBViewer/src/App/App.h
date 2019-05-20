#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gui/guiMain.h"
#include "gui/guiBase.h"
#include "gui/guiStaticText.h"
#include "gui/guiBoxTextInput.h"

class App : public guiMain
{
public:
    //
    guiBase m_base;
    guiStaticText m_info;
    guiBoxTextInput m_input;
    //
    ADBDriver m_adb;
    ADBDriver::Swipe_s m_adb_rect;

    App();
    ~App();

    void run();
    void loop();
    void logo();
    void jointh();
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t) noexcept;

private:
    //
    AppMenu       m_appmenu;
    std::thread   m_thu;
    std::atomic<bool> m_adbinit;
    SDL_Point     m_pinput;
};
