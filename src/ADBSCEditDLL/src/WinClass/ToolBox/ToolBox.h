
#pragma once

namespace Editor
{
    class ToolBox : public MDIWin::Base
    {
        public:
            //
            enum TabIndex
            {
                ITAB_HELP = 0,
                ITAB_API,
                ITAB_MONITOR
            };
            //
            ToolBox();
            ~ToolBox();
            //
            MDIWin::WinData        data() override;
            std::tuple<bool, bool> event_initEnd() override;
            void                   event_size(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&) override;
            void                   event_resize() override;
            //
            void                   show() override;
            void                   show(uint32_t) override;
            void                   show(std::vector<std::string> const &) override;
            //
            bool                   event(HWND, LPARAM, std::string&) override;
            //
            HWND                   getchild(TabIndex);
            //
            static LRESULT CALLBACK TabWndProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

        private:
            //
            HWND             m_hwnd   = nullptr;
            HWND             m_tab[3] = {};
            ImageLite::IRECT<int32_t> m_irchild[3];
            //
            void             sizeinit_(); // MDIWin::Base size initial call
            bool             setup_();
            HWND             setup_help_();
            HWND             setup_api_();
            HWND             setup_monitor_();
            void             add_monitor_(HWND, std::vector<std::string> const &);
            void             show_(uint32_t);
            bool             tabforeach_(uint32_t, std::function<bool(HWND*,uint32_t,uint32_t)>);
            //
            bool             event_api(HWND, LPARAM, std::string&);
            bool             event_monitor(HWND, LPARAM, std::string&);
    };
};
