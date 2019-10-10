
#pragma once

namespace Editor
{
    class EditBox : public WindowBase
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
                int32_t line;
                std::string text;
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
             EditBox(HINSTANCE, EditBox::StyleTheme = EditBox::StyleTheme::EB_STYLE_LIGHT);
             EditBox();
            ~EditBox();
            //
            template <typename T>
            bool                    gettext(T &);
            //
            bool                    create(HWND, HINSTANCE, RECT const&, std::string const&);
            bool                    setup(HWND, RECT const&, std::string const&);
            bool                    issaved();
            bool                    vieweol();
            LRESULT                 navigate();
            void                    notify(void*);
            void                    findtext(std::string const&);
            void                    textinsert(std::string const&);
            void                    setdebugline(int32_t);
            void                    showhelp();
            void                    setannotation(std::string const&, int32_t);
            void                    setannotation(EditBox::AnnotateData*);
            LRESULT                 command(UINT, WPARAM = 0, LPARAM = 0);
            //
            void                    filedefault();
            std::string             filesave(std::string const&);
            std::string             fileopen();

        private:
            //
#           if defined(_BUILD_DYNAMIC_SC)
            GameDev::LoadDll         m_liblex;
#           endif
            std::atomic<bool>        m_ishelp = false;
            //
            EditBox::TextServiceData m_data{};
            COLORREF                 m_styleColor[23]{};
            //
            void                    init(HINSTANCE);
            void                    initstyle(EditBox::StyleTheme);
            void                    style(int32_t);
            void                    style(int32_t, COLORREF);
            void                    style(int32_t, COLORREF, COLORREF);
            void                    style(int32_t, COLORREF, COLORREF, int32_t, std::string const&);
            bool                    setup(std::string const&);
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

