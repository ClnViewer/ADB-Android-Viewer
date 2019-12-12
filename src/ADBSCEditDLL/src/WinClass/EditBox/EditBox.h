
#pragma once

namespace Editor
{

    class EditBox : public MDIWin::Base
    {
        public:
            //
            enum StyleTheme
            {
                EB_STYLE_LIGHT,
                EB_STYLE_BLACK
            };
            //
            struct AnnotateData
            {
                int32_t                  line;
                std::string              text;
                std::vector<std::string> data;
            };
            //
            struct AutoHighlight
            {
                uint32_t base, count, curent;
                uint32_t range[25];
            };
            //
            struct FindTextData
            {
                int32_t lastpos;
                int32_t wordlen;
                int32_t flag;     //SCFIND_NONE, SCFIND_MATCHCASE, SCFIND_WHOLEWORD
                int32_t direct;    // SCI_SETSELECTIONSTART, SCI_SETSELECTIONEND, SCI_SETSELALPHA;
            };
            //
            struct TextServiceData
            {
                int32_t                lastChar;
                int32_t                scrollStep;
                EditBox::AutoHighlight lightText;
                EditBox::FindTextData  findText;
            };
            //
            EditBox::FindTextData  search_data{};
            //
             EditBox();
            ~EditBox();
            //
            MDIWin::WinData        data() override;
            std::tuple<bool, bool> event_initEnd() override;
            bool                   event_sizeinitEnd() override;
            void                   event_size(ImageLite::IRECT<int32_t>  const&, ImageLite::IRECT<int32_t>  const&) override;
            void                   event_resize() override;
            void                   show() override;
            void                   event(void*) override;

            //
            template <typename T>
            bool                    gettext(T &);
            //
            bool                    issaved();
            bool                    vieweol();
            LRESULT                 navigate();
            void                    findtext(std::string const&);
            void                    textnew(std::string const&, bool = false);
            void                    textinsert(std::string const&);
            bool                    textreplace(std::string const&, std::string const&);
            void                    gotoline(int32_t);
            void                    showhelp();
            void                    setdebugline(int32_t);
            void                    setannotation(std::string const&, int32_t);
            void                    setannotation(EditBox::AnnotateData*);
            LRESULT                 command(UINT, WPARAM = 0, LPARAM = 0);
            //
            void                    filedefault();
            bool                    filevalidext(std::string const&);
            bool                    filesave(std::string const&);
            bool                    fileopen(std::string const&);
            bool                    fileopen();
            //
            static LRESULT CALLBACK EditWndProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

        private:
            //
#           if defined(_BUILD_DYNAMIC_SC)
            GameDev::LoadDll         m_liblex;
#           endif
            HWND                     m_hwnd   = nullptr;
            std::atomic<bool>        m_ishelp = false;
            //
            void                     sizeinit_(); // MDIWin::Base size initial call
            //
            EditBox::TextServiceData m_txtdata{};
            COLORREF                 m_styleColor[23]{};
            //
            void                    initscintilla();
            void                    initstyle(EditBox::StyleTheme);
            void                    style(int32_t);
            void                    style(int32_t, COLORREF);
            void                    style(int32_t, COLORREF, COLORREF);
            void                    style(int32_t, COLORREF, COLORREF, int32_t, std::string const&);
            bool                    setup();
            LRESULT                 cmd(UINT, WPARAM = 0, LPARAM = 0);
            //
            void                    serviceAutoAppend(int32_t = 0);
            bool                    serviceAutoToolTip(std::string const&, int32_t = 0, bool = false);
            void                    serviceAutoToolTipShiftArgs();
            int32_t                 serviceAddMark(int32_t, int32_t, const char*[]);
    };
    extern template bool EditBox::gettext<std::string>(std::string&);
    extern template bool EditBox::gettext<std::vector<char>>(std::vector<char>&);
};

