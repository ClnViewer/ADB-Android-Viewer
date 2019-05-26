#pragma once

class guiMain
{
public:
    //
    SDL_Renderer *m_renderer;
    SDL_Window   *m_window;
    SDL_Event     m_event;
    uint32_t      m_uevent;
    //

    void addpool(guiRenderer_s*) noexcept;
    void removepool(int32_t) noexcept;

protected:
    //
    int32_t initm(int32_t, int32_t);
    void draw();
    void events(SDL_Event*);

    guiMain();
    ~guiMain();

private:
    //
    std::vector<guiRenderer_s*> m_guipool;
};
