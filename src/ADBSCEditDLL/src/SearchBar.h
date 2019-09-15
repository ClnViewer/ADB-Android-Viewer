
#pragma once

namespace Editor
{
    class SearchBar : public WindowBase
    {
        public:
            //
            bool                    create(HWND, HWND, HINSTANCE);
            std::string             getfind();
    };
};
