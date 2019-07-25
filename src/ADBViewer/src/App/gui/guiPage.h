#pragma once

class guiPage
{
public:
    //
    SDL_Point getoffset(SDL_Rect & r)
    {
        return { .x = r.x, .y = r.y };
    }

    SDL_Point getsize(SDL_Rect & r)
    {
        return { .x = r.w, .y = r.h };
    }

    SDL_Point getdisplay()
    {
        SDL_DisplayMode dm{};
        if (SDL_GetCurrentDisplayMode(0, &dm) < 0)
            return { .x = -1, .y = -1 };
        return { .x = dm.w, .y = dm.h };
    }

    SDL_Rect getwindow(SDL_Window *sdlw)
    {
        SDL_Rect r{};
        SDL_GetWindowPosition(sdlw, &r.x, &r.y);
        SDL_GetWindowSize(sdlw, &r.w, &r.h);
        return r;
    }
};


