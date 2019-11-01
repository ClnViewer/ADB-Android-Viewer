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
#include <commctrl.h>

namespace Editor
{
    struct TBChangeButton
    {
        int32_t btnidx, cmdcur, cmdnew, imgidx, state;
    };

    ToolBar::ToolBar()
    {
        Base::m_data = data();
    }

    void ToolBar::show()
    {
        if (!Base::m_data.window)
            return;
        //
        Base::refresh(m_rebar.gethandle());
        Base::refresh(Base::m_data.window);
    }

    MDIWin::WinData ToolBar::data()
    {
        MDIWin::WinData d(
                    MDIWin::BaseData::MDIWinType::MWTYPE_TOOLBAR,          // MDIWinType group
                    MDIWin::BaseData::MDIWinType::MWTYPE_TOOLBAR,          // MDIWinType type
                    MDIWin::BaseData::MDIWinStyle::MWSTYLE_NONE,           // MDIWinStyle
                    std::string(),                                         // Class name
                    std::string(),                                         // Title
                    0, 0
                );
        d.irdefault.set<int32_t>(0, 0, 0, 0);                              // % from main widow
        return d;
    }

    bool ToolBar::event_sizeinitBegin(ImageLite::IRECT<int32_t> const & ir)
    {
        do
        {
            if ((!Base::m_data.window) || (ir.w <= 0))
                break;

            Base::m_data.irsize.set<int32_t>(
                0,
                0,
                ir.w - 150,
                (getsizebtn_() + 4)
            );

            if (Base::m_data.irsize.empty())
                break;
            Base::refresh(SWP_NOMOVE);
        }
        while (0);
        return true;
    }

    void ToolBar::event_size(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const &)
    {
        Base::m_data.irsize.set<int32_t>(
                0,
                0,
                ((irc.w <= 0) ? Base::m_data.irsize.w : irc.w),
                Base::m_data.irsize.h
            );
        Base::resize();
    }

    void ToolBar::event_resize()
    {
        RECT rc{};
        HWND hwnd = m_rebar.gethandle();
        if (!hwnd)
            hwnd = Base::m_data.window;

        if (!::GetClientRect(hwnd, &rc))
            return;

        Base::m_data.irsize.set<int32_t>(
                0,
                0,
                rc.right,
                rc.bottom
            );
    }

    std::tuple<bool, bool> ToolBar::event_initBegin()
    {
        try
        {
            Base::m_data.irsize = {};

            do
            {
                if (!m_rebar.init(Base::m_data.wparent))
                    break;

                if (!m_search.init(
                        m_rebar.gethandle(),
                        Base::m_data.wparent
                    ))
                    break;

                if (!m_menu.init(
                        m_rebar.gethandle(),
                        Base::m_data.wparent
                    ))
                    break;

                Base::m_data.window = ::CreateWindowExA(
                            WS_EX_COMPOSITED,
                            TOOLBARCLASSNAME, "",
                            WS_VISIBLE          |
                            WS_CHILD            |
                            WS_CLIPCHILDREN     |
                            WS_CLIPSIBLINGS     |
                            TBSTYLE_FLAT        |
                            TBSTYLE_WRAPABLE    |
                            TBSTYLE_TRANSPARENT |
                            CCS_NORESIZE        |
                            CCS_NODIVIDER       |
                            CCS_NOPARENTALIGN,
                            0, 0, 1, 1,
                            Base::m_data.wparent,
                            (HMENU)ID_REBAR_TOOLBAR,
                            MDIWin::Config::instance().gethinstance(),
                            nullptr
                    );
                if (!Base::m_data.window)
                    break;

                if (!setup_())
                    break;

                m_rebar.addband(Base::m_data.window, getsizebtn_() + 4);
                m_rebar.addband(m_search.gethandle(), 0);
                m_rebar.addband(m_menu.gethandle(), 0);
                m_rebar.showband(0);
                //
                MDIWin::Config::instance()
                        .setclass<MDIWin::Config::HandleType::CLASS_TBAR>(this);
                //
                return { true, true };
            }
            while (0);
        }
        catch (...) {}
        return { true, false };
    }

    int32_t ToolBar::getsizebtn_()
    {
        if (!Base::m_data.window)
            return -1;

        LRESULT ret = ::SendMessage(Base::m_data.window, TB_GETBUTTONSIZE, 0, 0);
        auto p = MAKEPOINTS(ret);
        if (p.x != p.y)
            return -1;
        return p.y;
    }

