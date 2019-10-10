
#if !defined(_UNICODE)
#  define _UNICODE 1
#endif

#if !defined(UNICODE)
#  define UNICODE 1
#endif

#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>

#define _BUILD_IMAGELITE_DRAW 1
#include "ImageLite.h"

#define BORDER_PADDING        20
#define CATCH_ERR() \
    f_getCatchUnknownError(__FILE__, __FUNCTION__, __LINE__).c_str()

#define IDM_ONTOP        65000
#define IDM_SAVE_IMAGE_0 65001
#define IDM_SAVE_IMAGE_1 65002
#define IDM_SAVE_IMAGE_2 65003
#define IDM_SAVE_REGION  65004
#define IDM_SCALE_11     65005
#define IDM_SCALE_12     65006
#define IDM_SCALE_13     65007

#define ID_STATUS_BAR    65101
#define ID_VIEWER_ICON   65102

namespace ImageLite
{
    enum MenuSwitch
    {
        MENU_DISABLED = 1,
        MENU_ENABLED,
        MENU_CHECKED,
        MENU_UNCHECKED
    };
    static inline const wchar_t WSTR_VIEWER_CNAME[] = L"ImageViewerWindow";
    static inline const char STR_VIEWER_MBOX_NAME[] =  "Image Viewer error";
    static inline const char STR_FILE_DEFEXT[]      =  "png;jpg;bmp";
    static inline const char STR_FILE_INITDIR[]     =  ".\\";
    static inline const char STR_FILE_FILTER[]      =  "PNG files (*.png)\0*.png"       \
                                                       "\0BMP files (*.bmp)\0*.bmp"     \
                                                       "\0Lua image table (*.lua)\0*.lua\0";
    static inline const wchar_t WSTR_SAVE_IMAGE0[]  = L"Save image";
    static inline const wchar_t WSTR_SAVE_IMAGE1[]  = L"Save image top";
    static inline const wchar_t WSTR_SAVE_IMAGE2[]  = L"Save image bottom";
    static inline const wchar_t WSTR_SAVE_REGION[]  = L"Save selected region";
    static inline const wchar_t WSTR_SCALE[]        = L"Scale";
    static inline const wchar_t WSTR_ONTOP[]        = L"Always on top";

    static inline const char STR_SELECT_CANCEL[]    =  "cancel selection..";
    static inline const char STR_SELECT_SCALEWARN[] =  "set scale 1:1 to save selected region..";
    static inline const char STR_SAVE_OK[]          =  "image saved..";
    static inline const char STR_SAVE_REGION[]      =  "image saved, Lua code copied to clipboard..";
    static inline const char STR_LUA_CODE[]         =  "-- lua plugin function compare\n" \
                                                       "local x = LuaObject:imageCompareRegion";

    static std::string f_getCatchUnknownError(
            std::string const & file_, std::string const & fun_, int32_t line_
        )
    {
        std::string f = file_.c_str();
        std::size_t pos = f.find_last_of("/\\");
        if (pos != std::string::npos)
            f = f.substr(pos + 1, f.length());
        std::stringstream ss;
        ss << "Unknown error -> " << f.c_str() << ":" << line_ << ":" << fun_.c_str();
        return ss.str();
    }

    static void f_SetupFileDialog(HWND hwnd, OPENFILENAMEA *sfn_, std::string & s_)
    {
        sfn_->lStructSize = sizeof(OPENFILENAMEA);
        sfn_->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_LONGNAMES | OFN_OVERWRITEPROMPT;
        sfn_->lpstrFilter = static_cast<LPCSTR>(STR_FILE_FILTER);
        sfn_->lpstrDefExt = static_cast<LPCSTR>(STR_FILE_DEFEXT);
        sfn_->lpstrInitialDir = static_cast<LPCSTR>(STR_FILE_INITDIR);
        sfn_->nFilterIndex = 1;
        sfn_->hwndOwner = hwnd;
        sfn_->lpstrFile = static_cast<LPSTR>(
                            const_cast<char*>(s_.c_str())
                        );
        sfn_->nMaxFile = s_.size();
    }

