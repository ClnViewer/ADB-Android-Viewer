
#include "SCEditInternal.h"
#include "extern/tixml2ex.h"
#include "HelperMap.h"

#include "Scintilla.h"
#include "SciLexer.h"

#define  EDITBOX_STYLE_ACTIVE 1
#include "EditBoxStyle.h"

namespace Editor
{
    static const char l_style_file[]  = "ADBSCEdit.style";
    static const char l_style_sect[]  = "SCEdit/Style/Option";
    static const char l_style_attr1[] = "name";
    static const char l_style_attr2[] = "value";

#   include "EditBoxConstant.h"

   // convert HTML colors to COLORREF
    static COLORREF f_convert_color(std::string const & s)
    {
        try
        {
            uint32_t u = std::stoul(s, 0, 16);
            return RGB(
                       ((u >> 16) & 255),
                       ((u >> 8) & 255),
                       ( u  & 255)
                    );
        }
        catch(...)
        {
            return 0U;
        }
    }

    static void f_parse_section(
        const tinyxml2::XMLDocument & root,
        std::function<void(std::string&, std::string&)> f)
    {
        for (auto item : tinyxml2::selection(root, l_style_sect))
        {
            std::string name_ = attribute_value(item, l_style_attr1);
            std::string val_  = attribute_value(item, l_style_attr2);
            if ((name_.empty()) || (val_.empty()))
                continue;
            f(name_, val_);
        }
    }

    void EditBox::initstyle(EditBox::StyleTheme theme)
    {
        try
        {
            do
            {
                if (::_access(l_style_file, F_OK) < 0)
                    break;

                std::map<std::string, Editor::e_color> mapenum = HelperMap::InitMap<std::string, Editor::e_color>
#               define EBSTYLE__(A,...) (STR_(A), Editor::e_color::SC_COLOR_ ## A)
#               include "EditBoxStyleBase.h"
                ;
                auto doc = tinyxml2::load_xmlfile(l_style_file);
                f_parse_section(
                    static_cast<const tinyxml2::XMLDocument&>(*doc),
                    [=](std::string & name_, std::string & val_)
                    {
                        auto r = mapenum.find(name_);
                        if (r == mapenum.end())
                            return;

                        uint32_t n = static_cast<uint32_t>(r->second);
                        if (n >= __NELE(m_styleColor))
                            return;
                        m_styleColor[n] = f_convert_color(val_);
                    }
                );
            }
            while (0);

            for (uint32_t i = 0U; i < __NELE(m_styleColor); i++)
                if (!m_styleColor[i])
                    m_styleColor[i] = l_color[theme][i];

            return;
        }
        catch (tinyxml2::XmlException & _ex)
        {
            ALERTBox(_ex.what()); return;
        }
        catch (std::exception & _ex)
        {
            ALERTBox(_ex.what()); return;
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return;
        }
    }

    void EditBox::style(int32_t style_, COLORREF fore_, COLORREF back_, int32_t size_, std::string const & face_)
    {
        cmd(SCI_STYLESETFORE, style_, fore_);
        cmd(SCI_STYLESETBACK, style_, back_);
        if (size_ >= 1)
            cmd(SCI_STYLESETSIZE, style_, size_);
        if (!face_.empty())
            cmd(SCI_STYLESETFONT, style_, reinterpret_cast<LPARAM>(face_.c_str()));
    }

    void EditBox::style(int32_t style_, COLORREF fore_, COLORREF back_)
    {
        style(style_, fore_, back_, -1, "");
    }

    void EditBox::style(int32_t style_, COLORREF fore_)
    {
        style(style_, fore_, m_styleColor[e_color::SC_COLOR_TEXTS], -1, "");
    }

    void EditBox::style(int32_t style_)
    {
        style(style_, m_styleColor[e_color::SC_COLOR_BG], m_styleColor[e_color::SC_COLOR_TEXTS], -1, "");
    }

    bool EditBox::setup(HWND hwnd_, RECT const & r, std::string const & s)
    {
        if (!hwnd_)
            return false;
        m_hwnd = hwnd_;

        if ((!r.right) || (!r.bottom))
            return false;

        setsize(r);
        return setup(s);
    }

