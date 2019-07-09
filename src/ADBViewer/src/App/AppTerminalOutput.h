#pragma once

#include "../ADBViewer.h"

class App;

class AppTerminalOutput : public guiBase
{
public:
    //
    bool init(App*, SDL_Rect&);
    void start();
    void stop();
    void draw(std::string const &);

private:
    //
    App        *m_app;
    //
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
};

