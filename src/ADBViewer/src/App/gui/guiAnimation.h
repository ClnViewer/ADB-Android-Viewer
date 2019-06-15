#pragma once

#include "../../ADBViewer.h"

class App;

class guiAnimation : public guiBase
{
public:
    //
    int32_t           m_key_x = 0U;
    int32_t           m_key_y = 0U;

    guiAnimation();
    ~guiAnimation();

    bool init(App*, SDL_Color*, ResManager::IndexSpriteResource);
    virtual void run();
    virtual void stop();

private:
    //
    AppTimer          m_timer;
    App              *m_app = nullptr;
    SDL_Texture     **m_textures = nullptr;
    uint32_t          m_textures_sz = 0U;
    //
    void cleanup();
};