    bool EditBox::setup(std::string const & s)
    {
        /// cmd(SCI_STYLECLEARALL);
        cmd(SCI_STYLERESETDEFAULT);
        cmd(SCI_SETLEXER, SCLEX_LUA);
        cmd(SCI_SETCODEPAGE, SC_CP_UTF8);

        cmd(SCI_SETCARETFORE, m_styleColor[e_color::SC_COLOR_CARET]);
        cmd(SCI_SETTABWIDTH, 4);
        cmd(SCI_SETINDENTATIONGUIDES, 1);
        cmd(SCI_SETINDENT, 4);

        cmd(SCI_CONVERTEOLS, 2, 0);
        cmd(SCI_SETEOLMODE,  2, 0);

        // Selected line (CARET)
        cmd(SCI_SETCARETSTYLE, 1);
        cmd(SCI_SETCARETPERIOD, 400);
        cmd(SCI_SETCARETLINEVISIBLE, 1);
        cmd(SCI_SETCARETLINEBACK, m_styleColor[e_color::SC_COLOR_ACTIVELINE]);

        // Syntax words color
        cmd(SCI_SETKEYWORDS,          0, reinterpret_cast<LPARAM>(l_buildLuaRoot));
        cmd(SCI_SETKEYWORDS,          1, reinterpret_cast<LPARAM>(l_buildObject));
        cmd(SCI_SETKEYWORDS,          2, reinterpret_cast<LPARAM>(l_luaWord3));
        cmd(SCI_SETKEYWORDS,          3, reinterpret_cast<LPARAM>(l_luaWord4));
        cmd(SCI_SETKEYWORDS,          4, reinterpret_cast<LPARAM>(l_luaWord5));
        cmd(SCI_SETKEYWORDS,          5, reinterpret_cast<LPARAM>(l_luaWord6));
        cmd(SCI_SETKEYWORDS,          6, reinterpret_cast<LPARAM>(l_luaWord7));

        style(SCE_LUA_WORD,           m_styleColor[e_color::SC_COLOR_WORD1],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD2,          m_styleColor[e_color::SC_COLOR_WORD2],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD3,          m_styleColor[e_color::SC_COLOR_WORD3],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD4,          m_styleColor[e_color::SC_COLOR_WORD4],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD5,          m_styleColor[e_color::SC_COLOR_WORD5],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD6,          m_styleColor[e_color::SC_COLOR_WORD6],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_WORD7,          m_styleColor[e_color::SC_COLOR_WORD7],  m_styleColor[e_color::SC_COLOR_BG]);

        style(STYLE_INDENTGUIDE,      m_styleColor[e_color::SC_COLOR_INDENT], m_styleColor[e_color::SC_COLOR_BG]);
        style(STYLE_DEFAULT,          m_styleColor[e_color::SC_COLOR_TEXTS],  m_styleColor[e_color::SC_COLOR_BG], 11, Editor::Config::instance().getfontname().c_str());
        style(SCE_LUA_DEFAULT,        m_styleColor[e_color::SC_COLOR_TEXTS],  m_styleColor[e_color::SC_COLOR_BG], 11, Editor::Config::instance().getfontname().c_str());

        style(SCE_LUA_COMMENT,        m_styleColor[e_color::SC_COLOR_COMMENT],m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_COMMENTLINE,    m_styleColor[e_color::SC_COLOR_COMMENT],m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_COMMENTDOC,     m_styleColor[e_color::SC_COLOR_COMMENT],m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_NUMBER,         m_styleColor[e_color::SC_COLOR_NUMBERS],m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_LABEL,          m_styleColor[e_color::SC_COLOR_LABEL],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_STRING,         m_styleColor[e_color::SC_COLOR_STRING], m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_CHARACTER,      m_styleColor[e_color::SC_COLOR_CHARS],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_LITERALSTRING,  m_styleColor[e_color::SC_COLOR_CHARS],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_OPERATOR,       m_styleColor[e_color::SC_COLOR_SYNTAX], m_styleColor[e_color::SC_COLOR_BG]); // OK
        style(SCE_LUA_IDENTIFIER,     m_styleColor[e_color::SC_COLOR_IDENT],  m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_PREPROCESSOR,   m_styleColor[e_color::SC_COLOR_PREPROC],m_styleColor[e_color::SC_COLOR_BG]);
        style(SCE_LUA_STRINGEOL,      m_styleColor[e_color::SC_COLOR_CARET],  m_styleColor[e_color::SC_COLOR_BG]);

                /*
        style(SCE_LUA_WORD2
        style(SCE_LUA_WORD3
        style(SCE_LUA_WORD4
        style(SCE_LUA_WORD5
        style(SCE_LUA_WORD6
        style(SCE_LUA_WORD7
        style(SCE_LUA_WORD8
                */

        // Line number
        cmd(SCI_SETMARGINTYPEN,      _INDEX_MARGIN_LNUM, SC_MARGIN_NUMBER);
        cmd(SCI_SETMARGINWIDTHN,     _INDEX_MARGIN_LNUM, 40);
        cmd(SCI_SETMARGINSENSITIVEN, _INDEX_MARGIN_LNUM, 1);

        // Folding
        const char *l_true  = "1";
        const char *l_false = "0";

        cmd(SCI_SETMARGINWIDTHN, _INDEX_MARGIN_FOLD);
        cmd(SCI_SETPROPERTY, (WPARAM)"fold", (LPARAM)l_true);
        cmd(SCI_SETPROPERTY, (WPARAM)"fold.compact", (LPARAM)l_false);
        cmd(SCI_SETPROPERTY, (WPARAM)"fold.comment", (LPARAM)l_true);
        cmd(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)l_true);
        cmd(SCI_SETPROPERTY, (WPARAM)"lua.default.language", (LPARAM)l_true);
        cmd(SCI_SETMARGINWIDTHN, _INDEX_MARGIN_FOLD);
        cmd(SCI_SETMARGINTYPEN,  _INDEX_MARGIN_FOLD, SC_MARGIN_SYMBOL);
        cmd(SCI_SETMARGINMASKN,  _INDEX_MARGIN_FOLD, SC_MASK_FOLDERS);
        cmd(SCI_SETMARGINWIDTHN, _INDEX_MARGIN_FOLD, 25);

