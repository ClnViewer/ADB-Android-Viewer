#pragma once

class guiMain
{
public:
    //
    SDL_Renderer *m_renderer;
    SDL_Window   *m_window;
    SDL_Event     m_event;
    //

    void addpool(guiRenderer_s*) noexcept;
    void removepool(int32_t) noexcept;

    void flushicon(int32_t);

protected:
    //
    int32_t initm(SDL_Point*);
    void draw();
    void events(SDL_Event*);

    guiMain();
    ~guiMain();
    guiMain& operator=(guiMain const&) = delete;

private:
    //
    std::vector<guiRenderer_s*> m_guipool;
};