    static bool f_GetFileDialog(HWND hwnd, std::string & s)
    {
        try
        {
            OPENFILENAMEA sfn{};
            std::string fname("default.png");
            fname.resize(512);
            s = "";

            f_SetupFileDialog(hwnd, &sfn, fname);
            if (!::GetSaveFileNameA(&sfn))
                return false;

            s = fname;
            return true;
        }
        catch (std::exception const & ex_)
        {
            s = ex_.what();
        }
        catch (std::string const & s_)
        {
            s = s_;
        }
        catch (...)
        {
            s = CATCH_ERR();
        }
        return false;
    }

    /// Class Draw

    Draw::Draw(HINSTANCE hinst, HWND hparent)
    {
        init(hinst, hparent);
        m_index = 0;
    }

    Draw::Draw(ImageRGBbuffer & b)
    {
        add(0, b);
    }

    Draw::Draw(
            ImageRGBbuffer & b1,
            ImageRGBbuffer & b2
        )
    {
        add(0, b1);
        add(1, b2);
    }

    Draw::~Draw()
    {
        close(true);
        if (m_idgdi)
            Gdiplus::GdiplusShutdown(m_idgdi);
    }

    void Draw::init(HINSTANCE hinst, HWND hparent)
    {
        m_hparent = hparent;
        m_hinst   = hinst;
    }

    bool Draw::isactive()
    {
        return (m_isrun.load()); //((m_isrun.load()) || (m_hwnd));
    }

    HWND Draw::gethwnd()
    {
        return ((m_hparent) ? m_hparent : HWND_DESKTOP);
    }

    HINSTANCE Draw::gethinstance()
    {
        return ((m_hinst) ? m_hinst : ::GetModuleHandle(0));
    }

    POINT Draw::getwinpadding()
    {
        return {
            (BORDER_PADDING * 2),
            ((::GetSystemMetrics(SM_CYFRAME) +
              ::GetSystemMetrics(SM_CYCAPTION) +
             (::GetSystemMetrics(SM_CXPADDEDBORDER) * 2)) +
             (BORDER_PADDING * 2))
        };
        // GetSystemMetrics(SM_CYSIZEFRAME) + (GetSystemMetrics(SM_CYEDGE) *2)
    }

    POINT Draw::getwinsize(int32_t idx)
    {
        idx = (((idx < 1) || (idx > 2)) ? 0 : idx);

        if (idx)
            return m_data[(idx - 1)].point.get<POINT, int32_t>();

        // main window size, client rectangle (point)
        switch (m_index.load())
        {
            case 1:
                {
                    return {
                        m_data[0].point.x,
                        m_data[0].point.y
                    };
                }
            case 2:
                {
                    return {
                        (std::max(m_data[0].point.x, m_data[1].point.x) / 2),
                        ((m_data[0].point.y / 2) + (m_data[1].point.y / 2))
                    };
                }
            default:
                return  { 0, 0 };
        }
    }

    /// private method

    void Draw::errhandle(std::string const & s)
    {
        try
        {
            throw;
        }
        catch (std::exception const & ex_)
        {
            statusbar(ex_.what());
        }
        catch (std::string const & s_)
        {
            statusbar(s_);
        }
        catch (int32_t i_)
        {
            statusbar(std::to_string(i_));
        }
        catch (uint32_t u_)
        {
            statusbar(std::to_string(u_));
        }
        catch (...)
        {
            statusbar(s);
        }
    }

    void Draw::setwinsize(POINT const & p)
    {
        do
        {
            if (!p.x || !p.y)
                break;

            RECT  rc{};
            POINT pad = getwinpadding();

            if ((!::IsWindowVisible(m_hwnd)) || (::IsIconic(m_hwnd)))
                ::ShowWindow(m_hwnd, SW_SHOW);

            if (!::GetWindowRect(m_hwnd, &rc))
                break;

            (void) ::SetWindowPos(
                        m_hwnd,
                        0,
                        rc.left,
                        rc.top,
                        (p.x / m_scale + pad.x),
                        (p.y / m_scale + pad.y),
                        0
                    );
            ::InvalidateRect(m_hwnd, 0, false);
            ::UpdateWindow(m_hwnd);
        }
        while (0);
    }

