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

#include "SCEditInternal.h"
#include <commctrl.h>

namespace Editor
{
    struct RebarData
    {
        ImageLite::IPOINT<int32_t> ipoint;
        int32_t default_id, default_img_max, default_img;
        RebarData()
            : ipoint{0, 40},  default_id(ID_REBAR_BAND),
              default_img_max(0), default_img(0) {}
    };
    static RebarData rbd{};

    bool ReBar::init(HWND hwnd_)
    {
        do
        {
            if (!WinBase::create(
                    hwnd_,
                    REBARCLASSNAME,
                    WS_VISIBLE          |
                    WS_CHILD            |
                    WS_CLIPCHILDREN     |
                    WS_CLIPSIBLINGS     |
                    CCS_NODIVIDER       |
                    RBS_VARHEIGHT       |
                    RBS_BANDBORDERS,
                    (HMENU)ID_REBAR
                ))
                break;

            REBARINFO rbi{};
            rbi.cbSize = sizeof(rbi);
            rbi.fMask  = RBIM_IMAGELIST;
            rbi.himl   = MDIWin::Config::instance().getiml(MDIWin::Config::ImageButtonList::IMLBTN_REBAR_CTRL);

            if (!rbi.himl)
                break;
            if (!::SendMessage(WinBase::m_hwnd, RB_SETBARINFO, 0, reinterpret_cast<LPARAM>(&rbi)))
                break;

            rbd.default_img_max = static_cast<int32_t>(
                    MDIWin::Config::instance().getiml(MDIWin::Config::ImageButtonList::IMLBTN_REBAR_CTRL, rbd.ipoint)
                );
            return true;
        }
        while (0);
        return false;
    }

    int32_t ReBar::addband(HWND hwnd_, int32_t y_)
    {
        REBARBANDINFO rbi{};
        rbi.cbSize = sizeof(rbi);
        rbi.fMask =
            RBBIM_ID    |
            RBBIM_STYLE |
            RBBIM_SIZE  |
            RBBIM_IMAGE |
            RBBIM_CHILD |
            RBBIM_CHILDSIZE;
        rbi.fStyle =
            // RBBS_BREAK  |
            RBBS_CHILDEDGE |
            RBBS_TOPALIGN  |
            RBBS_GRIPPERALWAYS;

        rbd.ipoint.y   = ((y_) ? y_ : rbd.ipoint.y);
        rbi.hwndChild  = hwnd_;
        rbi.cyMinChild = rbd.ipoint.y;
        rbi.wID        = rbd.default_id;
        rbi.iImage     = rbd.default_img;

        if (rbd.default_img < (rbd.default_img_max - 1))
            rbd.default_img++;

        ::SendMessage(
                    WinBase::m_hwnd,
                    RB_INSERTBAND,
                    WPARAM(-1),
                    reinterpret_cast<LPARAM>(&rbi)
                );
        return rbd.default_id++;
    }

    void ReBar::showband(int32_t id_)
    {
        int32_t idx = ::SendMessage(WinBase::m_hwnd, RB_IDTOINDEX, WPARAM(ID_REBAR_BAND + id_), 0);
        if (idx != -1)
            ::SendMessage(WinBase::m_hwnd, RB_MAXIMIZEBAND, WPARAM(idx), 0);
    }

};
