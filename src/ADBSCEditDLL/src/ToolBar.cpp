
#include "SCEditInternal.h"
#include <commctrl.h>

namespace Editor
{
    struct TBChangeButton
    {
        int32_t btnidx, cmdcur, cmdnew, imgidx, state;
    };

    bool ToolBar::getsize(RECT & r)
    {
        HWND hwnd = m_rebar.handle();
        if (!hwnd)
            hwnd = WindowBase::m_hwnd;
        if (!hwnd)
            return false;

        RECT rc{};
        if (!::GetWindowRect(hwnd, &rc))
            return false;
        r = {};
        r.right  = (rc.right - rc.left);
        r.bottom = (rc.bottom - rc.top);
        return true;
    }

    int32_t ToolBar::getbtnsize()
    {
        if (!WindowBase::m_hwnd)
            return -1;

        LRESULT ret = SendMessage(WindowBase::m_hwnd, TB_GETBUTTONSIZE,0,0);
        return MAKEPOINTS(ret).y;
    }

    bool ToolBar::create(HWND hwnd_, HINSTANCE hinst_)
    {
        do
        {
            if (!m_rebar.create(hwnd_, hinst_))
                break;

            if (!m_findbar.create(m_rebar.handle(), hwnd_, hinst_))
                break;

            if (!m_menu.create(m_rebar.handle(), hwnd_, hinst_))
                break;

            if (!WindowBase::create(
                hwnd_,
                hinst_,
                (HMENU)ID_REBAR_TOOLBAR,
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
                TOOLBARCLASSNAME, ""
                ))
                break;

            if (!setup(hinst_))
                break;

            m_rebar.addband(handle(), getbtnsize());
            m_rebar.addband(m_findbar.handle(), 0);
            m_rebar.addband(m_menu.handle(), 0);
            m_rebar.showband(0);

            WindowBase::active();
            return true;
        }
        while (0);
        return false;
    }

