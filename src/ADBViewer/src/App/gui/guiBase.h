#pragma once

#define GuiLock(A)                                           \
    {                                                        \
        std::lock_guard<std::mutex> l_lock(guiBase::m_lock); \
        A                                                    \
    }

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
    std::mutex   m_lock;

    guiMain * getgui() const;
    virtual bool tinit(SDL_Texture**);
    virtual bool event(SDL_Event*, const void*);
    virtual bool evresize(SDL_Texture**);

    void ActiveOn();
    void ActiveOff();
    bool IsActive();

    guiBase();
    ~guiBase();
    //guiBase& operator=(guiBase const&) = delete;

protected:
    //
    bool initgui(guiMain*) noexcept;

private:
    //
    guiMain *m_guimain;
};
