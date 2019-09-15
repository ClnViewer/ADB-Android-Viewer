#pragma once

namespace Editor
{
    class PrintBox
    {
        private:
            //
            Editor::InfoBox *m_winfo  = nullptr;
            //
            PrintBox();
            PrintBox(const PrintBox&) = delete;
            PrintBox& operator=(const PrintBox&) = delete;

        public:
            //
            void init(Editor::InfoBox* = nullptr);
            static PrintBox& instance();
            Editor::InfoBox * getinfobox();
            //
            template <typename T> void BoxT(std::string const&, T const&);
            //
            void colorBox  (std::string const&, COLORREF const&, std::string const&, int32_t);
            void errorBox  (std::string const&, int32_t, uint32_t = 0U);
            void alertBox  (std::string const&, std::string const&, int32_t);
            void infoBox   (std::string const&);
            void defaultBox(std::string const&);
            void clearBox  ();
    };
    extern template void PrintBox::BoxT<InfoBox::TextType>(std::string const&, InfoBox::TextType const&);
    extern template void PrintBox::BoxT<COLORREF>(std::string const&, COLORREF const&);
};

#define TRACEBox(A,B)     Editor::PrintBox::instance().colorBox(A, B, "", -1)
#define TRACEBoxEx(A,B)   Editor::PrintBox::instance().colorBox(A, B, __FUNCTION__, __LINE__)
#define ERRORBox()        Editor::PrintBox::instance().errorBox(__FUNCTION__, __LINE__)
#define ERRORBoxEx(A)     Editor::PrintBox::instance().errorBox(__FUNCTION__, __LINE__, A)
#define ALERTBox(A)       Editor::PrintBox::instance().alertBox(A, __FUNCTION__, __LINE__)
#define DEFAULTBox(A)     Editor::PrintBox::instance().defaultBox(A)
#define INFOBox(A)        Editor::PrintBox::instance().infoBox(A)
#define CLEARBox()        Editor::PrintBox::instance().clearBox()
#define COLORBox(A,R,G,B) Editor::PrintBox::instance().BoxT<COLORREF>(A, RGB(R,G,B))
#define COLORREFBox(A,C)  Editor::PrintBox::instance().BoxT<COLORREF>(A, C)
