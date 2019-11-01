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

    /// static

    static HWND f_create(
        HWND      hparent,
        ImageLite::IRECT<int32_t> const & ir,
        std::string const & s1,
        std::string const & s2 = std::string(),
        int32_t flag           = 0,
        int32_t flagex         = WS_EX_COMPOSITED,
        HMENU hmenu            = nullptr,
        void *obj              = nullptr)
    {
            return ::CreateWindowExA(
                    flagex,
                    static_cast<LPCSTR>(s1.c_str()),
                    static_cast<LPCSTR>(s2.c_str()),
                    flag,
                    ir.x, ir.y, ir.w, ir.h,
                    hparent,
                    hmenu,
                    Config::instance().gethinstance(),
                    static_cast<LPVOID>(obj)
                );
    }

    /// end static

    /// BaseData class

    HICON BaseData::default_icon = nullptr; // cache regclass icon

    bool BaseData::regclass(std::string const & s, wndWrap fun, HBRUSH brush, HICON hicon, int32_t imenu)
    {
        WNDCLASSA wndc{};
        LPCSTR cname = static_cast<LPCSTR>(s.c_str());
        if (::GetClassInfoA(Config::instance().gethinstance(), cname, &wndc))
            return true;

        wndc.lpszClassName = cname;
        wndc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wndc.lpfnWndProc   = fun;
        wndc.hInstance     = Config::instance().gethinstance();
        wndc.hCursor       = ::LoadCursor(0, IDC_ARROW);

        if (imenu)
            wndc.lpszMenuName = MAKEINTRESOURCE(imenu);

        if (hicon)
            wndc.hIcon = hicon;
        else
        {
            if (!BaseData::default_icon)
                BaseData::default_icon = ::LoadIconA(
                                Config::instance().gethinstance(),
                                MAKEINTRESOURCE(ID_MAIN_ICON)
                        );
            wndc.hIcon = BaseData::default_icon;
        }

        if (brush)
            wndc.hbrBackground = brush;

        if (!::RegisterClassA(&wndc))
            return false;
        return true;
    }

    /// end BaseData class

    Base::Base() {}
    Base::~Base()
    {
        close();
    }

    void Base::close(bool b)
    {
        if (!m_data.window)
            return;

        if (b)
        {
            if (::IsWindow(m_data.window))
                ::ShowWindow(m_data.window, SW_HIDE);
            ::DestroyWindow(m_data.window);
        }
        m_data.window = nullptr;
    }

    void Base::open()
    {
        if (!m_data.window)
            return;
        if (::IsWindow(m_data.window))
            ::ShowWindow(m_data.window, SW_RESTORE);
    }

    ImageLite::IRECT<int32_t> Base::getsizedefault()
    {
        return m_data.irdefault;
    }
    ImageLite::IRECT<int32_t> Base::getsize()
    {
        return m_data.irsize;
    }
    uint32_t Base::getid()
    {
        return m_data.wid;
    }
    BaseData::MDIWinType Base::getgroup()
    {
        return m_data.wgroup;
    }
    BaseData::MDIWinType Base::gettype()
    {
        return m_data.wtype;
    }
    HWND Base::gethandle()
    {
        return m_data.window;
    }
    void Base::sethandle(HWND hw)
    {
        m_data.wparent   = hw;
    }
    bool Base::isvisable()
    {
        return m_data.isvisable;
    }
    bool Base::istitlebar()
    {
        switch (m_data.wstyle)
        {
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:
            case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE:
                return m_titlebar.isenable();
            default:
                return false;
        }
    }
    void Base::setvisable(bool b)
    {
        m_data.isvisable = b;
    }
    void Base::settitle(std::string const & s)
    {
        m_titlebar.settitle(s);
    }
    std::string Base::gettitle()
    {
        return m_titlebar.gettitle();
    }
    bool Base::ispined()
    {
        return m_data.ispined;
    }
    void Base::setpined(bool b)
    {
        m_data.ispined = b;
    }

    bool Base::init(HWND hw_, uint32_t id_, std::string const & s_, std::string const & c_, void *v_)
    {
        try
        {
            do
            {
                m_data.wparent    = hw_;
                m_data.wclass     = s_;
                m_data.wclassctrl = c_;
                m_data.wdata      = v_;
                m_data.wid        = id_;
                //
                {
                    auto [ iover, iret ] = event_initBegin();
                    if ((!iover) && (!create()))
                        break;
                    else if ((iover) && (!iret))
                        break;
                }
                if (!m_data.window)
                    break;

                int32_t sid;
                switch (m_data.wstyle)
                {
                    case BaseData::MDIWinStyle::MWSTYLE_NONE:
                    case BaseData::MDIWinStyle::MWSTYLE_SYS_RESIZE:
                    case BaseData::MDIWinStyle::MWSTYLE_SYS_CLEAR:
                    case BaseData::MDIWinStyle::MWSTYLE_SYS_CAPTION:
                    case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:        sid = -1; break;
                    case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION:       sid =  0; break;
                    case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_MOVE:  sid =  2; break;
                    case BaseData::MDIWinStyle::MWSTYLE_MDI_CAPTION_CLOSE: sid =  3; break;
                    default:                                               sid =  0; break;
                }
                if (sid != -1)
                {
                    if (!m_titlebar.create(
                            m_data.window,
                            m_data.wclassctrl,
                            static_cast<uint32_t>(sid)
                        ))
                        break;
                    m_titlebar.settitle(m_data.wtitle);
                }

                ::SetWindowLongPtr(
                        m_data.window,
                        GWLP_USERDATA,
                        (LONG_PTR)m_data.wid
                    );
                //
                {
                    auto [ iover, iret ] = event_initEnd();
                    if (iover)
                        return iret;
                }
                //
                return true;
            }
            while (0);
            return false;
        }
        catch (...) { return false; }
    }

    // reset size
    void Base::setsizedefault()
    {
        m_data.irsize = {};
    }

    // initial default window size
    void Base::setsizedefault(ImageLite::IRECT<int32_t> const & ir)
    {
        if (event_sizeinitBegin(ir))
            return;
        if (m_data.irdefault.empty())
            return;

        double pw = (ir.w / 100.0),
               ph = (ir.h / 100.0);

        m_data.irsize.set<int32_t>(
                std::abs(m_data.irdefault.x * pw) + ir.x,
                std::abs(m_data.irdefault.y * ph) + ir.y,
                std::abs(m_data.irdefault.w * pw),
                std::abs(m_data.irdefault.h * ph)
            );
        //
        if (m_data.irsize.empty())
            return;
        if (!event_sizeinitEnd())
            return;
        if (istitlebar())
            m_titlebar.setsize(m_data.irsize);
        if (m_data.window)
            resize();
    }
    // window - MDI child set size & resize
    void Base::setsize(ImageLite::IRECT<int32_t> const & irc, ImageLite::IRECT<int32_t> const & ire)
    {
        if ((!m_data.window) || (ispined()))
            return;

        event_size(irc, ire);

        if (istitlebar())
            m_titlebar.setsize(m_data.irsize);
    }
    // move window - data only resize
    void Base::setsize(ImageLite::IPOINT<int32_t> const & ip)
    {
        if ((!ip.x) || (!ip.y))
            return;

        int32_t w = ::GetSystemMetrics(SM_CXSCREEN);
        m_data.irsize.x = ((ip.x > 10) ? ((ip.x < w) ? ip.x : 0)  : 0);
        m_data.irsize.y = (((ip.y - m_data.irsize.y) < 2) ? m_data.irsize.y : ip.y);

        refresh();
        if (istitlebar())
            m_titlebar.setsize(m_data.irsize);
    }
    // resize window - data only resize
    void Base::setsize()
    {
        if (!m_data.window)
            return;

        if (event_size())
            return;

        RECT rc{};
        if (!::GetClientRect(m_data.window, &rc))
            return;

        ImageLite::IRECT<int32_t> ir;
        ir.convert(rc);
        if (ir.empty())
            return;

        switch (m_data.wstyle)
        {
            case BaseData::MDIWinStyle::MWSTYLE_MDI_RESIZE:
                {
                    ir.w += BaseData::stylewinpad(m_data.wstyle, BaseData::MDIWinPad::MWPAD_MDI_FX);
                    ir.h += BaseData::stylewinpad(m_data.wstyle, BaseData::MDIWinPad::MWPAD_MDI_FY);
                    break;
                }
            default:
                break;
        }
        if (!m_data.irsize.compare(ir))
            m_data.irsize.set(ir);

        event_resize();
    }

    bool Base::create()
    {
        ImageLite::IRECT<int32_t> ir = { 0, 0, 1, 1 };
        return create(ir);
    }
    bool Base::create(ImageLite::IRECT<int32_t> const & ir)
    {
        switch (m_data.wgroup)
        {
            case BaseData::MDIWinType::MWTYPE_MAIN:
                {
                    m_data.window = f_create(
                                m_data.wparent,
                                ir,
                                m_data.wclass,
                                m_data.wtitle,
                                m_data.wflags,
                                m_data.wflagex,
                                m_data.wmenu,
                                m_data.wdata
                            );
                    break;
                }
            case BaseData::MDIWinType::MWTYPE_CLIENT:
                {
                    CLIENTCREATESTRUCT ccs{};
                    ccs.hWindowMenu  = ::GetSubMenu(::GetMenu(m_data.wparent), 1);
                    ccs.idFirstChild = static_cast<uint32_t>(BaseData::MDIWinType::MWTYPE_CHILD);
                    m_data.window = f_create(
                                m_data.wparent,
                                ir,
                                "MDICLIENT",
                                std::string(),
                                m_data.wflags,
                                m_data.wflagex,
                                nullptr,
                                static_cast<void*>(&ccs)
                        );
                    break;
                }
            case BaseData::MDIWinType::MWTYPE_CHILD:
            case BaseData::MDIWinType::MWTYPE_CUSTOM:
                {
                    if (!m_data.wparent)
                        return false;

                    MDICREATESTRUCTA mcs{};
                    mcs.szClass = m_data.wclass.c_str();
                    mcs.szTitle = m_data.wtitle.c_str();
                    mcs.hOwner  = Config::instance().gethinstance();
                    mcs.x       = ir.x;
                    mcs.y       = ir.y;
                    mcs.cx      = ir.w;
                    mcs.cy      = ir.h;
                    mcs.style   = m_data.wflags;
                    m_data.irsize = {};

                    m_data.window = reinterpret_cast<HWND>(
                                ::SendMessage(
                                    m_data.wparent, WM_MDICREATE, 0, reinterpret_cast<LPARAM>(&mcs)
                                )
                            );
                    break;
                }
            default:
                return false;
        }
        if (!m_data.window)
            return false;

        //stylepad(m_data.wstyle, false);
        BaseData::setwinstyle(m_data.window, m_data.wstyle);
        return true;
    }
};

