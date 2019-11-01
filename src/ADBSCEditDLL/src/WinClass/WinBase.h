
#pragma once

namespace Editor
{
    class DLL_EXPORT WinBase
    {
        private:
            //
            void styleclear(HWND);

        protected:
            //
            HWND m_hwnd = nullptr;
            //
            bool create(HWND, std::string const&, uint32_t = 0U, HMENU = nullptr, void* = nullptr);

        public:
            //
            WinBase();
            ~WinBase();
            WinBase(WinBase&&) = delete;                     // move construction
            WinBase(WinBase const&) = delete;                // copy construction
            WinBase& operator = (WinBase&&) = delete;        // move assignment
            WinBase& operator = (WinBase const&) = delete;   // copy assignment
            //
            HWND gethandle();
            void show();
    };
};

