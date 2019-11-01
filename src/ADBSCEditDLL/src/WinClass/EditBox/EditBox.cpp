/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../../SCEditInternal.h"
#include "../../Utils/HelperMap.h"
#include "EditBoxStyle.h"

#include <Scintilla.h>
#include <SciLexer.h>

namespace Editor
{

#   include "EditBoxConstant.h"

    HelperMap::StringMap l_toolTip = HelperMap::InitMap<std::string, std::string>
#   define LFUN__(S,A,H1,H2,...) (STR_(A), H1 H2)
#   include <LuaPluginFunction.h>
    ;

    ///

    LRESULT CALLBACK EditBox::EditWndProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, UINT_PTR, DWORD_PTR ptr)
    {
        switch (umsg_)
        {
            case WM_DROPFILES:
                {
                    HDROP hdrop = reinterpret_cast<HDROP>(wp_);
                    if (!hdrop)
                        return 0;

                    do
                    {
                        std::vector<char> v(MAX_PATH);
                        uint32_t n = ::DragQueryFileA(hdrop, 0xFFFFFFFF, nullptr, 0);
                        if (!n)
                        {
                            ALERTBox(l_dropEmpty);
                            break;
                        }
                        if (n > 1)
                        {
                            ALERTBox(l_dropWarn);
                            break;
                        }
                        if (!::DragQueryFileA(hdrop, 0, &v[0], MAX_PATH))
                            break;

                        std::string s = &v[0];
                        if (s.empty())
                            break;
                        //
                        {
                            std::stringstream ss;
                            ss << l_dropOpen << s.c_str();
                            INFOBox(ss.str());
                        }
                        Editor::EditBox *eb = reinterpret_cast<Editor::EditBox*>(ptr);
                        if (eb)
                        {
                            if (eb->filevalidext(s))
                                (void) eb->fileopen(s);
                            else
                                ALERTBox(l_dropErrorExt);
                        }
                        else
                            ALERTBox(l_dropErrorHandle);
                    }
                    while (0);

                    ::DragFinish(hdrop);
                    return 1;
                }
        }
        return ::DefSubclassProc(hwnd_, umsg_, wp_, lp_);
    }

    EditBox::EditBox()
    {
        Base::m_data = data();
        MDIWin::Config::instance().setclass<MDIWin::Config::HandleType::CLASS_EDIT>(this);
    }

    EditBox::~EditBox()
    {
        MDIWin::Config::instance().setclass<MDIWin::Config::HandleType::CLASS_EDIT>(nullptr);
#       if !defined(_BUILD_DYNAMIC_SC)
        Scintilla_ReleaseResources();
#       endif
        if (m_hwnd)
        {
            ::RemoveWindowSubclass(m_hwnd, &EditBox::EditWndProc, 0);
            ::DestroyWindow(m_hwnd);
        }
        m_hwnd = nullptr;
    }

    void EditBox::initscintilla()
    {
        try
        {
#           if defined(_BUILD_DYNAMIC_SC)
            m_liblex.init("scilexer.dll");
            if (!m_liblex.isload())
                if (m_liblex.geterror())
                    ERRORBoxEx(m_liblex.geterror());
#           else
            if (MDIWin::Config::instance().gethinstance())
            {
                Scintilla_RegisterClasses(
                        MDIWin::Config::instance().gethinstance()
                    );
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

    /// base overwrite

    MDIWin::WinData EditBox::data()
    {
        MDIWin::WinData d(
                MDIWin::BaseData::MDIWinType::MWTYPE_CHILD,       // MDIWinType group
                MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_EDIT,  // MDIWinType type
                MDIWin::BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE, // MDIWinStyle
                std::string(),                                    // Class name
                "Edit Lua script source",                         // Title
                (WS_POPUP | WS_SYSMENU),
                WS_EX_ACCEPTFILES
            );
        d.irdefault.set<int32_t>(0, 0, 100, 70);                  // % from main widow
        return d;
    }

    void EditBox::sizeinit_()
    {
        do
        {
            if (!m_hwnd)
                break;

            (void) ::SetWindowPos(
                    m_hwnd,
                    0,
                    0, 0, Base::m_data.irsize.w, Base::m_data.irsize.h,
                    SWP_NOACTIVATE | SWP_SHOWWINDOW
                );
        }
        while (0);
    }

    bool EditBox::event_sizeinitEnd()
    {
        sizeinit_();
        return true;
    }
    void EditBox::event_resize()
    {
        sizeinit_();
    }

    void EditBox::event_size(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const & ire)
    {
        if (ire.empty())
        {
            // ToolBar expand
            if (Base::m_data.irsize.y == irc.y)
                return;

            Base::m_data.irsize.set<int32_t>(
                    0,
                    irc.y,
                    Base::m_data.irsize.w,
                    Base::m_data.irsize.h
                );
            Base::refresh(SWP_NOSIZE);
        }
        else
            Base::m_data.irsize.set<int32_t>(
                    0,
                    irc.y,
                    ((ire.w > irc.w) ? irc.w : ((ire.w < 20) ? irc.w : ire.w)),
                    ((ire.h > irc.h) ? irc.h : ire.h)
                );
    }

    void EditBox::show()
    {
        Base::refresh(SWP_NOMOVE);
    }

    std::tuple<bool, bool> EditBox::event_initEnd()
    {
        do
        {
            if (!Base::m_data.window)
                break;

            initscintilla();
            initstyle(EditBox::StyleTheme::EB_STYLE_LIGHT);

            m_hwnd = ::CreateWindowExA(
                            Base::m_data.wflagex,
                            "Scintilla", "",
                            (WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL),
                            0, 0, 1, 1,
                            Base::m_data.window,
                            (HMENU)ID_EDITBOX,
                            MDIWin::Config::instance().gethinstance(),
                            nullptr
                    );
            if (!m_hwnd)
                break;

            if (!setup())
                break;

            ::SetWindowSubclass(
                    m_hwnd,
                    &EditBox::EditWndProc,
                    0,
                    reinterpret_cast<DWORD_PTR>(this)
                );
            return { true, true };
        }
        while (0);
        return { true, false };
    }

#   include "EditBox.Service.cxx"
#   include "EditBox.Notify.cxx"
#   include "EditBox.Navigate.cxx"

};
