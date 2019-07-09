#pragma once

#include "../ADBViewer.h"
#define APP_STATE_SIZE 7

class App : public guiMain, public guiBase
{
public:
    //
    enum AppStateType
    {
        STATE_APP_EXIT = 0,        /// 0 - Application exit
        STATE_APP_STOP,            /// 1 - No session run
        STATE_APP_EDIT,            /// 2 - Script editor active
        STATE_APP_INPUT,           /// 3 - Input field active
        STATE_APP_TERM,            /// 4 - Terminal active
        STATE_APP_MENU,            /// 5 - Menu bar active
        STATE_APP_BENDER           /// 6 - Menu bar active
    };
    //
    AppCursor          m_appcursor;
    AppInputText       m_appinput;
    AppVideo           m_appvideo;
    AppEditor          m_appeditor;
    AppMenuBar         m_appmenubar;
    AppMenuPopUp       m_appmenupop;
    AppMessageBar      m_appmsgbar;
    AppAnimationBender m_appabender;
    AppScreen          m_appscreen;
    AppTerminal        m_appterminal;

    App();

    void loop();
    bool event(SDL_Event*, const void*) override;
    std::array<bool, APP_STATE_SIZE> const state();

private:
    //
};
