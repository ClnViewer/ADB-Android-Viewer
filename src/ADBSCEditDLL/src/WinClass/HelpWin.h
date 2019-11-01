
#pragma once

namespace MDIWin
{

    class HelpWin : public Base
    {
        public:
            //
            HelpWin()
            {
                Base::m_data = data();
            }

            WinData data() override
            {
                WinData d(
                                BaseData::MDIWinType::MWTYPE_CHILD,       // MDIWinType group
                                BaseData::MDIWinType::MWTYPE_CHILD_TOOLS, // MDIWinType type
                                BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE,  // MDIWinStyle
                                std::string(),                           // Class name
                                "Help Windows",                          // Title
                                (WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS),
                                WS_EX_COMPOSITED
                            );
                d.irdefault.set<int32_t>(80, 0, 20, 70);                // % from main widow
                return d;
            }

            void event_size(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const & ire) override
            {
                Base::m_data.irsize.set<int32_t>(
                        ((Base::m_data.irsize.x > ire.x) ? Base::m_data.irsize.x : ire.x),
                        ((Base::m_data.irsize.y > ire.y) ? Base::m_data.irsize.y : ire.y),
                        (irc.w - ire.w),
                        ire.h
                    );
                Base::resize();
            }
    };
};
