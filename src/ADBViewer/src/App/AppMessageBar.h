#pragma once

#include "../ADBViewer.h"

class App;

class AppMessageBar : public guiStaticText
{
public:
    //
    bool init(App*);
    bool evresize(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;
    void clear();
    void PrintInfo(MgrType, std::string const&, int32_t, SDL_Event*);

private:
    //
    App        *m_app;
    AppTimer    m_timer;
    //
    void settitle(std::string const&);
};
