#pragma once

#include "../../ADBViewer.h"

class App;

class guiIconTrigger : public guiBase
{
public:
    //
    ~guiIconTrigger();

    bool init(App*, SDL_Rect&, SDL_Color*, ResManager::IndexSpriteResource, std::function<bool(SDL_Event*, SDL_Rect*)>);
    bool tinit(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool evresize(SDL_Texture**) override;

    void On(SDL_Point*, uint32_t);
    void On(SDL_Point*);
    void On(uint32_t);
    void On();
    void Off();

private:
    //
    App                                       *m_app = nullptr;
    std::function<bool(SDL_Event*, SDL_Rect*)> m_func;
    SDL_Texture                              **m_textures = nullptr;
    uint32_t                                   m_textures_sz = 0U;
    uint32_t                                   m_lastid = 0U;
    //
    void cleanup();
};

