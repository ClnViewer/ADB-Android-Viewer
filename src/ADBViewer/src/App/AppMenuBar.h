#pragma once

#include "../ADBViewer.h"

class App;

class AppMenuBar : public guiBase
{
public:
    //
    AppMenuBar();
    ~AppMenuBar();

    bool init(App*);
    bool screenshot(SDL_Event*, bool);
    void setcursor(uint32_t);
    void infoset(MgrType, std::string const&, int32_t, SDL_Event *ev);
    bool event(SDL_Event*, const void*) override;

private:
    //
    App        *m_app;
    SDL_Color   m_color[2];
    SDL_Cursor *m_cursor[2];
    //
    ResManager::IndexStringResource clickpos(int32_t, int32_t, int32_t, int32_t);
    bool mousemove(SDL_Event*, AppMenuBar*);
    bool mousebutton(SDL_Event*, AppMenuBar*, int32_t);

    void settitle(std::string const&);

    bool savefile(std::string&);
    bool openfile(std::string&);
};
