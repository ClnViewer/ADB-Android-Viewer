#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Texture      *texture;
    SDL_Rect          rect;
    int32_t           id;
    std::atomic<bool> active;
} guiRenderer_s;

class guiMain
{
public:
    //
    SDL_Renderer *m_renderer;
    SDL_Window   *m_window;
    SDL_Event     m_event;
    //
    int32_t initm(int32_t w, int32_t h)
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
            return 3;

        if (TTF_Init() < 0)
            return 4;

#       if defined (_BUILD_FRAME_NO_TITLE)
        uint32_t wflag = SDL_WINDOW_BORDERLESS;
#       else
        uint32_t wflag = 0U;
#       endif

        if (!(m_window = SDL_CreateWindow(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_APPFULLNAME
                ),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                w, h,
                wflag
            )))
            return 1;

        if (!(m_renderer = SDL_CreateRenderer(
                m_window,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            )))
            return 2;
        return 0;
    }
    int32_t addpool(guiRenderer_s *gr) noexcept
    {
        int32_t ret = m_guipool.size();
        m_guipool.push_back(gr);
        return ret;
    }
    void removepool(int32_t id) noexcept
    {
        m_guipool.erase(m_guipool.begin() + id);
    }
    void draw()
    {
        if (!m_renderer)
            return;

        SDL_RenderClear(m_renderer);
        for (auto &gr : m_guipool)
            if ((gr->active) && (gr->texture))
                SDL_RenderCopy(m_renderer, gr->texture, nullptr, &gr->rect);
        SDL_RenderPresent(m_renderer);
    }

    guiMain()
     : m_renderer(nullptr), m_window(nullptr), m_event{} {}
    ~guiMain()
    {
        if (m_renderer)
            SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;

        if (m_window)
            SDL_DestroyWindow(m_window);
        m_window = nullptr;

        if (TTF_WasInit())
            TTF_Quit();
        SDL_Quit();
    }

private:
    //
    std::vector<guiRenderer_s*> m_guipool;
};
