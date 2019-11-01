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
    static uint32_t const style_flags[][4] =
    {
        // MWSTYLE_MDI_CAPTION_MOVE
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME),
            0,
            (WS_EX_WINDOWEDGE | WS_EX_MDICHILD),
            WS_EX_TOOLWINDOW
        },
        // MWSTYLE_MDI_CAPTION_CLOSE
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME),
            0,
            (WS_EX_WINDOWEDGE | WS_EX_MDICHILD),
            WS_EX_TOOLWINDOW
        },
        // MWSTYLE_MDI_CAPTION
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME),
            0,
            (WS_EX_WINDOWEDGE | WS_EX_MDICHILD),
            WS_EX_TOOLWINDOW
        },
        // MWSTYLE_MDI_RESIZE
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU),
            WS_THICKFRAME,
            (WS_EX_WINDOWEDGE | WS_EX_MDICHILD),
            WS_EX_TOOLWINDOW
        },
        // MWSTYLE_SYS_RESIZE
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU),
             WS_THICKFRAME,
            (WS_EX_WINDOWEDGE | WS_EX_MDICHILD),
            WS_EX_TOOLWINDOW
        },
        // MWSTYLE_SYS_CAPTION
        {
            (WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
            WS_THICKFRAME,
            0, 0
        },
        // MWSTYLE_SYS_CLEAR
        {
            (WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME),
            0,
            (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE),
            0
        },
        // MWSTYLE_NONE
        {
            0, 0, 0, 0
        }
    };

    static std::array<uint32_t, 4> f_getstylebytype(BaseData::MDIWinStyle t)
    {
        switch (t)
        {
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_SYS_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_SYS_CAPTION:
            case BaseData::MDIWinStyle::MWSTYLE_SYS_CLEAR:  break;
            default:
                t = BaseData::MDIWinStyle::MWSTYLE_NONE; break;
        }
        return
        {
            style_flags[t][0],
            style_flags[t][1],
            style_flags[t][2],
            style_flags[t][3]
        };
    }
    static void f_setstyle(HWND hwnd, int32_t r, int32_t a, int32_t f)
    {
        if ((r) || (a))
        {
            long style = ::GetWindowLong(hwnd, f);
            if (r)
                style &= ~r;
            if (a)
                style |= a;
            ::SetWindowLong(hwnd, f, style);
        }
    }
    static void f_setsize(HWND hwnd, ImageLite::IRECT<int32_t> const & ir, uint32_t flags = 0)
    {
        if ((!ir.w) || (!ir.h) || (!hwnd))
            return;

        flags |= (SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        (void) ::SetWindowPos(
                hwnd,
                0,
                ir.x, ir.y, ir.w, ir.h,
                flags
            );
    }

    /// BaseData class

    void BaseData::setwinstyle(HWND hwnd, BaseData::MDIWinStyle t)
    {
        if (!hwnd)
            return;

        ImageLite::IRECT<int32_t> ir{};
        std::array<uint32_t, 4> arr = f_getstylebytype(t);

        f_setstyle(hwnd, arr.at(0), arr.at(1), GWL_STYLE);
        f_setstyle(hwnd, arr.at(2), arr.at(3), GWL_EXSTYLE);
        f_setsize (hwnd, ir, SWP_NOMOVE | SWP_NOSIZE);
    }

    int32_t BaseData::stylewinpad(MDIWinStyle t, MDIWinPad p)
    {
        switch (t)
        {
            case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE: break;
            default:
                return 0;
        }
        switch (p)
        {
            case BaseData::MDIWinPad::MWPAD_MDI_FX:
            case BaseData::MDIWinPad::MWPAD_MDI_FY:
                {
                    return (::GetSystemMetrics(
                                    ((p == MDIWinPad::MWPAD_MDI_FX) ? SM_CXSIZEFRAME : SM_CYSIZEFRAME)
                                ) * 2);
                }
            case BaseData::MDIWinPad::MWPAD_MDI_BX:
            case BaseData::MDIWinPad::MWPAD_MDI_BY:
                {
                    return (::GetSystemMetrics(
                                    ((p == MDIWinPad::MWPAD_MDI_BX) ? SM_CXBORDER : SM_CYBORDER)
                                ) * 2);
                }
            case BaseData::MDIWinPad::MWPAD_MDI_FBX:
            case BaseData::MDIWinPad::MWPAD_MDI_FBY:
                {
                    return ((::GetSystemMetrics(
                                    ((p == MDIWinPad::MWPAD_MDI_FBX) ? SM_CXSIZEFRAME : SM_CYSIZEFRAME)
                                ) +
                            ::GetSystemMetrics(
                                    ((p == MDIWinPad::MWPAD_MDI_FBX) ? SM_CXBORDER : SM_CYBORDER)
                                )) * 2);
                }
            default:
                return 0;
        }
    }

    /// end BaseData class

    void Base::refresh()
    {
        f_setsize(m_data.window, m_data.irsize, SWP_NOMOVE | SWP_NOSIZE);
    }
    void Base::refresh(uint32_t flags)
    {
        f_setsize(m_data.window, m_data.irsize, flags);
    }
    void Base::refresh(HWND hwnd)
    {
        ImageLite::IRECT<int32_t> ir{};
        f_setsize(hwnd, ir, SWP_NOMOVE | SWP_NOSIZE);
    }
    void Base::resize()
    {
        f_setsize(m_data.window, m_data.irsize, 0);
    }

    void Base::stylepad(BaseData::MDIWinStyle t, bool isrefresh)
    {
        switch (t)
        {
            case BaseData::MDIWinStyle::MWSTYLE_SYS_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:
                {
                    int32_t pad      = ::GetSystemMetrics(92);
                    pad              = ((pad) ? (pad * 2) : 4);
                    pad             += ::GetSystemMetrics(SM_CXHSCROLL);
                    m_data.irsize.x  = 0;
                    m_data.irsize.y  = 0;
                    m_data.irsize.h -= pad;
                    break;
                }
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE:
                {
                    auto tb = Base::m_titlebar.getsize();
                    if (tb.h <= 0)
                        break;
                    m_data.irsize.x  = 0;
                    m_data.irsize.y  = tb.h;
                    m_data.irsize.h -= tb.h;
                    break;
                }
            default:
                break;
        }
        switch (t)
        {
            case BaseData::MDIWinStyle::MWSTYLE_SYS_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE:
                {
                    if (isrefresh)
                        f_setsize(m_data.window, m_data.irsize, 0);
                    break;
                }
            default:
                break;
        }
    }
};

