
#include "SCEditInternal.h"
#include "HelperMap.h"
#include "EditBoxStyle.h"

#include "Scintilla.h"
#include "SciLexer.h"

namespace Editor
{

#   include "EditBoxConstant.h"

    HelperMap::StringMap l_toolTip = HelperMap::InitMap<std::string, std::string>
#   define LFUN__(S,A,B,...) (STR_(A), B)
#   include <LuaPluginFunction.h>
    ;

    EditBox::EditBox()
    {
        init(nullptr);
        initstyle(EditBox::StyleTheme::EB_STYLE_LIGHT);
    }

    EditBox::EditBox(HINSTANCE hinst_, EditBox::StyleTheme theme_)
    {
        init(hinst_);
        initstyle(theme_);
    }

    EditBox::~EditBox()
    {
#       if !defined(_BUILD_DYNAMIC_SC)
        Scintilla_ReleaseResources();
#       endif
    }

    void EditBox::init(HINSTANCE hinst_)
    {
        try
        {
#           if defined(_BUILD_DYNAMIC_SC)
            m_liblex.init("scilexer.dll");
            if (!m_liblex.isload())
                if (m_liblex.geterror())
                    ERRORBoxEx(m_liblex.geterror());
#           else
            if (hinst_)
            {
                Scintilla_RegisterClasses(hinst_);
                Scintilla_LinkLexers();
            }
#           endif
        }
        catch (std::exception & ex_)
        {
            ALERTBox(ex_.what()); return;
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return;
        }
    }

    bool EditBox::issaved()
    {
        return !static_cast<bool>(cmd(SCI_GETMODIFY));
    }

    template <typename T>
    bool EditBox::gettext(T & s)
    {
        int32_t sz = cmd(SCI_GETLENGTH);
        if (sz <= 0)
            return false;

        auto dbuf = std::make_unique<char[]>(++sz);
        if (!cmd(SCI_GETTEXT, sz, reinterpret_cast<LPARAM>(dbuf.get())))
            return false;

        if constexpr (std::is_same<T, std::string>::value)
            s.assign(dbuf.get());
        else if constexpr (std::is_same<T, std::vector<char>>::value)
            s.assign(dbuf.get(), dbuf.get() + sz);
        return true;
    }
    template bool EditBox::gettext<std::string>(std::string&);
    template bool EditBox::gettext<std::vector<char>>(std::vector<char>&);


    bool EditBox::vieweol()
    {
        bool ret = !(cmd(SCI_GETVIEWEOL));
        (void) cmd(SCI_SETVIEWEOL, ret);
        return ret;
    }

    LRESULT EditBox::cmd(UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        if (!m_hwnd)
            return 0;
        return ::SendMessage(m_hwnd, umsg_, wp_, lp_);
    }

    LRESULT EditBox::command(UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        return cmd(umsg_, wp_, lp_);
    }

    bool EditBox::create(HWND hwnd_, HINSTANCE hinst_, RECT const & r, std::string const & s)
    {
        do
        {
            init(hinst_);

            if (!WindowBase::create(
                hwnd_,
                hinst_,
                (HMENU)ID_EDITBOX,
                r,
                WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                "Scintilla",
                "EditableSource"
                ))
                break;

            if (!setup(s))
                break;

            WindowBase::active();
            return true;
        }
        while (0);
        return false;
    }

#   include "EditBox.Service.cxx"
#   include "EditBox.Notify.cxx"
#   include "EditBox.Navigate.cxx"

};
