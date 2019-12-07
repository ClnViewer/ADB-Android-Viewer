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
#include <richedit.h>

namespace Editor
{
    //
    static inline const wchar_t *l_items[] =
    {
        L"Help",
        L"API",
        L"Monitor"
    };
    //
    /* OLE DRAG & DROP event from ListBox (MONITOR), DROP ID is registered MDIWin::MDIApp::m_dragid */
    LRESULT CALLBACK ToolBox::TabWndProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, UINT_PTR, DWORD_PTR ptr)
    {
        if ((umsg_ == MDIWin::MDIApp::m_dragid) &&
            (wp_ == (ID_TAB_IDX + static_cast<uint32_t>(TabIndex::ITAB_MONITOR))))
        {
            std::string s;
            Editor::ToolBox *tb = reinterpret_cast<Editor::ToolBox*>(ptr);
            if (tb)
                tb->event(
                    tb->getchild(ToolBox::TabIndex::ITAB_MONITOR),
                    lp_,
                    s
                );
        }
        return ::DefSubclassProc(hwnd_, umsg_, wp_, lp_);
    }
    //

    ToolBox::ToolBox()
    {
        Base::m_data = data();
    }

    ToolBox::~ToolBox()
    {
        for (uint32_t i = 0; i < __NELE(m_tab); i++)
        {
            if (m_tab[i])
                ::DestroyWindow(m_tab[i]);
            m_tab[i] = nullptr;
        }
        if (m_hwnd)
        {
            ::RemoveWindowSubclass(m_hwnd, &ToolBox::TabWndProc, 0);
            ::DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
    }

    MDIWin::WinData ToolBox::data()
    {
        MDIWin::WinData d(
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD,       // MDIWinType group
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_TOOLS, // MDIWinType type
                    MDIWin::BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE,  // MDIWinStyle
                    std::string(),                                    // Class name
                    "Tool box",                                       // Title
                    (WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS),
                    WS_EX_COMPOSITED
                );
        d.irdefault.set<int32_t>(72, 0, 28, 70);                      // % from main widow
        return d;
    }

    HWND ToolBox::getchild(TabIndex t)
    {
        return m_tab[t];
    }

    bool ToolBox::tabforeach_(uint32_t id, std::function<bool(HWND*, uint32_t, uint32_t)> fun)
    {
        for (uint32_t i = 0; i < __NELE(m_tab); i++)
            if (!fun(&m_tab[i], id, i))
                return false;
        return true;
    }

    void ToolBox::sizeinit_()
    {
        do
        {
            if (!m_hwnd)
                break;

            ImageLite::IRECT<int32_t> ir = m_titlebar.getsize();
            ir.x = 1;
            ir.y = ir.h;
            ir.w = (Base::m_data.irsize.w - 2);
            ir.h = (Base::m_data.irsize.h - ir.h);

            if (ir.w >= 220)
                (void) ::SetWindowPos(
                    m_hwnd,
                    0,
                    ir.x,
                    ir.y,
                    ir.w,
                    ir.h,
                    SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_SHOWWINDOW
                );

            ir.y += 2;
            ir.h -= 30;

            (void) tabforeach_(
                static_cast<uint32_t>(::SendMessage(m_hwnd, TCM_GETCURSEL, 0, 0)),
                [&](HWND *hwnd, uint32_t id, uint32_t n)
                {
                    if (!*hwnd)
                        return true;

                    bool     b      = ((m_irchild[n].w == ir.w) && (m_irchild[n].h == ir.h));
                    uint32_t flags  = ((n == id) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
                    if (b)
                             flags |= (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
                    else
                             flags |= (SWP_FRAMECHANGED | SWP_NOACTIVATE);

                    (void) ::SetWindowPos(
                                *hwnd,
                                0,
                                ir.x,
                                ir.y,
                                ir.w,
                                ir.h,
                                flags
                        );

                    if (!b)
                    {
                        RECT rc{};
                        if (::GetWindowRect(*hwnd, &rc))
                            m_irchild[n].convert(rc);
                    }
                    return true;
                }
            );
        }
        while (0);
    }

    void ToolBox::event_resize()
    {
        sizeinit_();
    }

    void ToolBox::event_size(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const & ire)
    {
        int32_t pad = MDIWin::BaseData::stylewinpad(m_data.wstyle, MDIWin::BaseData::MDIWinPad::MWPAD_MDI_BX),
                w = ((irc.w / 100.0) * Base::m_data.irdefault.w);

        w = (((irc.w - ire.w) > 4) ?
             (irc.w - ire.w) : w
        );
        Base::m_data.irsize.set<int32_t>(
                (irc.w - w),
                irc.y,
                w,
                (ire.h - pad)
            );
        if (Base::m_data.irsize.empty())
            return;

        Base::m_titlebar.setsize(Base::m_data.irsize);
        Base::resize();
        sizeinit_();
    }

    std::tuple<bool, bool> ToolBox::event_initEnd()
    {
        do
        {
            if (!Base::m_data.window)
                break;

            m_hwnd = ::CreateWindowExW(
                            WS_EX_COMPOSITED,
                            WC_TABCONTROLW, L"",
                            WS_VISIBLE      |
                            WS_TABSTOP      |
                            WS_CHILD        |
                            WS_CLIPSIBLINGS |
                            WS_CLIPCHILDREN |
                            TCS_BUTTONS     |
                            TCS_FLATBUTTONS,
                            5,
                            26, // Base::m_titlebar.getsize().h,
                            1,  // Base::m_data.irsize.w,
                            1,  // Base::m_data.irsize.h,
                            Base::m_data.window,
                            (HMENU)ID_TAB_CTRL,
                            MDIWin::Config::instance().gethinstance(),
                            nullptr
                    );
            if (!m_hwnd)
                break;

            TCITEMW tie{};
            tie.mask = TCIF_TEXT;
            tie.iImage = -1;

            for (uint32_t i = 0U; i < __NELE(l_items); i ++)
            {
                tie.pszText = const_cast<LPWSTR>(l_items[i]);
                ::SendMessageW(m_hwnd, TCM_INSERTITEMW, (WPARAM)i, reinterpret_cast<LPARAM>(&tie));
            }
            //
            ::SendMessage(m_hwnd, WM_SETFONT, (WPARAM)MDIWin::Config::instance().getfont(), (LPARAM)1);
            ::SetWindowSubclass(
                    m_hwnd,
                    &ToolBox::TabWndProc,
                    0,
                    reinterpret_cast<DWORD_PTR>(this)
                );
            //
            bool b = setup_();
            return { true, b };
        }
        while (0);
        return { true, false };
    }

    /* OLE DRAG & DROP from TreeView (API) & ListBox (MONITOR) */
    bool ToolBox::event(HWND hwnd_, LPARAM lp, std::string & s)
    {
        (void) tabforeach_(0,
                [&](HWND *hwnd, uint32_t, uint32_t n)
                {
                    if (*hwnd != hwnd_)
                        return true;

                    switch (n)
                    {
                        case TabIndex::ITAB_API:
                            {
                                (void) event_api(*hwnd, lp, s);
                                break;
                            }
                        case TabIndex::ITAB_MONITOR:
                            {
                                (void) event_monitor(*hwnd, lp, s);
                                break;
                            }
                        default:
                            break;
                    }
                    return true;
                }
            );
        return true;
    }

    bool ToolBox::setup_()
    {
        return tabforeach_(0,
                [&](HWND *hwnd, uint32_t, uint32_t n)
                {
                    switch (n)
                    {
                        case TabIndex::ITAB_HELP:
                            {
                                *hwnd = setup_help_();
                                break;
                            }
                        case TabIndex::ITAB_API:
                            {
                                *hwnd = setup_api_();
                                break;
                            }
                        case TabIndex::ITAB_MONITOR:
                            {
                                *hwnd = setup_monitor_();
                                break;
                            }
                        default:
                            return false;
                    }
                    if (!*hwnd)
                        return false;
                    //
                    ::ShowWindow(*hwnd, ((!n) ? SW_SHOWNORMAL : SW_HIDE));
                    return true;
                }
            );
    }

    void ToolBox::show_(uint32_t idx)
    {
        (void) tabforeach_(idx,
                [&](HWND *hwnd, uint32_t id, uint32_t n)
                {
                    if (!*hwnd)
                        return true;
                    ::ShowWindow(*hwnd, ((n == id) ? SW_SHOWNORMAL : SW_HIDE));
                    return true;
                }
            );
    }

    void ToolBox::show()
    {
        if (!m_hwnd)
            return;
        show_(
             static_cast<uint32_t>(::SendMessage(m_hwnd, TCM_GETCURSEL, 0, 0))
        );
    }

    void ToolBox::show(uint32_t idx)
    {
        if (!m_hwnd)
            return;
        if (idx > static_cast<uint32_t>(TabIndex::ITAB_MONITOR))
            return;
        //
        (void)::SendMessage(m_hwnd, TCM_SETCURSEL, (WPARAM)idx, 0);
        show_(idx);
    }

    void ToolBox::show(std::vector<std::string> const & v)
    {
        uint32_t idx = static_cast<uint32_t>(TabIndex::ITAB_MONITOR);
        //
        if ((!m_hwnd) || (!m_tab[idx]))
            return;
        //
        add_monitor_(m_tab[idx], v);
        (void)::SendMessage(
                    m_hwnd,
                    TCM_SETCURSEL,
                    (WPARAM)idx,
                    0
            );
        show_(idx);
    }

};
