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
    bool update(std::vector<uint8_t>&, uint32_t, uint32_t) noexcept;
    //
};
