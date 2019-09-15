
#pragma once

namespace Editor
{
    class MenuBar : public WindowBase
    {
        public:
            //
            bool                    create(HWND, HWND, HINSTANCE);
            void                    setenable(uint32_t, bool);
            void                    setcheck(uint32_t, bool);
    };
};

