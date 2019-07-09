#pragma once

#include "../ADBViewer.h"

class App;

class AppCursor : public guiBase
{
public:
    //
    enum CursorType
    {
        APP_CURSOR_HAND = 0,
        APP_CURSOR_ARROW
    };
    //
    ~AppCursor();

    bool init(App*);
    bool uevent(SDL_Event*, const void*) override;
    void Set(AppCursor::CursorType);

private:
    //
    App                  *m_app;
    SDL_Cursor           *m_cursor[2]{};
    AppCursor::CursorType m_idx = AppCursor::CursorType::APP_CURSOR_ARROW;
    //
};

