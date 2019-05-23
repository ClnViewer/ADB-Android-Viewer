#pragma once

#include "../ADBViewer.h"

class App;

class AppMenuPopUp : public guiBase
{
public:
    //
    AppMenuPopUp();
    ~AppMenuPopUp();

    void show();
    bool init(App*);

private:
    //
    App        *m_app;
#   if defined(OS_WIN)
    HWND        m_hwnd;
#   endif
    //
};
