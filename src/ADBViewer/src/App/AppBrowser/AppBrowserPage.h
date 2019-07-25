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
    enum FileType
    {
        FILETYPE_NONE = 0,
        FILETYPE_DIR,
        FILETYPE_FILE,
        FILETYPE_SYMLINK,
        FILETYPE_ROOT,
        FILETYPE_BACK,
        FILETYPE_INFO,
    };
    //
    struct DrawItem
    {
        std::string s;
        std::string cmds;
        std::string desc;
        FileType    type;
        SDL_Color   ctxt, cbg;

        DrawItem()
            : type(FileType::FILETYPE_NONE), ctxt{ 255, 255, 255, 0 }, cbg{ 0, 0, 0, 0 } {}

        DrawItem(SDL_Color const & c_, SDL_Color b_)
            : type(FileType::FILETYPE_NONE), ctxt(c_), cbg(b_) {}

        DrawItem(std::string const & s_, SDL_Color const & c_, SDL_Color const & b_)
            : s(s_), type(FileType::FILETYPE_NONE), ctxt(c_), cbg(b_) {}

        DrawItem(std::string const & s_, std::string const & m_, FileType f_, SDL_Color const & c_)
            : s(s_), cmds(m_), type(f_), ctxt(c_), cbg{ 0, 0, 0, 0 } {}

        DrawItem(std::string const & s_, std::string const & m_, std::string const & d_, FileType f_, SDL_Color const & c_)
            : s(s_), cmds(m_), desc(d_), type(f_), ctxt(c_), cbg{ 0, 0, 0, 0 } {}

        void Description(
            std::string const & as_,
            std::string const & bs_,
            std::string const & cs_,
            SDL_Color   const & color_)
        {
            std::stringstream ss;
            if (!as_.empty())
                ss << as_ << " ";
            if (!bs_.empty())
                ss << bs_ << " ";
            if (!cs_.empty())
                ss << "(" << cs_ << ")";

            this->ctxt = color_;
            this->desc = ss.str();
        }
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

