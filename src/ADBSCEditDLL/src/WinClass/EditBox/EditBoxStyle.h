

#pragma once

namespace Editor
{
    enum e_color
    {
#   define EBSTYLE__(A,...)  SC_COLOR_ ## A,
#   include "EditBoxStyleBase.h"
    };

#   if defined(EDITBOX_STYLE_ACTIVE)
    static inline const COLORREF l_color_b_[] =
    {
#   define EBSTYLE__(A,R1,G1,B1,R2,G2,B2,...)  RGB(R1,G1,B1),
#   include "EditBoxStyleBase.h"
    };

    static inline const COLORREF l_color_w_[] =
    {
#   define EBSTYLE__(A,R1,G1,B1,R2,G2,B2,...)  RGB(R2,G2,B2),
#   include "EditBoxStyleBase.h"
    };

    static inline const COLORREF *l_color[] = { l_color_w_, l_color_b_ };
#   endif

};
