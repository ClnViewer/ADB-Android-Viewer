#pragma once

#include "../../ADBViewer.h"

class App;

class AppTerminalOutput : public guiBase
{
public:
    //
    ~AppTerminalOutput();

    bool init(App*, AppTerminalPage*);
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    void start();
    void stop();
    void pageUp();
    void pageDown();
    void draw_cmd(std::string const &);
    void draw_txt(std::string const &);

private:
    //
    App                 *m_app = nullptr;
    SDL_Surface         *m_surface = nullptr;
    TTF_Font            *m_font = nullptr;  // auto remove from cache in DLL
    SDL_Color           *m_color_cmd = nullptr;
    SDL_Color           *m_color_txt = nullptr;
    SDL_Color            m_color_black = { 0x0, 0x0, 0x0, 0x0 };
    AppTerminalPage     *m_page = nullptr;
    AppTerminalPageNuber m_pagenum;
    std::mutex           s_lock;
    //
    void drawline(std::string const &, SDL_Color*, bool);
};

