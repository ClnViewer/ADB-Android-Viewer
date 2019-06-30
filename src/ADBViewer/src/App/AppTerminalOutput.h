#pragma once

#include "../ADBViewer.h"

class App;

class AppTerminalOutput : public guiBase
{
public:
    //
    bool init(App*);
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;

private:
    //
    App        *m_app;
    //
};

