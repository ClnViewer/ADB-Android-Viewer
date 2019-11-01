
#pragma once

namespace MDIWin
{

    class DLL_EXPORT MDIApp
    {
        protected:
            //
            MDIWin::Manager       m_mgr;
            std::vector<uint32_t> m_call_after_init;
            //
            void mdiloop(uint32_t);
            bool mdirun();
            bool mdirun(std::string const&, HINSTANCE = nullptr);
            //
            virtual LRESULT mdiFrameProc(HWND, HWND, UINT, WPARAM, LPARAM, bool);
            virtual LRESULT mdiChildProc(HWND, UINT, WPARAM, LPARAM, bool);

        public:
            //
            MDIApp();
            ~MDIApp();
            MDIApp(MDIApp&&) = delete;                     // move construction
            MDIApp(MDIApp const&) = delete;                // copy construction
            MDIApp& operator = (MDIApp&&) = delete;        // move assignment
            MDIApp& operator = (MDIApp const&) = delete;   // copy assignment
            //
            LRESULT mdiFrameProc_(HWND, HWND, UINT, WPARAM, LPARAM);
            LRESULT mdiChildProc_(HWND, UINT, WPARAM, LPARAM);
            //
            static uint32_t m_dragid;
            static LRESULT CALLBACK mdiFrameRedirect(HWND, UINT, WPARAM, LPARAM);
            static LRESULT CALLBACK mdiChildRedirect(HWND, UINT, WPARAM, LPARAM);
    };
};


