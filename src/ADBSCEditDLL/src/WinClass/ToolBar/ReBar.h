
#pragma once

namespace Editor
{
    class ReBar : public WinBase
    {
        public:
            //
            bool        init(HWND);
            int32_t     addband(HWND, int32_t);
            void        showband(int32_t);
    };
};
#define ID_REBAR_BAND 30000

