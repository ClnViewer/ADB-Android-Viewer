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

    guiMain * getgui() const;
    virtual bool event(SDL_Event*, const void*);

    guiBase();
    ~guiBase();

protected:
    //
    bool initgui(guiMain*) noexcept;

private:
    //
    guiMain *m_guimain;
};
