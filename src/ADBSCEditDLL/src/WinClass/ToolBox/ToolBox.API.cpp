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

namespace Editor
{
    struct ApiItems
    {
        char const *title;
        char const *desc;
    };
    static const char *l_string_type[] =
    {
        "int",
        "bool",
        "double",
        "string",
        "{table}"
    };
    //
    static ApiItems l_items[] =
    {
        { "Plugin API - LuaObject:", nullptr },
#       define LFUN__(S,A,H1,H2,...) { H1, H2 },
#       include <LuaPluginFunction.h>
    };
    //
    static HTREEITEM f_tvadd(HWND hwnd, HTREEITEM hti, std::string const & s, bool isdesc = false)
    {
        TV_INSERTSTRUCT tvi{};
        tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

        if (hti == TVI_ROOT)
        {
            tvi.hParent = nullptr;
            tvi.hInsertAfter = TVI_ROOT;
            tvi.item.iImage = 0;
            tvi.item.iSelectedImage = 1;
        }
        else
        {
            tvi.hParent = hti;
            tvi.hInsertAfter = TVI_LAST;
            tvi.item.iImage = ((isdesc) ? 5 : 3);
            tvi.item.iSelectedImage = ((isdesc) ? 4 : 2);
            if (isdesc)
                tvi.item.mask |= TVIF_CHILDREN;
        }
        tvi.item.pszText = const_cast<LPSTR>(s.c_str());
        return (HTREEITEM)::SendMessage(hwnd, TVM_INSERTITEM, 0, (LPARAM)&tvi);
    }

    HWND ToolBox::setup_api_()
    {
        HWND hwnd = ::CreateWindowEx(
                        WS_EX_COMPOSITED | WS_EX_CLIENTEDGE,
                        WC_TREEVIEWA, nullptr,
                        WS_CHILD            |
                        WS_CLIPCHILDREN     |
                        WS_VSCROLL          |
                        TVS_HASLINES        |
                        TVS_NOTOOLTIPS      |
                        TVS_SHOWSELALWAYS,
                        0, 30, 1, 1,
                        m_hwnd,
                        (HMENU)(ID_TAB_IDX + static_cast<uint32_t>(TabIndex::ITAB_API)),
                        MDIWin::Config::instance().gethinstance(),
                        nullptr
                    );
        if (!hwnd)
            return nullptr;

        ::SendMessage(hwnd, WM_SETFONT, (WPARAM)MDIWin::Config::instance().getfont(), (LPARAM)1);
        //
        ::SendMessage(hwnd, TVM_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(
                MDIWin::Config::instance().getiml(MDIWin::Config::ImageButtonList::IMLBTN_TOOLAPI_CTRL)
        ));

        HTREEITEM hroot = nullptr;
        HTREEITEM hparent = nullptr;

        for (uint32_t i = 0; i < __NELE(l_items); i++)
        {
            ApiItems & itm = l_items[i];
            if (!i)
            {
                hroot = f_tvadd(hwnd, TVI_ROOT, itm.title);
                continue;
            }
            if ((itm.title) && (hroot))
                hparent = f_tvadd(hwnd, hroot, itm.title);

            if ((itm.desc) && (hparent))
                (void) f_tvadd(hwnd, hparent, itm.desc, true);
        }
        //
        if (hroot)
            ::SendMessage(hwnd, TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hroot);
        return hwnd;
    }

    bool ToolBox::event_api(HWND hwnd, LPARAM lp, std::string & s)
    {
        s = "";
        //
        do
        {
            if (!hwnd)
                break;

            LPNMTREEVIEW lpnm = reinterpret_cast<LPNMTREEVIEW>(lp);
            if (!lpnm)
                break;

            std::vector<char> v(2048);
            TVITEM tvi{};
            tvi.mask = TVIF_TEXT;
            tvi.hItem = lpnm->itemNew.hItem;
            tvi.cchTextMax = v.size();
            tvi.pszText = &v[0];

            if (!::SendMessage(
                        m_tab[TabIndex::ITAB_API],
                        TVM_GETITEM,
                        TVGN_DROPHILITE,
                        reinterpret_cast<LPARAM>(&tvi)
                ))
                break;

            std::string txt = &v[0];
            std::size_t sp;
            //
            if ((sp = txt.find_first_of(" ")) != std::string::npos)
            {
                if (txt.at(0) == '[')
                    for (uint32_t i = 0; i < __NELE(l_string_type); i++)
                        if (txt.compare(1, (sp - 2), l_string_type[i]) == 0)
                        {
                            s += "local var = ";
                            break;
                        }
                s += txt.substr(sp + 1);
            }
            else
            {
                s  = "-- ";
                s += txt.c_str();
            }
            return true;
        }
        while (0);
        return false;
    }
};
