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

namespace Editor
{
    //
    LRESULT CALLBACK MenuBar::MenuWndProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, UINT_PTR, DWORD_PTR ptr)
    {
        switch (umsg_)
        {
            case WM_COMMAND:
                {
                    HWND hwnd = reinterpret_cast<HWND>(ptr);
                    if (hwnd)
                        (void) ::SendMessage(hwnd, umsg_, wp_, lp_);
                    return 0;
                }
        }
        return ::DefSubclassProc(hwnd_, umsg_, wp_, lp_);
    }

    ///

    MenuBar::~MenuBar()
    {
        if (WinBase::m_hwnd)
            ::RemoveWindowSubclass(WinBase::m_hwnd, &MenuBar::MenuWndProc, 0);
    }

    bool MenuBar::init(HWND hwnd_, HWND phwnd_)
    {
        do
        {
            std::string s = MDIWin::Config::instance().appname;
            s.append("ClassMenuControl");

            if (!MDIWin::BaseData::regclass(
                    s,
                    &MDIWin::MDIApp::mdiChildRedirect,
                    MDIWin::Config::instance().getbrush(MDIWin::Config::StyleBrush::BRUSH_GRAY),
                    nullptr,
                    ID_REBAR_MENU
                ))
                break;

            if (!WinBase::create(
                    hwnd_,
                    s,
                    WS_OVERLAPPED       |
                    WS_CLIPCHILDREN     |
                    WS_BORDER
                ))
                break;
            //
            ::SetWindowSubclass(
                    WinBase::m_hwnd,
                    &MenuBar::MenuWndProc,
                    0,
                    reinterpret_cast<DWORD_PTR>(phwnd_)
                );
            return true;
        }
        while (0);
        return false;
    }

    void MenuBar::setenable(uint32_t cmd, bool isenable)
    {
        HMENU hm = ::GetMenu(WinBase::m_hwnd);
        if (!hm)
            return;

        uint32_t flags = MF_BYCOMMAND;
        if (isenable)
            flags |= MF_ENABLED;
        else
            flags |= MF_DISABLED | MF_GRAYED;

        ::EnableMenuItem(hm, cmd, flags);
        ::DrawMenuBar(WinBase::m_hwnd);
    }

    void MenuBar::setcheck(uint32_t cmd, bool ischeck)
    {
        HMENU hm = ::GetMenu(WinBase::m_hwnd);
        if (!hm)
            return;

        uint32_t flags = MF_BYCOMMAND | MF_ENABLED;
        if (ischeck)
            flags |= MF_CHECKED;
        else
            flags |= MF_UNCHECKED;

        ::CheckMenuItem(hm, cmd, flags);
        ::DrawMenuBar(WinBase::m_hwnd);
    }

    void MenuBar::add(MainMenuId id, uint32_t off, std::vector<MDIWin::BaseData::dataMap> & v)
    {
        do
        {
            if (!WinBase::m_hwnd)
                break;

            HMENU hm = ::GetMenu(WinBase::m_hwnd);
            if (!hm)
                break;

            MENUITEMINFO mf{};
            mf.cbSize = sizeof(mf);
            mf.fMask = MIIM_ID | MIIM_SUBMENU;

            if (!::GetMenuItemInfo(hm, id, 1, &mf))
                break;
            if (!mf.hSubMenu)
                break;

            int32_t ipos = ::GetMenuItemCount(mf.hSubMenu);
            if (ipos > 0)
            {
                uint32_t i, pos = static_cast<uint32_t>(ipos);
                for (i = pos; i >= off; i--)
                    ::DeleteMenu(mf.hSubMenu, i, MF_BYPOSITION);
            }

            uint32_t i = off;
            for (auto & item : v)
            {
                std::string s = std::to_string(i - off + 1);
                s += ". - ";
                s += std::get<3>(item).c_str();

                ::InsertMenuA(
                    mf.hSubMenu,
                    i++,
                    MF_BYPOSITION | MF_STRING,
                    std::get<0>(item),
                    s.c_str()
                );
            }
            ::DrawMenuBar(WinBase::m_hwnd);
        }
        while (0);
    }
};
