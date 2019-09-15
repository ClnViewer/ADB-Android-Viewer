
#include "SCEditInternal.h"
#include <commctrl.h>

namespace Editor
{
    static WNDPROC redirect_wndp_default = nullptr;
    static LRESULT CALLBACK redirectProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        LRESULT ret = 0;

        /*
        /// Fixed key sensitive:
        if ((umsg_ == WM_KEYUP) &&
            ((wp_ == VK_BACK) || (wp_ == VK_DELETE) || (wp_ == VK_RETURN)))
            (void) ::SendMessage(Config::instance().gethandle(), WM_COMMAND, (WPARAM)IDM_REBAR_FIND_TEXT, (LPARAM)0);
            */
        if (umsg_ == WM_KEYUP)
            (void) ::SendMessage(Config::instance().gethandle(), WM_COMMAND, (WPARAM)IDM_REBAR_FIND_TEXT, (LPARAM)0);
        else if (redirect_wndp_default)
            ret = ::CallWindowProc(redirect_wndp_default, hwnd_, umsg_, wp_, lp_);
        return ret;
    }

    bool SearchBar::create(HWND hwnd_, HWND phwnd_, HINSTANCE hinst_)
    {
        if (!WindowBase::create(
                hwnd_,
                hinst_,
                (HMENU)ID_REBAR_SEARCH,
                WS_VISIBLE          |
                WS_CHILD            |
                WS_CLIPCHILDREN     |
                WS_CLIPSIBLINGS     |
                WS_BORDER           |
                ES_LEFT             |
                ES_AUTOVSCROLL,
                "EDIT", ""
            ))
            return false;

        HFONT hfont = Editor::Config::instance().getfont();
        if (hfont)
            ::SendMessage(WindowBase::m_hwnd, WM_SETFONT, (LPARAM)hfont, 1);

        ::SendMessage(WindowBase::m_hwnd, EM_SETMARGINS, EC_LEFTMARGIN,  (LPARAM)5);
        ::SendMessage(WindowBase::m_hwnd, EM_SETMARGINS, EC_RIGHTMARGIN, (LPARAM)10);

        ::SetParent(WindowBase::m_hwnd, phwnd_);
        redirect_wndp_default = (WNDPROC)::SetWindowLongPtr(WindowBase::m_hwnd, GWLP_WNDPROC, (LONG_PTR)redirectProc);
        return true;
    }

    std::string SearchBar::getfind()
    {
        do
        {
            if (!WindowBase::m_hwnd)
                break;

            int32_t sz = ::GetWindowTextLength(WindowBase::m_hwnd);
            if (!sz)
                break;

            auto dbuf = std::make_unique<char[]>(++sz);
            if (!::GetWindowText(WindowBase::m_hwnd, dbuf.get(), sz))
                break;

            return std::string(dbuf.get());
        }
        while (0);
        return std::string();
    }
};
