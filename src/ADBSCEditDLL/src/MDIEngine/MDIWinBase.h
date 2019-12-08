
#pragma once

#if !defined(DLL_EXPORT)
#  define DLL_EXPORT
#endif

namespace MDIWin
{
    class DLL_EXPORT BaseData
    {
        public:
            //
            using wndWrap = LRESULT CALLBACK (HWND, UINT, WPARAM, LPARAM);
            using dataMap = std::tuple<uint32_t, uint32_t, std::string, std::string>;
            //
            enum MDIWinType
            {
                MWTYPE_MAIN = 50000,
                MWTYPE_TOOLBAR,
                MWTYPE_CLIENT,
                MWTYPE_CUSTOM,
                MWTYPE_CHILD,
                MWTYPE_CHILD_EDIT,
                MWTYPE_CHILD_INFO,
                MWTYPE_CHILD_TOOLS,
                MWTYPE_CHILD_IMAGE,
                MWTYPE_CHILD_OTHER,
            };
            enum MDIWinStyle
            {
                MWSTYLE_MDI_CAPTION_MOVE,
                MWSTYLE_MDI_CAPTION_CLOSE,
                MWSTYLE_MDI_CAPTION,
                MWSTYLE_MDI_RESIZE,
                MWSTYLE_SYS_RESIZE,
                MWSTYLE_SYS_CAPTION,
                MWSTYLE_SYS_CLEAR,
                MWSTYLE_NONE
            };
            enum MDIWinPad
            {
                MWPAD_MDI_FX,
                MWPAD_MDI_FY,
                MWPAD_MDI_BX,
                MWPAD_MDI_BY,
                MWPAD_MDI_FBX,
                MWPAD_MDI_FBY
            };
            //
            static HICON default_icon;
            static bool DLL_EXPORT regclass(std::string const&, wndWrap, HBRUSH = nullptr, HICON = nullptr, int32_t = 0);
            static void DLL_EXPORT setwinstyle(HWND, MDIWinStyle);
            static int32_t DLL_EXPORT stylewinpad(MDIWinStyle, MDIWinPad = MDIWinPad::MWPAD_MDI_FX);
    };
    //

    struct WinData
    {
        HWND                  wparent   = nullptr;
        HWND                  window    = nullptr;
        HMENU                 wmenu     = nullptr;
        BaseData::MDIWinType  wgroup    = BaseData::MDIWinType::MWTYPE_CHILD;
        BaseData::MDIWinType  wtype     = BaseData::MDIWinType::MWTYPE_CHILD_OTHER;
        BaseData::MDIWinStyle wstyle    = BaseData::MDIWinStyle::MWSTYLE_NONE;
        std::string           wclass;
        std::string           wclassctrl;
        std::string           wtitle;
        uint32_t              wflags    = WS_OVERLAPPEDWINDOW | WS_OVERLAPPED | WS_CLIPCHILDREN;
        uint32_t              wflagex   = WS_EX_COMPOSITED;
        uint32_t              wid       = 0;
        void                 *wdata     = nullptr;
        bool                  isvisable = true;
        bool                  ispined   = false;
        ImageLite::IRECT<int32_t>  irdefault{};
        ImageLite::IRECT<int32_t>  irsize{};
        //
        WinData() {}
        //
        WinData(
            BaseData::MDIWinType  tg,
            BaseData::MDIWinType  tw,
            BaseData::MDIWinStyle ts,
            std::string           s1,
            std::string           s2,
            uint32_t              flags,
            uint32_t              flagex)
        : wgroup(tg), wtype(tw), wstyle(ts), wclass(s1), wtitle(s2),
          wflags(flags), wflagex(flagex), wid(static_cast<uint32_t>(tw)),
          wdata(nullptr), irdefault{}, irsize{} {}
        //
        WinData(
            BaseData::MDIWinType  tg,
            BaseData::MDIWinType  tw,
            BaseData::MDIWinStyle ts,
            std::string           s1,
            std::string           s2,
            uint32_t              flags,
            uint32_t              flagex,
            HMENU                 hmenu)
        : wmenu(hmenu), wgroup(tg), wtype(tw), wstyle(ts), wclass(s1), wtitle(s2),
          wflags(flags), wflagex(flagex), wid(static_cast<uint32_t>(tw)),
          wdata(nullptr), irdefault{}, irsize{} {}
    };
    //

