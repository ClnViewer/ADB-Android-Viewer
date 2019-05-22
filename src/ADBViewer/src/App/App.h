#pragma once

#include "../ADBViewer.h"

class App : public guiMain, public guiBase
{
public:
    //
    guiStaticText m_info;
    guiBoxTextInput m_input;
    //
    ADBDriver m_adb;
    ADBDriver::Swipe_s m_adb_rect;
    //
    AppMenuBar    m_appmenubar;
    SDL_Point     m_pinput;

    App();
    ~App();

    void run();
    void loop();
    void logo();
    void jointh();
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t) noexcept;
    bool event(SDL_Event*, SDL_Point*, const void*) override;

private:
    //
    std::thread   m_thu;
    std::atomic<bool> m_adbinit;
};
