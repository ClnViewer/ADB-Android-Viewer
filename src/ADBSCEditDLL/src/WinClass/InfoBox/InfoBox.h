
#pragma once

namespace Editor
{

    class InfoBox : public MDIWin::Base
    {
        private:
            //
            HWND             m_hwnd = nullptr;
            GameDev::LoadDll m_libed;
            //
            void             sizeinit_(); // MDIWin::Base size initial call

        public:
            //
            InfoBox();
            ~InfoBox();
            MDIWin::WinData        data() override;
            std::tuple<bool, bool> event_initEnd() override;
            void                   event_size(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&) override;
            void                   event_resize() override;
    };
};
