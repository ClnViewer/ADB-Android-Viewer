#pragma once

#include "../../ADBViewer.h"

class App;

class AppBrowserList : public guiBase, public guiPage
{

public:
    struct ListPosition
    {
        int32_t   idx;
        int32_t   sel;

        ListPosition() : idx(0), sel(-1) {}
        void Default() { idx = 0; sel = -1; }
        void Default(int32_t s_) { idx = 0; sel = s_; }
    };
    //
    AppBrowserParse      m_parser;
    //
    ~AppBrowserList();

    bool init(App*, AppBrowserPage*);
    bool tinit(SDL_Texture**) override;
    bool evresize(SDL_Texture**) override;
    bool event(SDL_Event*, const void*) override;
    bool uevent(SDL_Event*, const void*) override;
    //
    void start();
    void stop();
    //
    void lineUp();
    void lineDown();
    void pageUp();
    void pageDown();
    void pageHome();
    void pageEnd();
    void lineSelect(SDL_Event*);
    //
    void draw();
    void draw(AppBrowserList::ListPosition*);
    void draw(std::string const&);
    void draw(std::string const&, std::function<bool(std::string const&, AppBrowserPage::DrawItem&)>);
    void draw(std::vector<AppBrowserPage::DrawItem>&);
    void draw(std::vector<AppBrowserPage::DrawItem>&, AppBrowserList::ListPosition*);
    //

private:
    //
    App                 *m_app = nullptr;
    AppBrowserPage      *m_page = nullptr;
    SDL_Surface         *m_surface = nullptr;
    TTF_Font            *m_font = nullptr;  // auto remove from cache in DLL
    SDL_Color           *m_color = nullptr;
    //
    std::atomic<bool>    m_isclick = false;
    std::mutex           s_lock;
    std::vector<AppBrowserPage::DrawItem> m_drawitems;
    //
    int32_t              m_list_step = 0;
    ListPosition         m_listpos_default;
    ListPosition         m_listpos_files_local;
    ListPosition         m_listpos_files_device;
    //
    void drawline(AppBrowserPage::DrawItem const&, int32_t);
    //
    ListPosition * selectsource();
    void listselect(AppBrowserList::ListPosition*);
    void clickselectfile(AppBrowserPage::DrawItem const&, int32_t);
    //
    int32_t popupmenu_apk();
    int32_t popupmenu_info();
    int32_t popupmenu_file();

};

