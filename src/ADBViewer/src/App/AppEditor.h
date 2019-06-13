#pragma once

#include "../ADBViewer.h"

class App;

class AppEditor : public guiBase
{
public:
    //
    struct _RGB {
        uint8_t r, g, b;
        };
    struct _PIXELS {
        int32_t x, y, pos;
        struct _RGB rgb;
        };
    //
    enum AppEditorScriptType
    {
        SCR_CHECK_AND_CLICK,
        SCR_CLICK_ONLY,
        SCR_NONE
    };
    //
    SDL_Point                       ppixel;
    AppEditor::AppEditorScriptType  scrtype;

    AppEditor();
    ~AppEditor();

    bool isactive() const;
    bool isupdate() const;
    bool istarget() const;
    bool init(App*);
    void run();
    void stop();
    void cancel();
    void update(uint32_t, uint32_t, std::vector<uint8_t> &);
    bool event(SDL_Event*, const void*) override;

private:
    //
    App                            *m_app;
    std::atomic<bool>               m_active;
    std::atomic<bool>               m_target;
    std::vector<AppEditor::_PIXELS> m_pixels;
    std::vector<AppEditor::_PIXELS> m_ptarget;
    const AppEditor::_RGB           m_redrgb = { 255, 0, 0 };
    const AppEditor::_RGB           m_bluergb = { 0, 0, 255 };
    const AppEditor::_RGB           m_greenrgb = { 0, 255, 0 };

    //
    int32_t getpad(uint32_t);
    int32_t getpos(int32_t, int32_t, uint32_t, uint32_t);
    int32_t checkpos(uint32_t, uint32_t, int32_t);
    bool    foundpos(std::vector<AppEditor::_PIXELS>&, AppEditor::_PIXELS&);
    void    gui_icon_on();
    void    gui_icon_off();
    void    write_script(std::string const&);
};
