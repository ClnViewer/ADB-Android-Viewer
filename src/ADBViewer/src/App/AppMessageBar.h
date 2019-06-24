#pragma once

#include "../ADBViewer.h"

class App;

class AppMessageBar : public guiStaticText
{
public:
    //
    AppMessageBar();
    ~AppMessageBar();

    bool init(App*);
    bool evresize(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    void clear();

private:
    //
    App        *m_app;
    AppTimer    m_timer;
    //
};
