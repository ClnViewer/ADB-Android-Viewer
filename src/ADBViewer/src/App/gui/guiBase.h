#pragma once

class guiMain;
class guiBase;

typedef struct
{
    const void       *instance;
    SDL_Texture      *texture;
    SDL_Rect          rect;
    int32_t           id;
    std::atomic<bool> active;
} guiRenderer_s;

class guiBase
{
public:
    //
    guiRenderer_s gui;

    bool initgui(guiMain*) noexcept;
    virtual bool event(SDL_Event*, SDL_Point*, const void*);
    guiMain * getgui() const;

    guiBase();
    ~guiBase();

private:
    //
    guiMain *m_guimain;
};
