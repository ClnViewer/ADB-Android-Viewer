#pragma once

#include "../../ADBViewer.h"

#define SCROLL_NUM_SCREEN 20

class AppTerminalPage : public guiPage
{
public:
    //
    bool              init(SDL_Window*);
    std::string       getprompt();

    SDL_Rect          rbase{};
    //
    SDL_Rect          out_rsrc{};
    SDL_Rect          out_rdst{};
    SDL_Point         out_ssize{};
    SDL_Point         out_pos{};
    int32_t           out_npage = 0;
    int32_t           out_tpage = 0;
    int32_t           out_mpage = 0;
    //
    SDL_Rect          btn_r_close{};
    SDL_Rect          btn_r_edit{};
    SDL_Point         c_p_input{};
    SDL_Rect          c_p_output{};
    SDL_Point         c_p_pnum{};
    //
    inline static const char *term_prompt = "> ";
    //

private:
    //
    bool getwin(SDL_Window*);
};

