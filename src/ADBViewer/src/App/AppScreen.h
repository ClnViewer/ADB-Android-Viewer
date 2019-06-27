#pragma once

#include "../ADBViewer.h"

class App;

class AppScreen : public guiBase
{
public:
    //
    bool init(App*);
    bool event(SDL_Event*, const void*) override;

    bool screenshot(bool);
    bool screencopy();

private:
    //
    App        *m_app;
    //
    uint32_t      getpixel(SDL_Surface*, int32_t, int32_t);
    SDL_Surface * getscreen();
    void        * getsysscreen(SDL_Surface*);
    void          freesysscreen(void*);
    bool          setclipboard(void*);
};

