#pragma once

namespace Editor
{
    class DLL_EXPORT WindowBase
    {
        protected:
            //
            HWND m_hwnd = nullptr;

        public:
            //
            ~WindowBase();
            //
            HWND handle();
            void active(bool = true);
            void setsize(RECT const&);
            void setsize(HWND);
            bool create(HWND, HINSTANCE, int32_t, std::string const&);
            bool create(HWND, HINSTANCE, RECT const&, int32_t, std::string const&, std::string const&);
            bool create(HWND, HINSTANCE, HMENU, int32_t, std::string const&, std::string const&);
            bool create(HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::string const&, std::string const&);
            void styleclear(HWND);
            template <typename T>
            HWND createEx(HWND, HINSTANCE, HMENU, RECT const&, int32_t, T const&, T const&, void* = nullptr);

        private:
            //
            template <typename T>
            HWND create_(HWND, HINSTANCE, HMENU, RECT const&, int32_t, T const&, T const&, void*);
    };

    extern template HWND WindowBase::createEx<std::string> (HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::string const&,  std::string const&,  void*);
    extern template HWND WindowBase::createEx<std::wstring>(HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::wstring const&, std::wstring const&, void*);

    extern template HWND WindowBase::create_<std::string> (HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::string const&,  std::string const&,  void*);
    extern template HWND WindowBase::create_<std::wstring>(HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::wstring const&, std::wstring const&, void*);

};

