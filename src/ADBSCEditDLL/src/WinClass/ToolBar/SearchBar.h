
#pragma once

namespace Editor
{
    class SearchBar : public WinBase
    {
        private:
            //
            IDropTarget *m_dropt;

        public:
            //
            ~SearchBar();
            //
            bool                    init(HWND, HWND);
            std::string             gettext();
            //
            static LRESULT CALLBACK SearchWndProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
    };
};
