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
#include <codecvt>
#include <richedit.h>

namespace Editor
{
    static inline const char     l_str_ourl[] = " -- open URL: ";
    //
    static void f_styleText(HWND hwnd, uint32_t flags)
    {
        CHARFORMAT cf{};
        cf.cbSize = sizeof(cf);
        //
        ::SendMessage(hwnd, EM_GETCHARFORMAT, (WPARAM)1, (LPARAM)&cf);
        //
        switch (flags)
        {
            case CFM_BOLD:
                {
                    cf.dwMask = CFM_BOLD | CFM_COLOR;
                    cf.dwEffects ^= CFE_BOLD | CFE_AUTOCOLOR;
                    cf.crTextColor = RGB(128,128,128);
                    break;
                }
            case CFM_UNDERLINE:
                {
                    cf.dwMask = CFM_BOLD | CFM_UNDERLINE | CFM_COLOR;
                    cf.dwEffects ^= CFE_BOLD | CFE_UNDERLINE | CFE_AUTOCOLOR;
                    cf.crTextColor = RGB(77,77,77);
                    break;
                }
            case CFM_SUBSCRIPT:
                {
                    cf.dwMask = CFM_SUBSCRIPT;
                    cf.dwEffects ^= CFE_SUBSCRIPT;
                    break;
                }
            case CFM_LINK:
                {
                    cf.dwMask = CFM_LINK | CFM_LINKPROTECTED | CFM_COLOR;
                    cf.dwEffects ^= CFE_LINK | CFE_LINKPROTECTED | CFE_AUTOCOLOR;
                    cf.crTextColor = RGB(200,30,30);
                    break;
                }
            default:
                {
                    cf.dwMask = CFM_FACE;
                    cf.dwEffects = 0;
                    break;
                }
        }
        //
        ::SendMessage(hwnd, EM_SETSEL, (WPARAM)-2, (LPARAM)-1);
        ::SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    void ToolBox::print_help_(AppHelp::helparray_t & var, HWND hwnd)
    {
        auto & [ items, isz ] = var;
        if ((!isz) || (!items))
            return;

        if (!hwnd)
        {
            hwnd = getchild(TabIndex::ITAB_HELP);
            if (!hwnd)
                return;
        }

        ::LockWindowUpdate(hwnd);

        for (uint32_t i = 0; i < isz; i++)
        {
            std::string s;
            AppHelp::HelpItems & itm = items[i];
            if (itm.flag == CFM_LINK)
            {
                s  = itm.title;
                s += "\n";
                print_help_(hwnd, s, CFM_LINK);
                continue;
            }
            else if ((itm.flag != CFM_ALL) && (itm.flag != CFM_ALL2))
                continue;

            if (itm.title)
            {
                s  = "\n";
                s += itm.title;
                s += "\n";
                print_help_(hwnd, s, CFM_UNDERLINE);
                if (itm.flag != CFM_ALL2)
                    continue;
            }
            if (itm.key)
            {
                s = itm.key;
                print_help_(hwnd, s, CFM_BOLD);
            }
            if (itm.desc)
            {
                s  = "   ";
                s += itm.desc;
                print_help_(hwnd, s, CFM_SUBSCRIPT);
            }
            if (itm.info)
            {
                s  = "   ";
                s += itm.info;
                print_help_(hwnd, s, CFM_SUBSCRIPT);
            }
        }
        ::LockWindowUpdate(nullptr);
        ::SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);
    }

    void ToolBox::print_help_(HWND hwnd, std::string const & s, uint32_t flags)
    {
        if (!hwnd)
        {
            hwnd = getchild(TabIndex::ITAB_HELP);
            if (!hwnd)
                return;
        }

        std::vector<char> vs(s.length() + 2);
        vs.assign(s.begin(), s.end());
        vs[s.length()] = '\n';
        vs[s.length() + 1] = '\0';

        f_styleText(hwnd, flags);

        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ST_SELECTION;
        ::SendMessage(
            hwnd,
            ((flags == CFM_LINK) ? EM_SETTEXTEX : EM_REPLACESEL),
            (WPARAM)&se,
            (LPARAM)&vs[0]
        );
    }

