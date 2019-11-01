
#pragma once

namespace MDIWin
{
    class DLL_EXPORT TitleBar
    {
        public:
            enum HandleType
            {
                HT_PARENT,
                HT_WINDOW,
                HT_TITLE,
                HT_TB,
                HT_HWND_END,
                HT_BTN_ID_CLOSE,
                HT_BTN_ID_DEFAULT,
                HT_BTN_ID_PIN,
                HT_DEFAULT_ID = 50000
            };
            //
            TitleBar();
            ~TitleBar();
            //
            bool                       isenable();

            bool                       create(HWND, std::string const&, uint32_t = 0U);
            HWND                       getparent();
            ImageLite::IRECT<int32_t>  getsize();
            std::string                gettitle();
            void                       setsize(HDC);
            void                       setsize(ImageLite::IRECT<int32_t> const&);
            void                       settitle(std::string const&);
            //
            LRESULT                    wndControlProc(HWND, UINT, WPARAM, LPARAM);
            static LRESULT CALLBACK    wndControlRedirect(HWND, UINT, WPARAM, LPARAM);

        private:
            //
            HWND                       m_handle[4] = {};
            uint32_t                   m_numbutton = 0U;
            //
            ImageLite::IRECT<int32_t>  m_wsize{};
            ImageLite::IPOINT<int32_t> m_bsize{};
            //
            bool                       createbtn();
            void                       setposition(HandleType, ImageLite::IRECT<int32_t> const&, uint32_t = 0);
    };
};

