#pragma once

#include  "SDL2/SDL_syswm.h"

#define MENU_ITEM_ADD(ID1,ID2,M) __extension__ ({                               \
    auto wtxt = ResManager::stringpopup(ID2, AppConfig::instance().cnf_lang);   \
    AppendMenuW(M, MF_STRING, ID1, wtxt);                                       \
    })

#define MENU_ADD(PM,ID2,M) __extension__ ({                                     \
    auto wtxt = ResManager::stringpopup(ID2, AppConfig::instance().cnf_lang);   \
    AppendMenuW(M, MF_STRING | MF_POPUP, (UINT_PTR)PM, wtxt);                   \
    })

class guiPopUp
{
public:
    //
    static void style(HMENU);
    static void style(HMENU, int32_t);
    static int32_t show(HMENU, SDL_Window*);
    static void clean();

private:
    //
    static HBRUSH m_hbrush;

};
