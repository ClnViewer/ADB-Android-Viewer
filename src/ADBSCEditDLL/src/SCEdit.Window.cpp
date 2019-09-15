
#include "SCEditInternal.h"

namespace Editor
{
    bool SCEdit::registerWin_(std::string const & s, HICON hicon, int32_t imenu)
    {
        WNDCLASSA wndc{};

        if (::GetClassInfoA(m_hinst, static_cast<LPCSTR>(s.c_str()), &wndc))
            return true;

        wndc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wndc.lpfnWndProc   = &SCEdit::wndRedirect_;
        wndc.cbClsExtra    = 0;
        wndc.cbWndExtra    = 0;
        wndc.hInstance     = m_hinst;
        wndc.hCursor       = 0;
        wndc.hbrBackground = 0;

        if (imenu)
            wndc.lpszMenuName = MAKEINTRESOURCE(imenu);

        if (hicon)
            wndc.hIcon = hicon;
        else
            wndc.hIcon = ::LoadIconA(m_hinst, MAKEINTRESOURCE(ID_ICON));

        do
        {
            if (!m_hbgmain)
                if (!(m_hbgmain = ::CreateSolidBrush(RGB(240,240,240))))
                    break;

            wndc.hbrBackground = m_hbgmain;
            wndc.lpszClassName = static_cast<LPCSTR>(s.c_str());
            if (!::RegisterClassA(&wndc))
                break;

            return true;
        }
        while (0);
        return false;
    }

    bool SCEdit::calcWinSize_()
    {
        int32_t dt = 25,
                dl = 75,
                dx = (GetSystemMetrics(SM_CXSCREEN) - 150),
                dy = (GetSystemMetrics(SM_CYSCREEN) - 75);
        return calcWinSize_(dx, dy, dt, dl, 150);
    }

    bool SCEdit::calcWinSize_(HWND hwnd_)
    {
        RECT rcw{};
        if (!::GetWindowRect(hwnd_, &rcw))
            return false;

        /*
        ::AdjustWindowRectEx(
                &rcw,
                WS_BORDER | WS_CLIPCHILDREN | WS_THICKFRAME,
                0,
                WS_EX_CLIENTEDGE | WS_EX_COMPOSITED
            );
        */
        return calcWinSize_((rcw.right  - rcw.left), (rcw.bottom - rcw.top), rcw.top, rcw.left, 0);
    }

    bool SCEdit::calcWinSize_(int32_t dx, int32_t dy, int32_t dt, int32_t dl, int32_t off)
    {
        if ((dx <= 0) || (dy <= 0))
            return false;

        m_rect.rmain   = {};
        m_rect.rinf    = {};
        m_rect.redit   = {};

        m_rect.rtbar.right  = ((m_rect.rtbar.right) ? m_rect.rtbar.right : dx);
        m_rect.rtbar.bottom = ((m_rect.rtbar.bottom) ? m_rect.rtbar.bottom : 40);

        m_rect.rmain.top    = dt;
        m_rect.rmain.left   = dl;
        m_rect.rmain.right  = dx;                         // w
        m_rect.rmain.bottom = (dy - off);                 // h

        m_rect.redit.top    = m_rect.rtbar.bottom;
        m_rect.redit.right  = (m_rect.rmain.right - 4);
        m_rect.redit.bottom = (m_rect.rmain.bottom - m_rect.redit.top - 4);

        m_rect.rinf.top     = (m_rect.rmain.top + m_rect.rmain.bottom);
        m_rect.rinf.left    = m_rect.rmain.left;
        m_rect.rinf.right   = m_rect.rmain.right;         // w
        m_rect.rinf.bottom  = ((off) ? off : 150);        // h

        return true;
    }

    bool SCEdit::createWin_()
    {
        m_haccel = ::LoadAcceleratorsA(m_hinst, MAKEINTRESOURCE(ID_ACCEL));

        ::SetLastError(0);

        if (!calcWinSize_())
        {
            ERRORBox();
            return false;
        }

        ::SetLastError(0);

        WindowBase::m_hwnd = ::CreateWindowExA(
                        WS_EX_CLIENTEDGE,
                        static_cast<LPCSTR>(classNameMain),
                        nullptr,
                        WS_OVERLAPPEDWINDOW | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_EX_COMPOSITED,
                        m_rect.rmain.left, m_rect.rmain.top,
                        m_rect.rmain.right, m_rect.rmain.bottom,
                        0,
                        0,
                        m_hinst,
                        reinterpret_cast<LPVOID>(this)
                    );

        if (!WindowBase::m_hwnd)
        {
            ERRORBox();
            return false;
        }

        ::SetWindowLong(WindowBase::m_hwnd, GWL_STYLE, WS_BORDER);
        ::ShowWindow(WindowBase::m_hwnd, SW_SHOWNORMAL);

        Editor::Config::instance().init(WindowBase::m_hwnd);
        return true;
    }

    int32_t SCEdit::loopWin_()
    {
        bool isloop;
        MSG msg{};

        do
        {
            if ((isloop = static_cast<bool>(::GetMessageA(&msg, NULL, 0, 0))))
                if (::TranslateAcceleratorA(WindowBase::m_hwnd, m_haccel, &msg) == 0)
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessageA(&msg);
                }
        }
        while (isloop);
        return msg.wParam;
    }
};
