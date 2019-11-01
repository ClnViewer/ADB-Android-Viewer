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

#include "../SCEditInternal.h"

namespace MDIWin
{
    // WND private

    LRESULT TitleBar::wndControlProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        switch (umsg_)
        {
            case WM_COMMAND:
                {
                    if (!m_handle[HT_PARENT])
                        break;

                    switch (LOWORD(wp_))
                    {
                        case (HT_DEFAULT_ID + HT_BTN_ID_PIN):
                            {
                                do
                                {
                                    TBBUTTON tbb{};
                                    if (!::SendMessage(
                                                m_handle[HT_TB],
                                                TB_GETBUTTON,
                                                0,
                                                reinterpret_cast<LPARAM>(&tbb)
                                        ))
                                        break;

                                    tbb.iBitmap = ((!tbb.iBitmap) ? 3 : 0);
                                    (void) ::SendMessage(
                                                m_handle[HT_TB],
                                                TB_CHANGEBITMAP,
                                                (WPARAM)LOWORD(wp_),
                                                (LPARAM)tbb.iBitmap
                                        );
                                }
                                while (0);
                                return SendMessage(m_handle[HT_PARENT], WM_COMMAND, (WPARAM)LOWORD(wp_), 0);
                            }
                        case (HT_DEFAULT_ID + HT_BTN_ID_DEFAULT):
                            {
                                return SendMessage(m_handle[HT_PARENT], WM_COMMAND, (WPARAM)LOWORD(wp_), 0);
                            }
                        case (HT_DEFAULT_ID + HT_BTN_ID_CLOSE):
                            {
                                return SendMessage(m_handle[HT_PARENT], WM_CLOSE, 0, 0);
                            }
                        default:
                            break;

                    }
                    break;
                }
            case WM_CTLCOLORSTATIC:
                {
                    ::SetTextColor((HDC)wp_, RGB(64, 64, 64));
                    ::SetBkMode((HDC)wp_, TRANSPARENT);
                    return (LRESULT)Config::instance().getbrush(Config::BRUSH_GRAY);
                }
            case WM_PAINT:
                {
                    RECT rc{};
                    PAINTSTRUCT ps{};
                    HDC hdc = ::BeginPaint(hwnd_, &ps);
                    ::GetClientRect(hwnd_, &rc);
                    ::FillRect(hdc, &rc, Config::instance().getbrush(Config::BRUSH_GRAY));
                    ::EndPaint(hwnd_, &ps);
                    return 0;
                }
            case WM_NCHITTEST:
                {
                    return HTTRANSPARENT;
                }
            case WM_ERASEBKGND:
                //return 0;
                break;
            default:
                break;
        }
        return ::DefMDIChildProc(hwnd_, umsg_, wp_, lp_);
    }

    ///

    LRESULT CALLBACK TitleBar::wndControlRedirect(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        if (umsg_ == WM_NCCREATE)
        {
            LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lp_);
            if (lpcs)
            {
                LPARAM lp = reinterpret_cast<LPARAM>(lpcs->lpCreateParams);
                ::SetWindowLongPtrA(hwnd_, GWLP_USERDATA, lp);
            }
        }
        MDIWin::TitleBar *tb =
            reinterpret_cast<MDIWin::TitleBar*>(::GetWindowLongPtrA(hwnd_, GWLP_USERDATA));
        if (tb)
            return tb->wndControlProc(hwnd_, umsg_, wp_, lp_);
        return ::DefMDIChildProc(hwnd_, umsg_, wp_, lp_);
    }

    ///

    TitleBar::TitleBar() {}
    TitleBar::~TitleBar()
    {
        for (uint32_t i = HandleType::HT_WINDOW; i < HandleType::HT_HWND_END; i++)
        {
            if (m_handle[i])
                ::DestroyWindow(m_handle[i]);
            m_handle[i] = nullptr;
        }
    }

    bool TitleBar::isenable()
    {
        return (m_handle[HT_WINDOW] != nullptr);
    }

    HWND TitleBar::getparent()
    {
        return m_handle[HT_PARENT];
    }

    void TitleBar::setposition(HandleType type, ImageLite::IRECT<int32_t> const & ir, uint32_t flag)
    {
        flag |= (SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
        ::SetWindowPos(
                m_handle[type],
                HWND_TOPMOST,
                ir.x, ir.y, ir.w, ir.h,
                flag
        );
    }

    bool TitleBar::create(HWND hparent, std::string const & s, uint32_t numbtn)
    {
        do
        {
            m_handle[HT_PARENT] = hparent;
            m_numbutton = ((numbtn > 3) ? 3 : numbtn);
            //
            if (Config::instance().getiml(Config::ImageButtonList::IMLBTN_TITLEBAR_CTRL, m_bsize) < m_numbutton)
                break;
            if (m_bsize.empty())
                break;
            //
            m_handle[HT_WINDOW] = ::CreateWindowExA(
                    WS_EX_COMPOSITED,
                    static_cast<LPCSTR>(s.c_str()),
                    nullptr,
                    WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER,
                    0, 0, 1, 1,
                    m_handle[HT_PARENT],
                    nullptr,
                    Config::instance().gethinstance(),
                    static_cast<void*>(this)
                );

            if (!m_handle[HT_WINDOW])
                break;

            long style  = ::GetWindowLong(m_handle[HT_WINDOW], GWL_STYLE);
                 style &= ~(WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_SYSMENU);
            ::SetWindowLong(m_handle[HT_WINDOW], GWL_STYLE, style);
                 style  = ::GetWindowLong(m_handle[HT_WINDOW], GWL_EXSTYLE);
                 style &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
            ::SetWindowLong(m_handle[HT_WINDOW], GWL_EXSTYLE, style);

            m_wsize = {};
            setposition(HT_WINDOW, m_wsize, SWP_NOSIZE | SWP_NOMOVE);
            //
            m_handle[HT_TITLE] = ::CreateWindow(
                    "STATIC",
                    "..",
                    WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE |
                    ES_LEFT | SS_PATHELLIPSIS,
                    0, 2, 1 , 1,
                    m_handle[HT_WINDOW],
                    (HMENU)(HT_DEFAULT_ID + HT_TITLE),
                    Config::instance().gethinstance(),
                    nullptr
                );
            if (!m_handle[HT_TITLE])
                break;

            ::SendMessage(m_handle[HT_WINDOW], WM_SETFONT, (LPARAM)Config::instance().getfont(), 1);
            ::SendMessage(m_handle[HT_TITLE],  WM_SETFONT, (LPARAM)Config::instance().getfont(), 1);

            if (m_numbutton)
                return createbtn();
            return true;
        }
        while (0);
        return false;
    }

    bool TitleBar::createbtn()
    {
        do
        {
            m_handle[HT_TB] = CreateWindowExA(
                    WS_EX_COMPOSITED,
                    TOOLBARCLASSNAME,
                    (LPSTR)nullptr,
                    TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_TRANSPARENT |
                    WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                    CCS_NORESIZE | CCS_NODIVIDER,
                    0, 0, ((m_numbutton * m_bsize.x) + 2), (m_bsize.y + 2),
                    m_handle[HT_WINDOW],
                    nullptr,
                    Config::instance().gethinstance(),
                    nullptr
                );
            if (!m_handle[HT_TB])
                break;

            //
            ::SendMessage(m_handle[HT_TB], TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
            ::SendMessage(m_handle[HT_TB], TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
            ::SendMessage(m_handle[HT_TB], TB_SETIMAGELIST, (WPARAM)0, (LPARAM)
                          Config::instance().getiml(Config::ImageButtonList::IMLBTN_TITLEBAR_CTRL)
                    );

            int32_t idx = 0;
            TBBUTTON tbbtn[3]{};
            //
            tbbtn[0].iBitmap = idx;
            tbbtn[0].idCommand = (HT_DEFAULT_ID + HT_BTN_ID_PIN);
            tbbtn[0].fsState = TBSTATE_ENABLED;
            tbbtn[0].fsStyle = TBSTYLE_BUTTON;
            //
            tbbtn[1].iBitmap = idx + 1;
            tbbtn[1].idCommand = (HT_DEFAULT_ID + HT_BTN_ID_DEFAULT);
            tbbtn[1].fsState = TBSTATE_ENABLED;
            tbbtn[1].fsStyle = TBSTYLE_BUTTON;
            //
            tbbtn[2].iBitmap = idx + 2;
            tbbtn[2].idCommand = (HT_DEFAULT_ID + HT_BTN_ID_CLOSE);
            tbbtn[2].fsState = TBSTATE_ENABLED;
            tbbtn[2].fsStyle = TBSTYLE_BUTTON;

            ::SendMessage(m_handle[HT_TB], TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
            ::SendMessage(m_handle[HT_TB], TB_ADDBUTTONS, m_numbutton, (LPARAM)&tbbtn);
            ::SendMessage(m_handle[HT_TB], TB_SETBUTTONSIZE, 0, MAKELPARAM(m_bsize.x, m_bsize.y));
            ::SendMessage(m_handle[HT_TB], TB_SETPADDING, 0, MAKELPARAM(0, 0));
            ::SendMessage(m_handle[HT_TB], TB_AUTOSIZE, 0, 0);
            //
            COLORSCHEME scheme;
            scheme.dwSize          = sizeof(scheme);
            scheme.clrBtnHighlight = RGB(255,255,255);
            scheme.clrBtnShadow    = RGB(255,255,255);
            ::SendMessage(m_handle[HT_TB], TB_SETCOLORSCHEME, 0, (LPARAM)&scheme);
            //
            ::SendMessage(m_handle[HT_TB], WM_SETFONT, (LPARAM)Config::instance().getfont(), 1);

            return true;
        }
        while (0);
        return false;
    }

    std::string TitleBar::gettitle()
    {
        do
        {
            std::vector<char> vs(1024);
            if (::GetWindowTextA(
                    m_handle[HT_TITLE],
                    &vs[0],
                    1024) <= 0
                )
                break;
            if (!vs.size())
                break;
            std::string s(vs.begin(), vs.end());
            return s;
        }
        while (0);
        return std::string();
    }

    void TitleBar::settitle(std::string const & s)
    {
        if (!m_handle[HT_TITLE])
            return;
        if (s.empty())
            ::SetWindowTextA(m_handle[HT_TITLE], "..");
        else
            ::SetWindowTextA(m_handle[HT_TITLE], static_cast<LPCSTR>(s.c_str()));
    }

    ImageLite::IRECT<int32_t> TitleBar::getsize()
    {
        return m_wsize;
    }

    void TitleBar::setsize(ImageLite::IRECT<int32_t> const & ir)
    {
        if (!m_handle[HT_WINDOW])
            return;

        m_wsize.set<int32_t>(ir.w, (m_bsize.x + 8));
        setposition(HT_WINDOW, m_wsize, SWP_NOMOVE);
        HDC hdc = GetDC(m_handle[HT_WINDOW]);
        if (hdc)
        {
            setsize(hdc);
            ::ReleaseDC(m_handle[HT_WINDOW], hdc);
        }
        ::ShowWindow(m_handle[HT_WINDOW], SW_SHOWNOACTIVATE);
    }

    void TitleBar::setsize(HDC hdc)
    {
        do
        {
            if (!m_handle[HT_WINDOW])
                break;

            RECT rc{};
            if (!::GetClientRect(m_handle[HT_WINDOW], &rc))
                break;
            if ((rc.right <= 1) || (rc.bottom <= 1))
                break;

            m_wsize.convert(rc);
            if (m_wsize.empty())
                break;

            if (!m_handle[HT_TITLE])
                break;

            int32_t tbpad = 0;
            ImageLite::IRECT<int32_t> ir{};
            //
            do
            {
                if (!m_handle[HT_TB])
                    break;

                int32_t tbsz  = ((m_bsize.x * m_numbutton) + 2);
                        tbpad = (tbsz + 10);
                //
                ir.set<int32_t>((m_wsize.w - tbpad), 2, tbsz, (m_bsize.y + 2));
                setposition(HT_TB, ir);
            }
            while (0);
            //
            do
            {
                std::string s = gettitle();
                if (s.empty())
                    break;

                SIZE sz{};
                if (!::GetTextExtentPoint32A(
                        hdc,
                        static_cast<LPCSTR>(s.c_str()),
                        s.length(),
                        &sz
                    ))
                    break;

                ir.set<LONG>(sz.cx, sz.cy);
                ir.x = 10;
                ir.y = 2;
                ir.h = 24;

                if (m_wsize.w < (ir.w + 10 + 10 + tbpad))
                {
                    int32_t pad = (m_wsize.w - 10 - 10 - tbpad);
                            pad = ((pad > 0) ? pad : 0);
                    ir.w = ((ir.w > pad) ? pad : ir.w);
                }
                setposition(HT_TITLE, ir);
            }
            while (0);
        }
        while (0);
    }
};

