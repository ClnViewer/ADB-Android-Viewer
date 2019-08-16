#pragma once

#include "../../ADBViewer.h"

class AppBrowserPage : public guiPage
{
public:
    //
    enum MenuKey
    {
        MENUKEY_NONE = 0,
        MENUKEY_FILE,
        MENUKEY_INFO,
        MENUKEY_APK
    };
    enum MenuInput
    {
        MENUINPUT_NONE = 0,
        MENUINPUT_PC,
        MENUINPUT_ANDROID,
        MENUINPUT_INFO,
        MENUINPUT_APK_ENABLED,
        MENUINPUT_APK_DISABLED,
        MENUINPUT_APK_SYSTEM,
        MENUINPUT_APK_BRAND
    };
    //
    bool              init(SDL_Window*);
    //
    MenuKey           mskey = MenuKey::MENUKEY_NONE;
    MenuInput         mikey = MenuInput::MENUINPUT_NONE;

    SDL_Rect          rbase{};
    SDL_Rect          frame{};
    SDL_Rect          menu4{};
    SDL_Rect          menu9{};
    //
    SDL_Point         minsize{};
    //
    SDL_Rect          list_rdst{};

private:
    //
    bool getwin(SDL_Window*);
};

