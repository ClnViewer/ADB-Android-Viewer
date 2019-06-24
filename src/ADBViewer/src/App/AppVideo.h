#pragma once

#include "../ADBViewer.h"

class App;

class AppVideo : public guiBase
{
public:
    //
    AppVideo();
    ~AppVideo();

    bool init(App*);
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
    void run();
    void stop();
    bool event(SDL_Event*, const void*) override;

private:
    //
    App         *m_app;
    std::thread  m_thu;
    //
    void jointh();
    bool defscreen();
    bool defscreen(SDL_Texture**);
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t);
    //
};
