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
    bool IsFocus(SDL_Rect*);
    bool IsFocus();
    void PushEvent(int32_t);
    void PushEvent(int32_t, void*);

    bool SurfaceInit(SDL_Surface**, SDL_Point const &, SDL_Color const &, int32_t = 32);
    bool SurfaceInit(SDL_Surface**, SDL_Point const &, SDL_Color const &, std::mutex&, int32_t = 32);
    bool TextureInit(SDL_Texture**, SDL_Surface*, std::mutex&);

    template <typename T> T * GetGui();

protected:
    //
    bool initgui(guiMain*) noexcept;

private:
    //
    guiMain *m_guimain;
    //
    bool          IsPosition(SDL_Point&, SDL_Rect*);
    SDL_Surface * SurfaceCreate(SDL_Point const &, SDL_Color const &, int32_t = 32);
};

extern template guiMain * guiBase::GetGui<guiMain>();
extern template SDL_Rect * guiBase::GetGui<SDL_Rect>();
extern template SDL_Window * guiBase::GetGui<SDL_Window>();
extern template SDL_Renderer * guiBase::GetGui<SDL_Renderer>();
extern template guiRenderer_s * guiBase::GetGui<guiRenderer_s>();

