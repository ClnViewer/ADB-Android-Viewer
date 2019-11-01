
#pragma once

namespace Editor
{
    class DLL_EXPORT SCEApp : public MDIWin::MDIApp
    {
        private:
            //
            Editor::LuaEngine       m_luae;
            Editor::LuaPluginEditor m_luap;
            //
            void               run_();
            //                 WNDPROC events
            UINT               _OnNCHitTest(HWND, int32_t, int32_t);
            void               _OnCommand(HWND, UINT, WPARAM, LPARAM);

        public:
            //
            void init(HINSTANCE, HICON);
            void init(std::string const&, HINSTANCE, HICON);
            void init(std::string const&, std::string const&, HINSTANCE, HICON);
            void initexcept();
            void appname(std::string const&);
            //
            bool run(bool);
            //
            LRESULT mdiFrameProc(HWND, HWND, UINT, WPARAM, LPARAM, bool) override;
            LRESULT mdiChildProc(HWND, UINT, WPARAM, LPARAM, bool) override;
    };
};