    /** \brief Base:: initialization inherited:
    *
    * \code
    *  1. Manager::add<NameSpace::ClassName>(HandleParent, void * object)
    *      1.2 Manager:: NameSpace::ClassName class_pointer
    *      /// [init() - overwrite possible, using MDIWin::WinData]
    *      1.3 Manager:: class_pointer->init(
    *               HandleParent,
    *               unique id,
    *               registered class name,
    *               registered class control, // optional, is control TitleBar enabled
    *               void * object
    *           ))
    *           2.  NameSpace::ClassName create(MDIWin::WinData)
    *           3.  if (MDIWin::WinData.wstyle)
    *                   NameSpace::ClassName TitleBarClass.create(...)
    *           4.  /// (initclass() - overwrite possible, using MDIWin::WinData)
    *               NameSpace::ClassName initclass()
    *
    *  /// (overwrite possible)
    *  NameSpace::ClassName initsize(ImageLite::IRECT<int32_t>&)
    *
    *  /// (overwrite required)
    *  NameSpace::ClassName data() return MDIWin::WinData(...)
    *
    *  /// (inherited class begin constructor)
    *  NameSpace::ClassName()
    *      Base::m_data = data(); /// (overwrite required)
    **/

    class DLL_EXPORT Base
    {
        public:
            //
            Base();
            ~Base();
            Base(Base&&) = delete;                     // move construction
            Base(Base const&) = delete;                // copy construction
            Base& operator = (Base&&) = delete;        // move assignment
            Base& operator = (Base const&) = delete;   // copy assignment

        private:
            //
            void                       stylepad(BaseData::MDIWinStyle, bool  = true);

        protected:
            //
            WinData    m_data;
            TitleBar   m_titlebar;
            //
            bool                       create();
            bool                       create(ImageLite::IRECT<int32_t> const&);
            void                       resize();
            void                       refresh();
            void                       refresh(HWND);
            void                       refresh(uint32_t);
            //
            virtual WinData            data();
            virtual std::tuple<bool, bool> event_initBegin();
            virtual std::tuple<bool, bool> event_initEnd();
            //
            virtual bool               event_sizeinitBegin(ImageLite::IRECT<int32_t> const&);  // override
            virtual bool               event_sizeinitEnd();                                    // override
            virtual void               event_size(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&); //  override
            virtual bool               event_size();                                           // override
            virtual void               event_resize();                                         // override
            //

        public:
            //
            void                       open();
            void                       close(bool = true);
            HWND                       gethandle();                // m_data.window
            void                       sethandle(HWND);            // m_data.wparent, m_data.winstance
            ImageLite::IRECT<int32_t>  getsize();
            ImageLite::IRECT<int32_t>  getsizedefault();
            uint32_t                   getid();
            BaseData::MDIWinType       getgroup();
            BaseData::MDIWinType       gettype();
            std::string                gettitle();
            bool                       isvisable();
            bool                       istitlebar();
            bool                       ispined();
            void                       setpined(bool);
            void                       setvisable(bool);
            void                       settitle(std::string const&);
            bool                       init(HWND, uint32_t, std::string const&, std::string const&, void*);
            // size
            void                       setsizedefault();  // initial set empty
            void                       setsizedefault(ImageLite::IRECT<int32_t> const&);  // initial mode, call initsizeX()
            void                       setsize();                                         // resize from HWND mode
            void                       setsize(ImageLite::IPOINT<int32_t> const&);        // move mode
            void                       setsize(ImageLite::IRECT<int32_t> const&, ImageLite::IRECT<int32_t> const&); // set child mode
            //
            virtual void               show();
            virtual void               show(HWND);
            virtual void               show(uint32_t);
            virtual void               show(std::string const&);
            virtual void               show(std::vector<std::string> const&);
            //
            virtual void               event(void*);
            virtual std::string        event();
            virtual bool               event(HWND, LPARAM, std::string&);
            virtual void               event(int32_t, bool);
            virtual void               event(int32_t, int32_t = -1, int32_t = -1, int32_t = -1);
            virtual void               event(uint32_t, uint32_t, std::vector<MDIWin::BaseData::dataMap>&);
    };
};

