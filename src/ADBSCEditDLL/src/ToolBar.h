
#pragma once

namespace Editor
{
    class ToolBar : public WindowBase
    {
        public:
            //
            bool                    create(HWND, HINSTANCE);
            bool                    getsize(RECT&);
            int32_t                 getbtnsize();
            std::string             getfind();
            void                    changebutton(int32_t, int32_t = -1, int32_t = -1, int32_t = -1);
            void                    event(int32_t, bool = false);

        private:
            //
            Editor::ReBar           m_rebar;
            Editor::SearchBar       m_findbar;
            Editor::MenuBar         m_menu;
            //
            bool                    setup(HINSTANCE);
    };
};
