
#include "../../ADBViewer.h"

int32_t guiMain::initm(int32_t w, int32_t h)
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

void guiMain::addpool(guiRenderer_s *gr) noexcept
    {
        gr->id = m_guipool.size();
        m_guipool.push_back(gr);
    }

void guiMain::removepool(int32_t id) noexcept
    {
        m_guipool[id] = nullptr;
    }

void guiMain::draw()
    {
        if (!m_renderer)
            return;

        SDL_RenderClear(m_renderer);
        for (auto &gr : m_guipool)
            if ((gr) && (gr->active) && (gr->texture))
                SDL_RenderCopy(m_renderer, gr->texture, nullptr, &gr->rect);
        SDL_RenderPresent(m_renderer);
    }

void guiMain::events(SDL_Event *ev, SDL_Point *pt)
    {
        for (auto &gr : m_guipool)
            if ((gr) && (gr->active) && (gr->instance))
            {
                guiBase *gb = (guiBase*)gr->instance;
                if ((gb) && (gb->event(ev, pt, gr->instance)))
                    break;
            }
    }

guiMain::guiMain()
     : m_renderer(nullptr), m_window(nullptr), m_event{} {}

guiMain::~guiMain()
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
