#pragma once

namespace MDIWin
{
    //
    class Config
    {
        public:
            //
            enum StyleBrush
            {
                BRUSH_GRAY,
                BRUSH_LIGHT,
                BRUSH_WHITE
            };
            enum ImageButtonList
            {
                IMLBTN_TITLEBAR_CTRL,
                IMLBTN_TOOLBAR_CTRL,
                IMLBTN_REBAR_CTRL,
                IMLBTN_TOOLAPI_CTRL
            };
            enum HandleType
            {
                HWND_MAIN,
                HWND_CLIENT,
                HWND_TOOLBAR,
                HWND_INFOBOX,
                CLASS_APP,
                CLASS_MGR,
                CLASS_TBAR,
                CLASS_EDIT
            };
            enum TextMsgType
            {
                IB_TYPE_ERROR,
                IB_TYPE_ALERT,
                IB_TYPE_INFO,
                IB_TYPE_DEFAULT
            };

        private:
            //
            HINSTANCE            m_hinst     = nullptr;
            HFONT                m_hfont     = nullptr;
            HWND                 m_hwnd[4]   = {};
            HBRUSH               m_hbrush[3] = {};
            HIMAGELIST           m_himl[4]   = {};
            //
            void               * m_class[4]  = {};
            //
            Config();
            ~Config();
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;
            //
            void              text();
            void              text(std::string const&);
            void              text(std::string const&, COLORREF const&);
            void              text(std::string const&, TextMsgType);

        public:
            //
            HICON        appicon = nullptr;
            std::string  appname;
            std::string  srctext;
            std::string  savepath;
            //
            std::atomic<bool> issaved    = false;
            std::atomic<bool> isselect   = false;
            std::atomic<bool> iswintop   = false;
            std::atomic<bool> isgridshow = true;
            //
            void clear();
            void init(HINSTANCE);
            static Config& instance();
            //
            HINSTANCE         gethinstance();
            HBRUSH            getbrush(StyleBrush);
            HFONT             getfont();
            std::string       getfontname();
            uint32_t          getiml(ImageButtonList, ImageLite::IPOINT<int32_t>&);
            HIMAGELIST        getiml(ImageButtonList);
            //
            // print:
            //
            void colorBox  (std::string const&, COLORREF const&);
            void errorBox  (std::string const&, int32_t, uint32_t = 0U);
            void alertBox  (std::string const&, std::string const&, int32_t);
            void debugBox  (std::string const&, COLORREF const&, std::string const&, int32_t);
            void defaultBox(std::string const&);
            void infoBox   (std::string const&);
            void clearBox  ();
            //
            template <typename T> void BoxT(std::string const&, T const&);
            //
            template <class T, HandleType t>
            T * getclass()
            {
                void *obj = nullptr;

                switch (t)
                {
                    case HandleType::CLASS_APP:  obj = m_class[0]; break;
                    case HandleType::CLASS_MGR:  obj = m_class[1]; break;
                    case HandleType::CLASS_TBAR: obj = m_class[2]; break;
                    case HandleType::CLASS_EDIT: obj = m_class[3]; break;
                    default: break;
                }
                return static_cast<T*>(obj);
            }
            //
            template <HandleType t>
            void setclass(void *val)
            {
                switch (t)
                {
                    case HandleType::CLASS_APP:  m_class[0] = val; break;
                    case HandleType::CLASS_MGR:  m_class[1] = val; break;
                    case HandleType::CLASS_TBAR: m_class[2] = val; break;
                    case HandleType::CLASS_EDIT: m_class[3] = val; break;
                    default: break;
                }
            }
            //
            template <HandleType Tht>
            HWND gethandle()
            {
                if constexpr ((Tht == HandleType::HWND_MAIN)    ||
                              (Tht == HandleType::HWND_CLIENT)  ||
                              (Tht == HandleType::HWND_TOOLBAR) ||
                              (Tht == HandleType::HWND_INFOBOX))
                    return m_hwnd[Tht];
                else
                    return nullptr;
            }
            //
            template <HandleType Tht>
            void sethandle(HWND hwnd)
            {
                if constexpr ((Tht == HandleType::HWND_MAIN)    ||
                              (Tht == HandleType::HWND_CLIENT)  ||
                              (Tht == HandleType::HWND_TOOLBAR) ||
                              (Tht == HandleType::HWND_INFOBOX))
                    if (hwnd)
                        m_hwnd[Tht] = hwnd;
            }
    };
    extern template void Config::BoxT<Config::TextMsgType>(std::string const&, Config::TextMsgType const&);
    extern template void Config::BoxT<COLORREF>(std::string const&, COLORREF const&);
};

///
/// class Config global print macro
///
#define TRACEBox(A,B)     MDIWin::Config::instance().colorBox(A, B)
#define TRACEBoxEx(A,B)   MDIWin::Config::instance().debugBox(A, B, __FUNCTION__, __LINE__)
#define ERRORBox()        MDIWin::Config::instance().errorBox(__FUNCTION__, __LINE__)
#define ERRORBoxEx(A)     MDIWin::Config::instance().errorBox(__FUNCTION__, __LINE__, A)
#define ALERTBox(A)       MDIWin::Config::instance().alertBox(A, __FUNCTION__, __LINE__)
#define DEFAULTBox(A)     MDIWin::Config::instance().defaultBox(A)
#define INFOBox(A)        MDIWin::Config::instance().infoBox(A)
#define CLEARBox()        MDIWin::Config::instance().clearBox()
#define COLORBox(A,R,G,B) MDIWin::Config::instance().BoxT<COLORREF>(A, RGB(R,G,B))
#define COLORREFBox(A,C)  MDIWin::Config::instance().BoxT<COLORREF>(A, C)
//
