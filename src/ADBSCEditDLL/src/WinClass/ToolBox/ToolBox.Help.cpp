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
    struct HelpeItems
    {
        char const *title;
        char const *key;
        char const *desc;
        char const *info;
    };
    //
    static HelpeItems l_items[] =
    {
#       define HLP_LABEL(A)    { A, nullptr, nullptr, nullptr },
#       define HLP_ITEM(A,B,C) { nullptr, A, B, C },
#       include "../../SCEditHelpItems.h"
    };
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
            default:
                {
                    cf.dwMask = CFM_FACE;
                    cf.dwEffects = 0;
                    break;
                }
        }
        //
        ::SendMessage(hwnd, EM_SETSEL, -2, -1);
        ::SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }
    static void f_print(HWND hwnd, std::string const & s, uint32_t flags)
    {
        std::vector<char> vs(s.length() + 2);
        vs.assign(s.begin(), s.end());
        vs[s.length()] = '\n';
        vs[s.length() + 1] = '\0';

        f_styleText(hwnd, flags);

        SETTEXTEX se{};
        se.codepage = CP_ACP;
        se.flags    = ST_SELECTION;
        ::SendMessage(hwnd, EM_SETTEXTEX, (WPARAM)&se, (LPARAM)&vs[0]);
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
                        ES_AUTOVSCROLL  |
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

        for (uint32_t i = 0; i < __NELE(l_items); i++)
        {
            std::string s;
            HelpeItems & itm = l_items[i];
            if (itm.title)
            {
                s  = "\n";
                s += itm.title;
                s += "\n";
                f_print(hwnd, s, CFM_UNDERLINE);
                continue;
            }
            if (itm.key)
            {
                s = itm.key;
                f_print(hwnd, s, CFM_BOLD);
            }
            if (itm.desc)
            {
                s  = "   ";
                s += itm.desc;
                f_print(hwnd, s, CFM_SUBSCRIPT);
            }
            if (itm.info)
            {
                s  = "   ";
                s += itm.info;
                f_print(hwnd, s, CFM_SUBSCRIPT);
            }
        }
        return hwnd;
    }
};
