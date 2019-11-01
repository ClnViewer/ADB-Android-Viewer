
#pragma once

namespace Editor
{
    class MenuBar : public WinBase
    {
        public:
            //
            enum MainMenuId
            {
                MMID_Main = 0,
                MMID_Edit,
                MMID_View,
                MMID_Debug,
                MMID_Plugins,
                MMID_Window
            };
            //
            ~MenuBar();
            //
            bool        init(HWND, HWND);
            void        setenable(uint32_t, bool);
            void        setcheck(uint32_t,  bool);
            void        add(MainMenuId, uint32_t, std::vector<MDIWin::BaseData::dataMap>&);
            //
            static LRESULT CALLBACK MenuWndProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
    };
};