    bool ToolBar::setup_()
    {
        ::SetLastError(0);

        const TBBUTTON tbb[] =
        {
#           define TB_ITEM(A,B,C,...) TB_ITEM_ ## B (A,B,C)
#           define TB_ITEM_TBSTYLE_BUTTON(A,B,C) \
                { .iBitmap = __COUNTER__, .idCommand = C, .fsState = A, .fsStyle = B, .bReserved = {}, .dwData = 0, .iString = 0 },
#           define TB_ITEM_TBSTYLE_SEP(A,B,C) \
                { .iBitmap = 0, .idCommand = C, .fsState = A, .fsStyle = B, .bReserved = {}, .dwData = 0, .iString = 0 },
#           define TB_CHANGE(...)
#           include "ToolBarItems.h"
        };
        //int32_t imgcnt = (__COUNTER__ + 3);

        ImageLite::IPOINT<int32_t> ip{};
        MDIWin::Config::instance().getiml(
                MDIWin::Config::ImageButtonList::IMLBTN_TITLEBAR_CTRL, ip
            );
        if (ip.empty())
            return false;

        ip.x += 2;
        ip.y += 2;

        ::SendMessage(Base::m_data.window, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
        ::SendMessage(Base::m_data.window, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
        ::SendMessage(Base::m_data.window, TB_SETIMAGELIST, (WPARAM)0, (LPARAM)
                MDIWin::Config::instance().getiml(MDIWin::Config::ImageButtonList::IMLBTN_TOOLBAR_CTRL)
            );
        ::SendMessage(Base::m_data.window, TB_SETBUTTONSIZE, 0, MAKELPARAM(ip.x, ip.y));
        ::SendMessage(Base::m_data.window, TB_ADDBUTTONS, __NELE(tbb), (LPARAM)&tbb);
        ::SendMessage(Base::m_data.window, TB_SETPADDING, 0, MAKELPARAM(0, 0));
        ::SendMessage(Base::m_data.window, TB_AUTOSIZE, 0, 0);
        ::SendMessage(Base::m_data.window, WM_SETFONT, (WPARAM)MDIWin::Config::instance().getfont(), (LPARAM)1);
        //
        return true;
    }

    // get search text
    std::string ToolBar::event()
    {
        return m_search.gettext();
    }

    // change button
    void ToolBar::event(int32_t bidx, int32_t bcmd, int32_t bset, int32_t bimg)
    {
        static const TBChangeButton l_chbtn[] =
        {
#           define TB_CHANGE(A,B,C,D,E) \
                { .btnidx = A, .cmdcur = B, .cmdnew = C, .imgidx = D, .state = E },
#           define TB_ITEM(...)
#           include "ToolBarItems.h"
        };

        do
        {
            if (bidx > ::SendMessage(Base::m_data.window, TB_BUTTONCOUNT, 0, 0))
                break;

            TBBUTTON tbb{};
            if (!::SendMessage(Base::m_data.window, TB_GETBUTTON, bidx, reinterpret_cast<LPARAM>(&tbb)))
                break;

            if (tbb.fsStyle == TBSTYLE_SEP)
                break;

            for (uint32_t i = 0U; i < __NELE(l_chbtn); i++)
            {
                if ((l_chbtn[i].btnidx == bidx) &&
                    (
                     ((bcmd == -1) && (l_chbtn[i].cmdcur == tbb.idCommand)) ||
                     ((bcmd != -1) && (l_chbtn[i].cmdnew == bcmd))
                    ))
                {
                    //
                    TBChangeButton btn{};
                    btn.cmdcur = tbb.idCommand;
                    btn.cmdnew = ((bcmd == -1) ? l_chbtn[i].cmdnew : bcmd);
                    btn.state  = ((bset == -1) ? l_chbtn[i].state  : bset);
                    btn.imgidx = ((bimg == -1) ? l_chbtn[i].imgidx : bimg);
                    //
                    if ((btn.state != -1) && (btn.state != tbb.fsState))
                        ::SendMessage(Base::m_data.window, TB_SETSTATE, btn.cmdcur, (LPARAM)btn.state);
                    //
                    if ((btn.imgidx != -1) && (btn.imgidx != tbb.iBitmap))
                        ::SendMessage(Base::m_data.window, TB_CHANGEBITMAP, btn.cmdcur, btn.imgidx);
                    //
                    if ((btn.cmdnew != -1) && (btn.cmdnew != tbb.idCommand))
                        ::SendMessage(Base::m_data.window, TB_SETCMDID, btn.cmdcur, btn.cmdnew);
                    //
                    break;
                }
            }
        }
        while (0);
    }

    // set plugins toolBar menu
    void ToolBar::event(uint32_t mid, uint32_t off, std::vector<MDIWin::BaseData::dataMap> & v)
    {
        m_menu.add(
                static_cast<MenuBar::MainMenuId>(mid),
                off,
                v
            );
    }

    // set style toolBar button (image/enable/disable)
    void ToolBar::event(int32_t id, bool isenable)
    {
        switch (id)
        {
            case IDM_EVENT_SCRUN_START:
                {
                    event(TB_BTN_CHECK,     -1, 0);
                    event(TB_BTN_START,     -1, 0);
                    event(TB_BTN_EXIT,      -1, 0);
                    event(TB_BTN_STOP,      -1, TBSTATE_ENABLED);

                    m_menu.setenable(IDM_BTN_SCRUN_TEST,  false);
                    m_menu.setenable(IDM_BTN_SCRUN_START, false);
                    m_menu.setenable(IDM_BTN_EXIT,   false);
                    m_menu.setenable(IDM_BTN_SCRUN_STOP,  true);

                    if (isenable)
                    {
                        event(TB_BTN_MODE,      IDM_BTN_DBGDUMP);
                        event(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE, -1, 21);
                        //
                        m_menu.setcheck(IDM_BTN_DBGSTEP,  true);
                        m_menu.setcheck(IDM_BTN_DBGCYCLE, false);
                        //
                        m_menu.setenable(IDM_BTN_DBGDUMP,  true);
                        m_menu.setenable(IDM_BTN_DBGNEXT,  true);
                    }
                    else
                    {
                        m_menu.setcheck(IDM_BTN_DBGSTEP,   false);
                        m_menu.setcheck(IDM_BTN_DBGCYCLE,  true);
                        //
                        m_menu.setenable(IDM_BTN_DBGDUMP,  false);
                        m_menu.setenable(IDM_BTN_DBGNEXT,  false);
                    }
                    break;
                }
            case IDM_EVENT_SCRUN_STOP:
                {
                    event(TB_BTN_MODE,      IDM_BTN_DBGCYCLE);
                    event(TB_BTN_CHECK,     -1, TBSTATE_ENABLED);
                    event(TB_BTN_START,     -1, TBSTATE_ENABLED);
                    event(TB_BTN_EXIT,      -1, TBSTATE_ENABLED);
                    event(TB_BTN_STOP,      -1, 0);
                    event(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE);
                    //
                    m_menu.setenable(IDM_BTN_SCRUN_TEST,  true);
                    m_menu.setenable(IDM_BTN_SCRUN_START, true);
                    m_menu.setenable(IDM_BTN_EXIT,   true);
                    m_menu.setenable(IDM_BTN_SCRUN_STOP,  false);
                    //
                    m_menu.setenable(IDM_BTN_DBGDUMP,  false);
                    m_menu.setenable(IDM_BTN_DBGNEXT,  false);
                    //
                    m_menu.setcheck(IDM_BTN_DBGSTEP,  false);
                    m_menu.setcheck(IDM_BTN_DBGCYCLE, true);

                    break;
                }
            case IDM_BTN_DBGMODE:
                {
                    event(TB_BTN_MODE,
                                ((isenable) ? IDM_BTN_DBGSTEP : IDM_BTN_DBGCYCLE)
                            );
                    m_menu.setcheck(IDM_BTN_DBGSTEP,   isenable);
                    m_menu.setcheck(IDM_BTN_DBGCYCLE, !isenable);
                    break;
                }
            case IDM_BTN_DBGSTEP:
                {
                    event(TB_BTN_MODE, IDM_BTN_DBGSTEP);
                    m_menu.setcheck(IDM_BTN_DBGSTEP,  true);
                    m_menu.setcheck(IDM_BTN_DBGCYCLE, false);
                    break;
                }
            case IDM_BTN_DBGCYCLE:
                {
                    event(TB_BTN_MODE, IDM_BTN_DBGCYCLE);
                    m_menu.setcheck(IDM_BTN_DBGSTEP,  false);
                    m_menu.setcheck(IDM_BTN_DBGCYCLE, true);
                    break;
                }

            case IDM_EVENT_EDIT_FINDTEXT:
                {
                    if (isenable)
                        event(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE, -1, 21);
                    else
                        event(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE);
                    break;
                }
            case IDM_BTN_WINTOP:
            case IDM_BTN_EXTDBGV:
            case IDM_BTN_DBGMUTE:
            case IDM_EDIT_SHOW_ENDLINE:
            case IDM_EDIT_SHOW_INDENTG:
                {
                    m_menu.setcheck(id, isenable);
                    break;
                }
            default:
                break;
        }
    }

};