    void ToolBox::clear_help_(HWND hwnd)
    {
        if (!hwnd)
        {
            hwnd = getchild(TabIndex::ITAB_HELP);
            if (!hwnd)
                return;
        }
        ::SendMessage(hwnd, EM_SETSEL,     (WPARAM)0, (LPARAM)-1);
        ::SendMessage(hwnd, EM_REPLACESEL, (WPARAM)0, (LPARAM)"");
    }

    void ToolBox::menu_help_(HWND hwnd)
    {
        if (!hwnd)
        {
            hwnd = getchild(TabIndex::ITAB_HELP);
            if (!hwnd)
                return;
        }
        auto obj = m_help.get(AppHelp::HelpType::THELP_LINK);
        print_help_(obj, hwnd);
    }

    HWND ToolBox::setup_help_()
    {
        HWND hwnd = ::CreateWindowExW(
                        WS_EX_COMPOSITED | WS_EX_CLIENTEDGE,
                        MSFTEDIT_CLASS, nullptr,
                        WS_CHILD        |
                        WS_CLIPCHILDREN |
                        WS_VSCROLL      |
                        ES_MULTILINE    |
                        ES_SAVESEL      |
                        ES_READONLY,
                        0, 30, 1, 1,
                        m_hwnd,
                        (HMENU)(ID_TAB_IDX + static_cast<uint32_t>(TabIndex::ITAB_HELP)),
                        MDIWin::Config::instance().gethinstance(),
                        nullptr
                    );
        if (!hwnd)
            return nullptr;

        ::SendMessage(hwnd, WM_SETFONT,       (WPARAM)MDIWin::Config::instance().getfont(), (LPARAM)1);
        ::SendMessage(hwnd, EM_FMTLINES,      (WPARAM)1,   (LPARAM)0);
        ::SendMessage(hwnd, EM_AUTOURLDETECT, (WPARAM)1,   (LPARAM)0);
        ::SendMessage(hwnd, EM_SETMARGINS,    (WPARAM)EC_LEFTMARGIN, (LPARAM)12);
        ::SendMessage(hwnd, EM_SETEVENTMASK, 0, ::SendMessage(hwnd, EM_GETEVENTMASK, 0, 0) | ENM_LINK);
        //
        menu_help_(hwnd);
        auto obj = m_help.get(AppHelp::HelpType::THELP_APP);
        print_help_(obj, hwnd);
        //
        return hwnd;
    }

    bool ToolBox::find_help_(std::string const & s, HWND hwnd)
    {
        if (s.empty())
            return false;

        if (!hwnd)
        {
            hwnd = getchild(TabIndex::ITAB_HELP);
            if (!hwnd)
                return false;
        }

        auto obj = m_help.get(s);
        if (std::get<1>(obj) == 0)
            return false;

        clear_help_(hwnd);
        menu_help_(hwnd);
        print_help_(obj, hwnd);
        return true;
    }

    bool ToolBox::event_help(HWND hwnd, LPARAM lp_, std::string&)
    {
        do
        {
            if (!hwnd)
                break;

            ENLINK *el = reinterpret_cast<ENLINK*>(lp_);
            if (!el)
                break;

            std::wstring ws;
            std::vector<wchar_t> vs(1024);
            ::SendMessage(hwnd, EM_EXSETSEL,    0, reinterpret_cast<LPARAM>(&el->chrg));
            ::SendMessageW(hwnd, EM_GETSELTEXT, 0, reinterpret_cast<LPARAM>(&vs[0]));

            ws = &vs[0];
            if (ws.empty())
                break;

            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cnv;
            std::string s = cnv.to_bytes(ws.c_str());
            if (s.empty())
                break;
            /*
                // Debug:
                ALERTBox(s);
             */
            AppHelp::HelpType t = m_help.getid(s);
            if (t == AppHelp::HelpType::THELP_NONE)
                break;
            else if (t == AppHelp::HelpType::THELP_URL)
            {
                std::stringstream ss;
                ss << l_str_ourl << s.c_str();
                INFOBox(ss.str());
                ::ShellExecuteA(
                        MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
                        0,
                        s.c_str(),
                        0, 0,
                        SW_SHOW
                    );
                break;
            }
            //
            {
                auto obj = m_help.get(t);
                if (std::get<1>(obj) == 0)
                    break;

                clear_help_(hwnd);
                menu_help_(hwnd);
                print_help_(obj, hwnd);
            }
            return true;
        }
        while (0);
        return false;
    }

};
