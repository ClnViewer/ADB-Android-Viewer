#pragma once

#include "../ADBViewer.h"

class App;

class AppInputText : public guiTextInputBox
{
public:
    //
    bool init(App*);
    bool event(SDL_Event*, const void*) override;

private:
    //
    App        *m_app;
    //
    bool runselect(bool, int32_t);
};

