
#include "SCEditInternal.h"
#include <Commctrl.h>
#include <richedit.h>

namespace Editor
{
    static const COLORREF l_color[] =
    {
        RGB(240, 0, 0),
        RGB(0, 240, 0),
        RGB(0, 0, 240),
        RGB(40, 40, 40)
    };

    InfoBox::InfoBox()
    {
        m_libed.init("msftedit.dll");
    }

    InfoBox::~InfoBox()
    {
        if (m_hrtb)
            ::DestroyWindow(m_hrtb);
        m_hrtb = nullptr;
    }

    bool InfoBox::ready()
    {
        return (m_hrtb != nullptr);
    }

    void InfoBox::text()
    {
        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ST_DEFAULT;
        ::SendMessage(m_hrtb, EM_SETTEXTEX, (WPARAM)&se, (LPARAM)&"");
    }

    void InfoBox::text(std::string const & s)
    {
        highlight(l_color[InfoBox::TextType::IB_TYPE_DEFAULT]);
        settext(s, false);
    }

    void InfoBox::text(std::string const & s, COLORREF const & color)
    {
        highlight(color);
        settext(s, false);
    }

    void InfoBox::text(std::string const & s, InfoBox::TextType t)
    {
        highlight(l_color[t]);
        settext(s, false);
    }

    void InfoBox::highlight(COLORREF const & color)
    {
        CHARFORMAT cf{};
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_COLOR;
        cf.dwEffects = 0;
        cf.crTextColor = color;
        ::SendMessage(m_hrtb, EM_SETSEL, -2, -1);
        ::SendMessage(m_hrtb, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    void InfoBox::settext(std::string const & s, bool isclear)
    {
        if ((!m_hrtb) || (s.empty()))
            return;

        std::vector<char> v(s.length() + 3);
        v.assign(s.begin(), s.end());
        v[s.length()] = '\r';
        v[s.length() + 1] = '\n';
        v[s.length() + 2] = '\0';

        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ((isclear) ? ST_DEFAULT : ST_SELECTION);
        ::SendMessage(m_hrtb, EM_SETTEXTEX, (WPARAM)&se,      (LPARAM)&v[0]);
        ::SendMessage(m_hrtb, WM_VSCROLL,   (WPARAM)SB_BOTTOM, (LPARAM)0);
    }

    bool InfoBox::create(HWND hwnd_, HINSTANCE hinst_, RECT const & r, std::string const & s)
    {
        do
        {
            if (!m_libed.isload())
            {
                if (m_libed.geterror())
                    ERRORBoxEx(m_libed.geterror());
                break;
            }
            if (!WindowBase::create(
                    hwnd_,
                    hinst_,
                    r,
                    WS_OVERLAPPED | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
                    SCEdit::getChildClassName(),
                    "Debug Output:"
                ))
                break;

            if (!setup(WindowBase::m_hwnd, hinst_, r))
                break;

            WindowBase::active();

            if (!s.empty())
                text(s);

            Editor::PrintBox::instance().init(this);
            return true;
        }
        while (0);
        return false;
    }

    bool InfoBox::setup(HWND hwnd_, HINSTANCE hinst_, RECT const & rc)
    {
        do
        {
            if ((!hwnd_) || (!hinst_))
                break;

            RECT re{};
            re.top = 0;
            re.left = 2;
            re.right = (rc.right - 4);
            re.bottom = (rc.bottom - 2);

            if (!(m_hrtb = WindowBase::createEx<std::wstring>(
                    hwnd_,
                    hinst_,
                    (HMENU)ID_INFO_RTBOX,
                    re,
                    WS_CHILD | WS_VSCROLL | WS_VISIBLE | WS_CLIPCHILDREN |
                    ES_MULTILINE | ES_SUNKEN | ES_AUTOVSCROLL | ES_READONLY,
                    MSFTEDIT_CLASS, L""
                    )))
                    break;

            ::SendMessage(m_hrtb, EM_FMTLINES,      (WPARAM)1, (LPARAM)0);
            ::SendMessage(m_hrtb, EM_AUTOURLDETECT, (WPARAM)1, (LPARAM)0);
            ::SendMessage(m_hrtb, EM_SETMARGINS,    (WPARAM)EC_LEFTMARGIN, (LPARAM)12);
            ::ShowWindow (m_hrtb, SW_SHOW);
            return true;
        }
        while (0);
        return false;
    }
};
