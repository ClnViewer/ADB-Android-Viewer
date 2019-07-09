#pragma once

#include "../../ADBViewer.h"

class App;

class guiIcon : public guiBase
{
public:
    //
    bool init(App*, SDL_Rect&, ResManager::IndexImageResource, std::function<bool(SDL_Event*, SDL_Rect*)>);
    bool tinit(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool evresize(SDL_Texture**) override;

    bool setcord(SDL_Rect&);
    void On(SDL_Point*);
    void On();
    void Off();

private:
    //
    App                                       *m_app = nullptr;
    ResManager::IndexImageResource             m_resid;
    std::function<bool(SDL_Event*, SDL_Rect*)> m_func;
    //
};