    bool Draw::initgdi()
    {
        if (m_idgdi)
            return true;

        Gdiplus::GdiplusStartupInput ingdi = {0};
        ingdi.GdiplusVersion = 1;
        if (Gdiplus::GdiplusStartup(&m_idgdi, &ingdi, 0))
            return false;
        return true;
    }

    void Draw::setwintop()
    {
        ::SetWindowPos(
            m_hwnd,
            ((m_iswintop) ? HWND_TOPMOST : HWND_NOTOPMOST),
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE
        );
        sysmenu(IDM_ONTOP,
                ((m_iswintop) ? MENU_CHECKED : MENU_UNCHECKED)
        );
    }

    void Draw::statusbar()
    {
        if (!m_hstatus)
            return;

        std::wstringstream ss;
        for (int32_t i = 0; i < m_index; i++)
        {
            if (i)
                ss << "   | ";
            ss << "  image #" << (i + 1) << ": ";
            ss << m_data[i].point.x << "x" << m_data[i].point.y;
        }
        if (!ss.str().empty())
            ::SendMessage(
                m_hstatus,
                SB_SETTEXT,
                MAKEWPARAM(SBT_NOBORDERS, SB_SIMPLEID),
                reinterpret_cast<LPARAM>(ss.str().c_str())
            );
    }

    void Draw::statusbar(std::string const & _s)
    {
        if (!m_hstatus)
        {
            errbox(_s);
            return;
        }
        std::wstring s(_s.begin(), _s.end());
        if (!s.empty())
            ::SendMessage(
                m_hstatus,
                SB_SETTEXT,
                MAKEWPARAM(SBT_NOBORDERS, SB_SIMPLEID),
                reinterpret_cast<LPARAM>(s.c_str())
            );
    }

    void Draw::sysmenu(uint32_t id, uint32_t cmd)
    {
        if ((!m_hwnd) || (!m_index.load()))
            return;

        HMENU hmenu = ::GetSystemMenu(m_hwnd, false);
        if (!hmenu)
            return;

        uint32_t flags = MF_STRING | MF_BYCOMMAND;

        switch (cmd)
        {
            case MENU_DISABLED:  flags |= MF_DISABLED | MF_GRAYED; break;
            case MENU_ENABLED:   flags |= MF_ENABLED; break;
            case MENU_CHECKED:   flags |= MF_CHECKED; break;
            case MENU_UNCHECKED: flags |= MF_UNCHECKED; break;
            default:
                return;
        }
        switch (cmd)
        {
            case MENU_DISABLED:
            case MENU_ENABLED:
                {
                    ::EnableMenuItem(hmenu, id, flags);
                    break;
                }
            case MENU_CHECKED:
            case MENU_UNCHECKED:
                {
                    ::CheckMenuItem(hmenu, id, flags);
                    break;
                }
        }
    }