        cmd(SCI_MARKERDEFINE, 0,                         SC_MARK_CIRCLE);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER,         SC_MARK_BOXPLUS);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN,     SC_MARK_BOXMINUS);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,      SC_MARK_CIRCLEPLUSCONNECTED);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID,  SC_MARK_CIRCLEMINUSCONNECTED);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL,  SC_MARK_TCORNERCURVE);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB,      SC_MARK_VLINE);
        cmd(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL,     SC_MARK_LCORNERCURVE);

        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDER,        m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDER,        m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN,    m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPEN,    m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND,     m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEREND,     m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPENMID, m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERSUB,     m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB,     m_styleColor[e_color::SC_COLOR_FOLDER]);
        cmd(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERTAIL,    m_styleColor[e_color::SC_COLOR_TEXTS]);
        cmd(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL,    m_styleColor[e_color::SC_COLOR_FOLDER]);

        cmd(SCI_SETFOLDFLAGS, 4, 0);
        cmd(SCI_SETMARGINSENSITIVEN, _INDEX_MARGIN_FOLD, 1); // On

        // Markers bookmark etc..
        int32_t mask = 0,
                marker = 1;
        mask = serviceAddMark(marker++, mask, l_bookMarkXpm);
        mask = serviceAddMark(marker++, mask, l_errorMarkXpm);
        mask = serviceAddMark(marker,   mask, l_debugMarkXpm);
        cmd(SCI_SETMARGINMASKN, _INDEX_MARGIN_FOLD, mask);

        // Indicators
        cmd(SCI_INDICSETSTYLE,        _FIND_INDICATOR, INDIC_STRAIGHTBOX);
        cmd(SCI_INDICSETFORE,         _FIND_INDICATOR, m_styleColor[e_color::SC_COLOR_FINDIND]);
        cmd(SCI_INDICSETALPHA,        _FIND_INDICATOR, 10);
        cmd(SCI_INDICSETOUTLINEALPHA, _FIND_INDICATOR, 100);

        // Auto Completion/ToolTip list
        cmd(SCI_AUTOCSETORDER,         1, SC_ORDER_PERFORMSORT);
        cmd(SCI_AUTOCSETFILLUPS,       0, reinterpret_cast<LPARAM>(l_autoAppend));
        cmd(SCI_AUTOCSTOPS,            0, reinterpret_cast<LPARAM>(l_autoStop));
        cmd(SCI_REGISTERIMAGE,         1, reinterpret_cast<LPARAM>(l_autoMethodXpm));
        cmd(SCI_SETMOUSEDWELLTIME,     900);
        cmd(SCI_CALLTIPSETFOREHLT,     m_styleColor[e_color::SC_COLOR_WORD1]);


        if (!s.empty())
            cmd(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(s.data()));

        return true;
    }
};