    bool ToolBar::setup(HINSTANCE hinst_)
    {
        if (!m_hwnd)
            return false;

        ::SetLastError(0);

        TBADDBITMAP tbimg{};
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
        int32_t imgcnt = (__COUNTER__ + 3);

        tbimg.hInst = hinst_;
        tbimg.nID   = ID_TOOLBAR_BITMAP;

        ::SendMessage(m_hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
        ::SendMessage(m_hwnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
        ::SendMessage(m_hwnd, TB_SETBITMAPSIZE, (WPARAM)0, (LPARAM)MAKELONG(30, 30));
        if (::SendMessage(m_hwnd, TB_ADDBITMAP, (WPARAM)imgcnt, (LPARAM)&tbimg) == -1)
            return false;

        ::SendMessage(m_hwnd, TB_ADDBUTTONS, __NELE(tbb), (LPARAM)&tbb);
        ::SendMessage(m_hwnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(30, 30));
        ::SendMessage(m_hwnd, TB_SETPADDING, 0, MAKELPARAM(0, 0));
        ::SendMessage(m_hwnd, TB_AUTOSIZE, 0, 0);
        return true;
    }

    void ToolBar::changebutton(int32_t bidx, int32_t bcmd, int32_t bset, int32_t bimg)
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
            if (bidx > ::SendMessage(m_hwnd, TB_BUTTONCOUNT, 0, 0))
                break;

            TBBUTTON tbb{};
            if (!::SendMessage(m_hwnd, TB_GETBUTTON, bidx, reinterpret_cast<LPARAM>(&tbb)))
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
                        ::SendMessage(m_hwnd, TB_SETSTATE, btn.cmdcur, (LPARAM)btn.state);
                    //
                    if ((btn.imgidx != -1) && (btn.imgidx != tbb.iBitmap))
                        ::SendMessage(m_hwnd, TB_CHANGEBITMAP, btn.cmdcur, btn.imgidx);
                    //
                    if ((btn.cmdnew != -1) && (btn.cmdnew != tbb.idCommand))
                        ::SendMessage(m_hwnd, TB_SETCMDID, btn.cmdcur, btn.cmdnew);
                    //
                    break;
                }
            }
        }
        while (0);
    }

    std::string ToolBar::getfind()
    {
        return m_findbar.getfind();
    }

    void ToolBar::event(int32_t id, bool isenable)
    {
        switch (id)
        {
            case IDM_BTN_SCRUN_START:
                {
                    changebutton(TB_BTN_CHECK,     -1, 0);
                    changebutton(TB_BTN_START,     -1, 0);
                    changebutton(TB_BTN_EXIT,      -1, 0);
                    changebutton(TB_BTN_STOP,      -1, TBSTATE_ENABLED);

                    m_menu.setenable(IDM_SCRUN_TEST,  false);
                    m_menu.setenable(IDM_SCRUN_START, false);
                    m_menu.setenable(IDM_MAIN_EXIT,   false);
                    m_menu.setenable(IDM_SCRUN_STOP,  true);

                    if (isenable)
                    {
                        changebutton(TB_BTN_MODE,      IDM_MAIN_DBGDUMP);
                        changebutton(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE, -1, 21);
                        //
                        m_menu.setcheck(IDM_MAIN_DBGSTEP,  true);
                        m_menu.setcheck(IDM_MAIN_DBGCYCLE, false);
                        //
                        m_menu.setenable(IDM_MAIN_DBGDUMP,  true);
                        m_menu.setenable(IDM_MAIN_DBGNEXT,  true);
                    }
                    else
                    {
                        m_menu.setcheck(IDM_MAIN_DBGSTEP,   false);
                        m_menu.setcheck(IDM_MAIN_DBGCYCLE,  true);
                        //
                        m_menu.setenable(IDM_MAIN_DBGDUMP,  false);
                        m_menu.setenable(IDM_MAIN_DBGNEXT,  false);
                    }
                    break;
                }
            case IDM_BTN_SCRUN_STOP:
                {
                    changebutton(TB_BTN_MODE,      IDM_MAIN_DBGCYCLE);
                    changebutton(TB_BTN_CHECK,     -1, TBSTATE_ENABLED);
                    changebutton(TB_BTN_START,     -1, TBSTATE_ENABLED);
                    changebutton(TB_BTN_EXIT,      -1, TBSTATE_ENABLED);
                    changebutton(TB_BTN_STOP,      -1, 0);
                    changebutton(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE);
                    //
                    m_menu.setenable(IDM_SCRUN_TEST,  true);
                    m_menu.setenable(IDM_SCRUN_START, true);
                    m_menu.setenable(IDM_MAIN_EXIT,   true);
                    m_menu.setenable(IDM_SCRUN_STOP,  false);
                    //
                    m_menu.setenable(IDM_MAIN_DBGDUMP,  false);
                    m_menu.setenable(IDM_MAIN_DBGNEXT,  false);
                    //
                    m_menu.setcheck(IDM_MAIN_DBGSTEP,  false);
                    m_menu.setcheck(IDM_MAIN_DBGCYCLE, true);

                    break;
                }
            case IDM_MAIN_DBGMODE:
                {
                    changebutton(TB_BTN_MODE,
                                ((isenable) ? IDM_MAIN_DBGSTEP : IDM_MAIN_DBGCYCLE)
                            );
                    m_menu.setcheck(IDM_MAIN_DBGSTEP,   isenable);
                    m_menu.setcheck(IDM_MAIN_DBGCYCLE, !isenable);
                    break;
                }
            case IDM_MAIN_DBGSTEP:
                {
                    changebutton(TB_BTN_MODE, IDM_MAIN_DBGSTEP);
                    m_menu.setcheck(IDM_MAIN_DBGSTEP,  true);
                    m_menu.setcheck(IDM_MAIN_DBGCYCLE, false);
                    break;
                }
            case IDM_MAIN_DBGCYCLE:
                {
                    changebutton(TB_BTN_MODE, IDM_MAIN_DBGCYCLE);
                    m_menu.setcheck(IDM_MAIN_DBGSTEP,  false);
                    m_menu.setcheck(IDM_MAIN_DBGCYCLE, true);
                    break;
                }

            case IDM_REBAR_FIND_TEXT:
                {
                    if (isenable)
                        changebutton(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE, -1, 21);
                    else
                        changebutton(TB_BTN_NAVIGATTE, IDM_EDIT_SHOW_NAVIGATE);
                    break;
                }
            case IDM_MAIN_WINTOP:
            case IDM_MAIN_EXTDBGV:
            case IDM_MAIN_DBGMUTE:
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
