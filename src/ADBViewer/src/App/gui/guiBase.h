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
    SDL_Rect         *rsrc;
    SDL_Rect          rdst;
    int32_t           id;
    std::atomic<bool> active;
} guiRenderer_s;

class guiBase
{
public:
    //
    guiRenderer_s gui{};
    std::mutex    m_lock;

    guiBase();
    ~guiBase();
    guiBase& operator=(guiBase const&) = delete;

    virtual bool tinit(SDL_Texture**);
    virtual bool event(SDL_Event*, const void*);
    virtual bool uevent(SDL_Event*, const void*);
    virtual bool evresize(SDL_Texture**);

    void ActiveOn();
    void ActiveOff();
    bool IsActive();
    bool IsRegion(SDL_Event*, SDL_Rect*);
    bool IsRegion(SDL_Event*);
    void PushEvent(int32_t);

    template <typename T> T * GetGui();

protected:
    //
    bool initgui(guiMain*) noexcept;

private:
    //
    guiMain *m_guimain;
};

extern template guiMain * guiBase::GetGui<guiMain>();
extern template SDL_Rect * guiBase::GetGui<SDL_Rect>();
extern template SDL_Window * guiBase::GetGui<SDL_Window>();
extern template SDL_Renderer * guiBase::GetGui<SDL_Renderer>();
extern template guiRenderer_s * guiBase::GetGui<guiRenderer_s>();

