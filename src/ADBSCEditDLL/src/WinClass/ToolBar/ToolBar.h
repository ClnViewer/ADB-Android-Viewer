
#pragma once

namespace Editor
{
    class ToolBar : public MDIWin::Base
    {
        public:
            //
            ToolBar();
            //
            MDIWin::WinData        data() override;
            std::tuple<bool, bool> event_initBegin() override;
            bool                   event_sizeinitBegin(ImageLite::IRECT<int32_t> const&) override;
            void                   event_size(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&) override;
            void                   event_resize() override;
            //
            std::string            event() override;
            void                   event(int32_t, bool) override;
            void                   event(int32_t, int32_t = -1, int32_t = -1, int32_t = -1) override;
            void                   event(uint32_t, uint32_t, std::vector<MDIWin::BaseData::dataMap>&) override;
            void                   show() override;

        private:
            //
            Editor::ReBar           m_rebar;
            Editor::MenuBar         m_menu;
            Editor::SearchBar       m_search;
            //
            bool                    setup_();
            int32_t                 getsizebtn_();
    };
};
