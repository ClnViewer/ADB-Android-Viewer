#pragma once

class guiMain
{
public:
    //
    SDL_Renderer *m_renderer;
    SDL_Window   *m_window;
    SDL_Event     m_event;
    //
    int32_t initm(int32_t, int32_t);
    void addpool(guiRenderer_s*) noexcept;
    void removepool(int32_t) noexcept;
    void draw();
    void events(SDL_Event*, SDL_Point*);

    guiMain();
    ~guiMain();

private:
    //
    std::vector<guiRenderer_s*> m_guipool;
};
