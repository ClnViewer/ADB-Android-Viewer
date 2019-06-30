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
    guiAnimation& operator=(guiAnimation const&) = delete;

    bool init(App*);
    bool init(App*, SDL_Color*, ResManager::IndexSpriteResource);
    bool isinit();

    virtual void run();
    virtual void stop();

protected:
    //
    App              *m_app = nullptr;

private:
    //
    AppTimer          m_timer;
    SDL_Texture     **m_textures = nullptr;
    uint32_t          m_textures_sz = 0U;
    //
    void cleanup();
};
