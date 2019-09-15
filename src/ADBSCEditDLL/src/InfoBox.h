
#pragma once

namespace Editor
{
    class InfoBox : public WindowBase
    {
        public:
            //
            enum TextType
            {
                IB_TYPE_ERROR,
                IB_TYPE_ALERT,
                IB_TYPE_INFO,
                IB_TYPE_DEFAULT
            };
            //
            InfoBox();
            ~InfoBox();
            //
            bool                    ready();
            bool                    create(HWND, HINSTANCE, RECT const&, std::string const&);
            void                    text(); // clear all
            void                    text(std::string const&);
            void                    text(std::string const&, InfoBox::TextType);
            void                    text(std::string const&, COLORREF const&);

        private:
            //
            HWND                    m_hrtb  = nullptr;
            GameDev::LoadDll        m_libed;
            //
            bool                    setup(HWND, HINSTANCE, RECT const&);
            void                    settext(std::string const&, bool);
            void                    highlight(COLORREF const&);
    };
};

