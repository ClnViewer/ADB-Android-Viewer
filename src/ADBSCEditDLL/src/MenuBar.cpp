
#include "SCEditInternal.h"

namespace Editor
{
    static WNDPROC redirect_wndp_default = nullptr;
    static LRESULT CALLBACK redirectProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        LRESULT ret = 0;
        if (umsg_ == WM_COMMAND)
            (void) ::SendMessage(Config::instance().gethandle(), umsg_, wp_, lp_);
        else if (redirect_wndp_default)
            ret = ::CallWindowProc(redirect_wndp_default, hwnd_, umsg_, wp_, lp_);
        return ret;
    }

    bool MenuBar::create(HWND hwnd_, HWND phwnd_, HINSTANCE hinst_)
    {
        RECT rc = { 0, 0, CW_USEDEFAULT, CW_USEDEFAULT };
        if (!WindowBase::create(
                hwnd_,
                hinst_,
                rc,
                WS_OVERLAPPED | WS_CLIPCHILDREN | WS_BORDER,
                SCEdit::getMenuClassName(),
                ""
            ))
            return false;

        HFONT hfont = Editor::Config::instance().getfont();
        if (hfont)
            ::SendMessage(WindowBase::m_hwnd, WM_SETFONT, (LPARAM)hfont, 1);

        ::SetParent(WindowBase::m_hwnd, phwnd_);
        redirect_wndp_default = (WNDPROC)::SetWindowLongPtr(WindowBase::m_hwnd, GWLP_WNDPROC, (LONG_PTR)redirectProc);
        return true;
    }

    void MenuBar::setenable(uint32_t cmd, bool isenable)
    {
        HMENU hm = ::GetMenu(WindowBase::m_hwnd);
        if (!hm)
            return;

        uint32_t flags = MF_BYCOMMAND;
        if (isenable)
            flags |= MF_ENABLED;
        else
            flags |= MF_DISABLED | MF_GRAYED;

        ::EnableMenuItem(hm, cmd, flags);
        ::DrawMenuBar(WindowBase::m_hwnd);
    }

    void MenuBar::setcheck(uint32_t cmd, bool ischeck)
    {
        HMENU hm = ::GetMenu(WindowBase::m_hwnd);
        if (!hm)
            return;

        uint32_t flags = MF_BYCOMMAND | MF_ENABLED;
        if (ischeck)
            flags |= MF_CHECKED;
        else
            flags |= MF_UNCHECKED;

        ::CheckMenuItem(hm, cmd, flags);
        ::DrawMenuBar(WindowBase::m_hwnd);
    }
};