    void Draw::sysmenu()
    {
        if (m_issysmenu)
            return;

        m_issysmenu = true;
        int32_t flags;

        if ((!m_hwnd) || (!m_index.load()))
            return;

        (void) ::GetSystemMenu(m_hwnd, true);
        HMENU hmenu = ::GetSystemMenu(m_hwnd, false);
        if (!hmenu)
            return;

        ::AppendMenuW(hmenu, MF_SEPARATOR, 0, L"");
        {
            flags = MF_STRING | MF_BYCOMMAND;
            if (m_iswintop.load())
                flags |= MF_CHECKED;
            ::AppendMenuW(hmenu, flags, IDM_ONTOP, WSTR_ONTOP);
        }
        ::AppendMenuW(hmenu, MF_SEPARATOR, 0, L"");

        switch (m_index.load())
        {
            case 1:
                {
                    ::AppendMenuW(hmenu, MF_STRING | MF_BYCOMMAND, IDM_SAVE_IMAGE_0, WSTR_SAVE_IMAGE0);
                    break;
                }
            case 2:
                {
                    ::AppendMenuW(hmenu, MF_STRING | MF_BYCOMMAND, IDM_SAVE_IMAGE_1, WSTR_SAVE_IMAGE1);
                    ::AppendMenuW(hmenu, MF_STRING | MF_BYCOMMAND, IDM_SAVE_IMAGE_2, WSTR_SAVE_IMAGE2);
                    break;
                }
            default:
                    return;
        }
        ::AppendMenuW(hmenu, MF_STRING | MF_BYCOMMAND | MF_DISABLED | MF_GRAYED, IDM_SAVE_REGION, WSTR_SAVE_REGION);
        ::AppendMenuW(hmenu, MF_SEPARATOR, 0, L"");

        HMENU hsmenu = CreatePopupMenu();
        ::AppendMenuW(hmenu, MF_STRING | MF_BYCOMMAND | MF_POPUP, (UINT_PTR)hsmenu, WSTR_SCALE);

        for (int32_t i = IDM_SCALE_11, n = 1; i <= IDM_SCALE_13; i++, n++)
        {
            flags = MF_STRING | MF_BYCOMMAND;
            if (m_scale.load() == n)
                flags |= MF_CHECKED;
            std::wstring ws(L"1:");
            ws.append(std::to_wstring(n));
            ::AppendMenuW(hsmenu, flags, i, ws.c_str());
        }
        ::DestroyMenu(hsmenu);
    }

    bool Draw::setup(HINSTANCE hinst_)
    {
        WNDCLASSEXW wndc{};

        /*
            // Develop only:
            ::UnregisterClassW(static_cast<LPCWSTR>(WSTR_VIEWER_CNAME), hinst_);
         */

        if (::GetClassInfoExW(hinst_, static_cast<LPCWSTR>(WSTR_VIEWER_CNAME), &wndc))
            return true;

        wndc.cbSize        = sizeof(wndc);
        wndc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wndc.lpfnWndProc   = &Draw::wndRedirect_;
        wndc.hInstance     = hinst_;
        wndc.lpszClassName = WSTR_VIEWER_CNAME;
        wndc.hCursor       = ::LoadCursor(0, IDC_ARROW);
#       if defined(ID_VIEWER_ICON)
        if (hinst_)
            wndc.hIcon = ::LoadIconW(hinst_, MAKEINTRESOURCEW(ID_VIEWER_ICON));
#       endif

        if (!::RegisterClassExW(&wndc))
            return false;
        return true;
    }

    void Draw::errbox(std::string const & s)
    {
        ::MessageBoxA(
            gethwnd(),
            static_cast<LPCSTR>(s.c_str()),
            STR_VIEWER_MBOX_NAME,
            MB_OK | MB_ICONERROR
        );
    }

    /// public section

    void Draw::add(int32_t idx, ImageRGBbuffer & b)
    {
        switch (idx)
        {
            case 0:
                {
                    m_data[1] = {};
                    idx = 0; m_index = 1;
                    break;
                }
            case 2:
                {
                    if (!m_index)
                    {
                        idx = 0; m_index = 1;
                    }
                    else
                    {
                        idx = 1; m_index = 2;
                    }
                    break;
                }
            default: idx = 0; m_index = 1; break;
        }
        try
        {
            if (b.empty())
                return;

            m_data[idx] = b.getbmp();

            do
            {
                if ((!m_hwnd) || (!m_isrun))
                    break;

                POINT p = getwinsize(0);
                if (!p.x || !p.y)
                    break;

                setwinsize(p);
                statusbar();
            }
            while (0);
            m_issysmenu = false;
        }
        catch (...)
        {
            errhandle(CATCH_ERR());
            return;
        }
    }

    void Draw::add(int32_t idx, ImageRGBbuffer & b, std::string const & s)
    {
        if (m_hwnd)
        {
            std::wstring ws(s.begin(), s.end());
            ::SetWindowTextW(m_hwnd, ws.c_str());
        }
        add(idx, b);
    }

    void Draw::close(bool isdispose)
    {
        m_isrun = false;
        if (m_hwnd)
            if (isdispose)
                ::SendMessage(m_hwnd, WM_CLOSE, 0, 0);
        if (m_thr.joinable())
            m_thr.join();

        m_index   = 0;
        m_data[0] = {};
        m_data[1] = {};
        m_hwnd    = nullptr;
    }

