#pragma once

#include "../ADBViewer.h"

class App;

class AppAnimationBender : public guiAnimation
{
public:
    //
    bool init(App*);
    void run() override;
    void stop() override;
    bool event(SDL_Event*, const void*) override;

private:
    //
    SDL_Color         m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    //
};
