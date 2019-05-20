#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gui/guiMain.h"
#include "gui/guiBase.h"
#include "gui/guiStaticText.h"
#include "gui/guiBoxTextInput.h"

class App;

class AppMenu : public guiBase
{
public:
    //
    std::atomic<bool> m_isrun;
    std::atomic<bool> m_isstop;
    std::atomic<bool> m_ispos;
    std::atomic<bool> m_isfullscreen;
    std::atomic<uint32_t> m_scale;
    std::atomic<uint32_t> m_compress;

    AppMenu();
    ~AppMenu();

    bool init(App*);
    bool mouse();
    bool screenshot();
    void setcursor(uint32_t);
    void infoset(MgrType, std::string const&, int32_t);

private:
    //
    App        *m_app;
    SDL_Color   m_color[2];
    SDL_Cursor *m_cursor[2];
    //
    ResManager::IndexStringResource clickpos(int32_t, int32_t, int32_t);
    void mousemove();
    void mousebutton();

    void settitle(std::string const&);

    bool savefile(std::string&);
    bool openfile(std::string&);
};
