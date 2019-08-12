#pragma once

#include "../../ADBViewer.h"

class App;

class AppBrowser : public guiBase
{
public:
    //
    AppBrowser();

    bool init(App*);
    bool tinit(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;
    bool evresize(SDL_Texture**) override;
    bool isenabled() const;


private:
    //
    App              *m_app = nullptr;
    AppBrowserPage    m_page;
    AppBrowserList    m_list;
    guiIconTrigger    m_icon_menu4;
    guiIconTrigger    m_icon_menu9;
    std::mutex        m_lock;
    std::atomic<bool> m_enable;
    SDL_Color         m_color = { 0x0, 0x0, 0x0, 0x0 };
    //
    void runselect(bool);
    void resizewin(int32_t);
    //
    /// Menu 4 event
    bool mevent4(SDL_Event*, SDL_Rect*);
    void mevent4motion(int32_t);
    void mevent4button(int32_t);

    /// Menu 9 event
    bool mevent9(SDL_Event*, SDL_Rect*);
    void mevent9motion_file(int32_t);
    void mevent9motion_apk(int32_t);
    void mevent9button_file(int32_t);
    void mevent9button_apk(int32_t);};

