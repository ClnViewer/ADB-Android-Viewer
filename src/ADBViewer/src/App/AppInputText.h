#pragma once

#include "../ADBViewer.h"

class App;

class AppInputText : public guiTextInputBox
{
public:
    //
    bool init(App*);
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;
    bool isenabled();

private:
    //
    App        *m_app;
    //
    bool runselect(bool, int32_t);
};

