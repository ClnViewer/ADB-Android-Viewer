#pragma once

#include "../../ADBViewer.h"

class App;

class AppTerminalPageNuber : public guiBase
{
public:
    //
    bool init(App*, AppTerminalPage*, std::function<void()>);
    // bool event(SDL_Event*, const void*) override;
    bool evresize(SDL_Texture**) override;
    void draw();
    void stop();

private:
    //
    App                  *m_app = nullptr;
    TTF_Font             *m_font = nullptr;  // auto remove from cache in DLL
    SDL_Color            *m_color = nullptr;
    SDL_Color             m_color_disabled = { 64, 64, 64, 0 }; // not texture focus color
    AppTerminalPage      *m_page = nullptr;
    std::function<void()> m_func = nullptr;
    //
};