    bool Draw::show(std::string const & s, HINSTANCE hinst, HWND hparent)
    {
        if ((m_hwnd) || (m_isrun))
        {
            do
            {
                if (!m_hwnd)
                    break;

                POINT p = getwinsize(0);
                if (!p.x || !p.y)
                    break;

                setwinsize(p);
            }
            while (0);
            return true;
        }
        if (!m_idgdi)
            if (!initgdi())
                return false;

        HINSTANCE l_hinst   = ((hinst) ?
                     hinst : gethinstance()
                );
        HWND l_hparent = ((hparent) ?
                     hparent : gethwnd()
                );

        if (!setup(l_hinst))
            return false;

        if (m_thr.joinable())
            m_thr.join();

        m_isrun = true;

        try
        {
            std::thread thr
            {
                [=]()
                {
                    do
                    {
                        POINT p = getwinsize(0);
                        if (!p.x || !p.y)
                            break;

                        std::wstring ws(s.begin(), s.end());
                        POINT pad = getwinpadding();

                        m_hwnd = CreateWindowExW(
                                WS_EX_CLIENTEDGE,
                                static_cast<LPCWSTR>(WSTR_VIEWER_CNAME),
                                static_cast<LPCWSTR>(ws.c_str()),
                                WS_OVERLAPPEDWINDOW | WS_EX_COMPOSITED,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                (p.x + pad.x),
                                (p.y + pad.y),
                                l_hparent,
                                0,
                                l_hinst,
                                reinterpret_cast<LPVOID>(this)
                            );
                        if (!m_hwnd)
                            break;

                        m_hstatus = ::CreateStatusWindowW(
                                WS_CHILD | SBARS_TOOLTIPS,
                                L"",
                                m_hwnd,
                                ID_STATUS_BAR
                            );
                        if (!m_hstatus)
                            break;

                        statusbar();
                        setwintop();
                        //
                        ::SendMessage(m_hstatus, SB_SETBKCOLOR, 0, (LPARAM)RGB(255,255,255));
                        ::ShowWindow(m_hwnd, SW_SHOW);
                        ::ShowWindow(m_hstatus, SW_SHOW);

                        MSG msg{};
                        while (::GetMessage(&msg, 0, 0, 0) > 0)
                        {
                            ::TranslateMessage(&msg);
                            ::DispatchMessage(&msg);
                        }
                    }
                    while (0);
                    m_isrun = false;
                }
            };
            m_thr = move(thr);
            return true;
        }
        catch (...)
        {
            m_isrun = false;
            errhandle(CATCH_ERR());
            return false;
        }
    }

    /// WndProc private section

    LRESULT CALLBACK Draw::wndRedirect_(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
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
        ImageLite::Draw *imgd =
            reinterpret_cast<ImageLite::Draw*>(GetWindowLongPtrA(hwnd_, GWLP_USERDATA));
        if (imgd)
            return imgd->wndProc_(hwnd_, umsg_, wp_, lp_);
        return ::DefWindowProc(hwnd_, umsg_, wp_, lp_);
    }

