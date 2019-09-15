
#pragma once

namespace Editor
{
    class DLL_EXPORT SCEdit : public WindowBase
    {
        public:
            //
            struct RectWin
            {
                RECT rmain, redit, rinf, rtbar;
            };
            //
            ~SCEdit();
            //
            void Init(HINSTANCE, HICON = nullptr);                                                   // Open empty
            void Init(std::string const&, HINSTANCE = nullptr, HICON = nullptr);                     // Open file
            void Init(std::string const&, std::string const&, HINSTANCE = nullptr, HICON = nullptr); // Open as text and file name to save latter
            bool Run(bool = false);
            static std::string const getChildClassName();
            static std::string const getMenuClassName();
            //
#           if defined(_BUILD_DLL)
            static inline const char appName[] = "LUA SCEdit";
#           endif
            //
        private:
            //
            HINSTANCE            m_hinst   = nullptr;
            HACCEL               m_haccel  = nullptr;
            HBRUSH               m_hbgmain = nullptr;
            HICON                m_hicon   = nullptr;
            Editor::LuaEngine    m_luae;
            Editor::ToolBar      m_tb;
            Editor::EditBox      m_hedit;
            Editor::InfoBox      m_hinfo;
            Editor::CWindowDock  m_dock;
            RectWin              m_rect       = {};
            //
            std::atomic<bool>    m_issave     = false;
            std::atomic<bool>    m_isselect   = false;
            std::atomic<bool>    m_iswintop   = false;
            std::atomic<bool>    m_isgridshow = true;
            //
            std::string m_text;
            std::string m_savepath;
            //
            void                    run_();
            int32_t                 loopWin_();
            bool                    createWin_();
            bool                    registerWin_(std::string const&, HICON = nullptr, int32_t = 0);
            bool                    calcWinSize_();
            bool                    calcWinSize_(int32_t, int32_t, int32_t, int32_t, int32_t);
            bool                    calcWinSize_(HWND);
            //
            LRESULT                 wndProc_(HWND, UINT, WPARAM, LPARAM);
            static LRESULT CALLBACK wndRedirect_(HWND, UINT, WPARAM, LPARAM);
            //
            // WNDPROC events
            void                    _OnClose(HWND);
            BOOL                    _OnCreate(HWND, LPCREATESTRUCT);
            BOOL                    _OnNotify(HWND, int32_t, NMHDR*);
            void                    _OnMove(HWND, int32_t, int32_t);
            void                    _OnSize(HWND, UINT, int32_t, int32_t);
            UINT                    _OnNCHitTest(HWND, int32_t, int32_t);
            void                    _OnCommand(HWND, UINT, WPARAM, LPARAM);

        protected:
            //
#           if defined(_BUILD_DLL)
            static inline const char classNameMain[]  = "SCEditWindowMain";
            static inline const char classNameChild[] = "SCEditWindowChild";
            static inline const char classNameMenu[]  = "SCEditWindowMenu";
#           endif
    };
};