    LRESULT Draw::wndProc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        try
        {
            switch(msg)
            {
                case WM_CLOSE:
                    {
                        m_isrun = false;
                        ::DestroyWindow(hwnd);
                        return 0;
                    }
                case WM_DESTROY:
                    {
                        ::PostQuitMessage(0);
                        return 0;
                    }
                case WM_NCDESTROY:
                    {
                        if (m_isrun.load())
                            m_isrun = false;
                        m_hstatus = nullptr;
                        m_hwnd    = nullptr;
                        return 0;
                    }
                case WM_PAINT:
                    {
                        PAINTSTRUCT ps{};
                        HDC     hdc  = ::BeginPaint(hwnd, &ps),
                                hdcb = ::CreateCompatibleDC(hdc);
                        HBITMAP hbmpb = nullptr,
                                hbmpo = nullptr;

                        do
                        {
                            if (!hdcb)
                                break;
                            if (!(hbmpb = ::CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom)))
                                break;
                            if (!(hbmpo = (HBITMAP)::SelectObject(hdcb, hbmpb)))
                                break;

                            if (m_index.load())
                            {
                                Gdiplus::Bitmap *drawimg1 = nullptr,
                                                *drawimg2 = nullptr;
                                try
                                {
                                    Gdiplus::Graphics gdi(hdcb);
                                    gdi.Clear(Gdiplus::Color(255, 255, 255));

                                    RECT rc{};
                                    ::GetClientRect(hwnd, &rc);
                                    if (m_hstatus)
                                    {
                                        RECT rs{};
                                        ::GetWindowRect(m_hstatus, &rs);
                                        rc.bottom -= (
                                                (rs.bottom - rs.top) +
                                                (::GetSystemMetrics(SM_CXPADDEDBORDER) * 2)
                                            );
                                    }
                                    switch (m_index.load())
                                    {
                                        case 1:
                                            {
                                                drawimg1 = new Gdiplus::Bitmap(
                                                    reinterpret_cast<BITMAPINFO*>(&m_data[0].header),
                                                    &m_data[0].buffer[0]
                                                );
                                                Gdiplus::Rect rect1(
                                                    10,
                                                    10,
                                                    rc.right  - BORDER_PADDING,
                                                    rc.bottom - BORDER_PADDING
                                                );
                                                gdi.DrawImage(drawimg1, rect1);
                                                break;
                                            }
                                        case 2:
                                            {
                                                drawimg1 = new Gdiplus::Bitmap(
                                                    reinterpret_cast<BITMAPINFO*>(&m_data[0].header),
                                                    &m_data[0].buffer[0]
                                                );
                                                drawimg2 = new Gdiplus::Bitmap(
                                                    reinterpret_cast<BITMAPINFO*>(&m_data[1].header),
                                                    &m_data[1].buffer[0]
                                                );
                                                auto x = (rc.right - BORDER_PADDING);
                                                auto y = ((rc.bottom / 2) - (BORDER_PADDING / 2) - 2);
                                                // (y) last -2 == pen size / 2

                                                Gdiplus::Pen  pen1(Gdiplus::Color(100, 192, 0), 4);
                                                Gdiplus::Pen  pen2(Gdiplus::Color(0, 122, 192), 4);
                                                Gdiplus::Rect rect1(
                                                    10,
                                                    10,
                                                    x,
                                                    y
                                                );
                                                Gdiplus::Rect rect2(
                                                    10,
                                                    (rect1.Height + BORDER_PADDING),
                                                    x,
                                                    y
                                                );
                                                gdi.DrawRectangle(&pen1, rect1);
                                                gdi.DrawRectangle(&pen2, rect2);
                                                gdi.DrawImage(drawimg1, rect1);
                                                gdi.DrawImage(drawimg2, rect2);
                                                break;
                                            }
                                        default:
                                                break;
                                    }
                                    if (!m_select.empty(true))
                                    {
                                        Gdiplus::Pen  pen(Gdiplus::Color(255, 0, 0), 2);
                                        Gdiplus::Rect rect = m_select.get<Gdiplus::Rect>();
                                        gdi.DrawRectangle(&pen, rect);
                                    }
                                }
                                catch (...)
                                {
                                    errhandle(CATCH_ERR());
                                }

                                if (drawimg1)
                                    delete drawimg1;
                                if (drawimg2)
                                    delete drawimg2;
                            }
                            else
                                ::FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                        }
                        while (0);

                        if (hdcb)
                            ::BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcb, 0, 0, SRCCOPY);

                        ::EndPaint(hwnd, &ps);

                        if (hdcb)
                        {
                            if (hbmpo)
                                ::SelectObject(hdcb, hbmpo);
                            ::DeleteDC(hdcb);
                        }
                        if (hbmpb)
                            ::DeleteObject(hbmpb);
                        if (hdcb)
                            ::DeleteDC(hdcb);
                        return 0;
                    }
                case WM_SIZE:
                    {
                        if (m_hstatus)
                            ::SendMessage(m_hstatus, WM_SIZE, wParam, lParam);
                        ::InvalidateRect(hwnd, 0, false);
                        ::UpdateWindow(hwnd);
                        return 0;
                    }
                case WM_ERASEBKGND:
                    {
                        if (m_select.active)
                            return 1;
                        break;
                    }
                case WM_LBUTTONDOWN:
                    {
                        if ((m_index.load() != 1) || (m_scale.load() != 1))
                            break;

                        POINTS p = MAKEPOINTS(lParam);
                        if (!m_select.empty())
                            ::InvalidateRect(hwnd, 0, false);
                        //
                        m_select.init(p);
                        sysmenu(IDM_SAVE_REGION, MENU_DISABLED);
                        return 0;
                    }
                case WM_LBUTTONUP:
                    {
                        if ((m_index.load() != 1) || (m_scale.load() != 1))
                            break;

                        if (!m_select.empty(true))
                            sysmenu(IDM_SAVE_REGION, MENU_ENABLED);

                        if (m_select.active)
                            statusbar(m_select.print(BORDER_PADDING / 2));

                        m_select.active = false;
                        return 0;
                    }
                case WM_LBUTTONDBLCLK:
                    {
                        if ((m_index.load() != 1) || (m_scale.load() != 1))
                            break;

                        sysmenu(IDM_SAVE_REGION, MENU_DISABLED);

                        if (m_select.empty(true))
                            break;

                        m_select.erase();
                        ::InvalidateRect(hwnd, 0, false);
                        statusbar(STR_SELECT_CANCEL);
                        return 0;
                    }
                case WM_RBUTTONDBLCLK:
                    {
                        int32_t id;
                        switch (m_scale)
                        {
                            case 1:  m_scale = 2; id = IDM_SCALE_12; break;
                            case 2:  m_scale = 3; id = IDM_SCALE_13; break;
                            default: m_scale = 1; id = IDM_SCALE_11; break;
                        }
                        ::SendMessage(hwnd, WM_SYSCOMMAND, (WPARAM)id, (LPARAM)0);
                        if (m_scale == 1)
                            sysmenu(IDM_SAVE_REGION, MENU_ENABLED);
                        else
                            sysmenu(IDM_SAVE_REGION, MENU_DISABLED);
                        return 0;
                    }
                case WM_MOUSEMOVE:
                    {
                        if ((m_index.load() != 1) || (m_scale.load() != 1))
                            break;

                        POINTS p = MAKEPOINTS(lParam);
                        if (!m_select.set(p))
                            return 0;

                        RECT rc = m_select.eraseregion();
                        ::InvalidateRect(hwnd, &rc, false);
                        statusbar(m_select.print(BORDER_PADDING / 2));
                        return 0;
                    }
                case WM_INITMENU:
                    {
                        sysmenu();
                        return 0;
                    }
                case WM_SYSCOMMAND:
                    {
                        uint32_t id = LOWORD(wParam);
                        switch (id)
                        {
                            case IDM_ONTOP:
                                {
                                    m_iswintop = !(m_iswintop.load());
                                    setwintop();
                                    return 0;
                                }
                            case IDM_SCALE_11:
                            case IDM_SCALE_12:
                            case IDM_SCALE_13:
                                {
                                    POINT p = getwinsize(0);
                                    if (!p.x || !p.y)
                                        break;

                                    switch (id)
                                    {
                                        case IDM_SCALE_12: m_scale = 2; break;
                                        case IDM_SCALE_13: m_scale = 3; break;
                                        default:           m_scale = 1; break;
                                    }
                                    setwinsize(p);

                                    for (int32_t i = IDM_SCALE_11, n = 1; i <= IDM_SCALE_13; i++, n++)
                                    {
                                        sysmenu(i,
                                              ((m_scale == n) ? MENU_CHECKED : MENU_UNCHECKED)
                                            );
                                    }
                                    return 0;
                                }
                            case IDM_SAVE_REGION:
                                {
                                    do
                                    {
                                        if (m_index.load() != 1)
                                            break;

                                        if (m_scale.load() != 1)
                                        {
                                            statusbar(STR_SELECT_SCALEWARN);
                                            break;
                                        }

                                        ImageLite::IRECT<int32_t> ir =
                                            m_select.get<ImageLite::IRECT<int32_t>>((BORDER_PADDING / 2), 3);

                                        if (ir.x < 0)
                                            ir.w -= ir.x;
                                        if (ir.y < 0)
                                            ir.h -= ir.y;
                                        if (m_data[0].point.x < ir.w)
                                            ir.w = m_data[0].point.x;
                                        if (m_data[0].point.y < (ir.y + ir.h))
                                            ir.h -= ((ir.y + ir.h) - m_data[0].point.y);
                                        if ((ir.h <= 0) || (ir.w <= 0))
                                            break;

                                        try
                                        {
                                            std::string s;
                                            if (!f_GetFileDialog(hwnd, s))
                                            {
                                                if (!s.empty())
                                                    statusbar(s);
                                                break;
                                            }
                                            ImageLite::ImageRGBbuffer img(m_data[0], ir, BufferType::IBT_BGR_PAD);
                                            ImageLite::ImageType it = img.getimgtype(s);
                                            if (it == ImageLite::ImageType::IT_NONE)
                                                break;

                                            img.save(s, it);

                                            if (it == ImageLite::ImageType::IT_ILUA)
                                            {
                                                statusbar(STR_SAVE_OK);
                                                break;
                                            }

                                            if (!::OpenClipboard(hwnd))
                                                break;
                                            do
                                            {
                                                std::stringstream ss;
                                                ss << STR_LUA_CODE << "(\"" << s.c_str() << "\", ";
                                                ss << ir.x << ", " << ir.y << ", " << ir.w << ", " << ir.h << ")\n";

                                                HGLOBAL hmem = ::GlobalAlloc(GMEM_MOVEABLE, (ss.str().length() + 1));
                                                if (!hmem)
                                                    break;
                                                ::memcpy(::GlobalLock(hmem), ss.str().data(), ss.str().length());
                                                ::GlobalUnlock(hmem);
                                                ::EmptyClipboard();
                                                ::SetClipboardData(CF_TEXT, hmem);
                                            }
                                            while (0);
                                            ::CloseClipboard();
                                            statusbar(STR_SAVE_REGION);
                                        }
                                        catch (...)
                                        {
                                            errhandle(CATCH_ERR());
                                        }
                                    }
                                    while (0);

                                    m_select.erase();
                                    sysmenu(IDM_SAVE_REGION, MENU_DISABLED);

                                    ::InvalidateRect(hwnd, 0, false);
                                    ::UpdateWindow(hwnd);
                                    return 0;
                                }
                            case IDM_SAVE_IMAGE_0:
                            case IDM_SAVE_IMAGE_1:
                            case IDM_SAVE_IMAGE_2:
                                {
                                    do
                                    {
                                        int32_t idx = ((id == IDM_SAVE_IMAGE_2) ? 1 : 0);
                                        idx = (((m_index.load() - 1) < idx) ? -1 : idx);
                                        if (m_index.load() == -1)
                                            break;

                                        try
                                        {
                                            std::string s;
                                            if (!f_GetFileDialog(hwnd, s))
                                            {
                                                if (!s.empty())
                                                    statusbar(s);
                                                break;
                                            }
                                            ImageLite::ImageRGBbuffer img(m_data[0], BufferType::IBT_BGR_PAD);
                                            ImageLite::ImageType it = img.getimgtype(s);
                                            if (it == ImageLite::ImageType::IT_NONE)
                                                break;

                                            img.save(s, it);
                                            statusbar(STR_SAVE_OK);
                                        }
                                        catch (...)
                                        {
                                            errhandle(CATCH_ERR());
                                        }
                                        return 0;
                                    }
                                    while (0);

                                    statusbar(CATCH_ERR());
                                    return 0;
                                }
                            default:
                                break;
                        }
                        break;
                    }
                default: return DefWindowProc (hwnd, msg, wParam, lParam);
            }
            return DefWindowProc (hwnd, msg, wParam, lParam);
        }
        catch (...)
        {
            errhandle(CATCH_ERR());
            return 0;
        }
    }
};
