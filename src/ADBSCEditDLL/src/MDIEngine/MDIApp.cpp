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

#include "../SCEditInternal.h"
#include <windowsx.h>
#include <ole2.h>

namespace MDIWin
{
    uint32_t MDIApp::m_dragid = 0U;
    //
    MDIApp::MDIApp()
    {
        Config::instance().setclass<Config::HandleType::CLASS_APP>(this);
        MDIApp::m_dragid = ::RegisterWindowMessage(DRAGLISTMSGSTRING);
    }
    MDIApp::~MDIApp()
    {
        ::RevokeDragDrop(Config::instance().gethandle<Config::HandleType::HWND_MAIN>());
        Config::instance().setclass<Config::HandleType::CLASS_APP>(nullptr);
    }
    //
    bool MDIApp::mdirun()
    {
        return mdirun(Config::instance().appname, nullptr);
    }
    bool MDIApp::mdirun(std::string const & classname, HINSTANCE hinst)
    {
        do
        {
            if (hinst)
                Config::instance().init(hinst);
            if (!m_mgr.init(
                classname,
                reinterpret_cast<void*>(this)
                ))
                break;

            HWND hwnd = Config::instance().gethandle<Config::HandleType::HWND_MAIN>();
            if (!hwnd)
                break;

            ::ShowWindow(hwnd, SW_SHOWNORMAL);
            ::UpdateWindow(hwnd);
            //
            if (m_call_after_init.size())
                for (auto id : m_call_after_init)
                    ::SendMessageA(hwnd, WM_COMMAND, (WPARAM)id, reinterpret_cast<LPARAM>(hwnd));

            return true;
        }
        while (0);
        return false;
    }

    //

    void MDIApp::mdiloop(uint32_t aid)
    {
        HACCEL haccel = nullptr;
        MSG msg{};

        if (aid)
            haccel = ::LoadAccelerators(
                        Config::instance().gethinstance(),
                        MAKEINTRESOURCE(aid)
                    );

        while (::GetMessage(&msg, nullptr, 0, 0) > 0)
        {
            if ((Config::instance().gethandle<Config::HandleType::HWND_CLIENT>()) &&
                (::TranslateMDISysAccel(
                    Config::instance().gethandle<Config::HandleType::HWND_CLIENT>(),
                    &msg))
                )
                continue;
            if ((haccel) &&
                (::TranslateAccelerator(
                    Config::instance().gethandle<Config::HandleType::HWND_MAIN>(),
                    haccel,
                    &msg))
                )
                continue;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        if (haccel)
            ::DestroyAcceleratorTable(haccel);
    }

    /// override

    LRESULT MDIApp::mdiFrameProc(HWND hwnd_, HWND fhwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, bool b)
    {
        if (b)
            return 0;
        else
            return ::DefFrameProc(hwnd_, fhwnd_, umsg_, wp_, lp_);
    }
    LRESULT MDIApp::mdiChildProc(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_, bool b)
    {
        if (b)
            return 0;
        else
            return ::DefMDIChildProc(hwnd_, umsg_, wp_, lp_);
    }

    /// redirect

    LRESULT CALLBACK MDIApp::mdiFrameRedirect(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        MDIApp *app = Config::instance()
                        .getclass<MDIWin::MDIApp, Config::HandleType::CLASS_APP>();
        if (app)
            return app->mdiFrameProc_(
                            hwnd_,
                            Config::instance().gethandle<Config::HandleType::HWND_CLIENT>(),
                            umsg_,
                            wp_,
                            lp_
                );
        return ::DefFrameProc(
                            hwnd_,
                            Config::instance().gethandle<Config::HandleType::HWND_CLIENT>(),
                            umsg_,
                            wp_,
                            lp_
                );
    }
    LRESULT CALLBACK MDIApp::mdiChildRedirect(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        MDIApp *app = Config::instance()
                        .getclass<MDIWin::MDIApp, Config::HandleType::CLASS_APP>();
        if (app)
            return app->mdiChildProc_(hwnd_, umsg_, wp_, lp_);
        return ::DefMDIChildProc(hwnd_, umsg_, wp_, lp_);
    }

    /// local proxy call

    LRESULT MDIApp::mdiFrameProc_(HWND hwnd_, HWND fhwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        bool b = false;
        switch (umsg_)
        {
            case WM_GETMINMAXINFO:
                {
                    MINMAXINFO *minMax = reinterpret_cast<MINMAXINFO*>(lp_);
                    minMax->ptMinTrackSize.x = 100;
                    minMax->ptMinTrackSize.y = 100;
                    b = true;
                    break;
                }
            case WM_CLOSE:
                {
                    ::PostQuitMessage(0);
                    b = true;
                    break;
                }
            case WM_CREATE:
                {
                    do
                    {
                        HWND hwnd = Config::instance().gethandle<Config::HandleType::HWND_CLIENT>();
                        if (hwnd)
                            break;
                        //
                        {
                            auto a = m_mgr.add<Editor::ToolBar>(hwnd_);
                            if (!a)
                                break;

                            hwnd = a->gethandle();
                            if ((!hwnd) || (!::IsWindow(hwnd)))
                                break;
                            Config::instance().sethandle<Config::HandleType::HWND_TOOLBAR>(hwnd);
                        }
                        //
                        {
                            auto a = m_mgr.add<MDIWin::ClientWin>(hwnd_);
                            if (!a)
                                break;
                            hwnd = a->gethandle();
                            if ((!hwnd) || (!::IsWindow(hwnd)))
                                break;
                            Config::instance().sethandle<Config::HandleType::HWND_CLIENT>(hwnd);
                        }
                        //
                        {
                            auto a = m_mgr.add<Editor::EditBox>();
                            if ((!a) || (!a->gethandle()))
                                break;
                        }
                        //
                        {
                            auto a = m_mgr.add<Editor::InfoBox>();
                            if ((!a) || (!a->gethandle()))
                                break;
                        }
                        b = true;
                    }
                    while (0);
                    break;
                }
            case WM_NOTIFY:
                {
                    if (!lp_)
                        break;

                    try
                    {
                        LPNMHDR lph = reinterpret_cast<LPNMHDR>(lp_);
                        if ((lph->idFrom == ID_REBAR) && (lph->code == RBN_HEIGHTCHANGE))
                        {
                            m_mgr.setsize();
                            return 1;
                        }
                        /* TODO:
                        else if ((lph->idFrom == ID_REBAR) && (lph->code == NM_NCHITTEST))
                        {
                            return HTTRANSPARENT;// HTCAPTION;
                        }
                        */
                    }
                    catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                    break;
                }
            default: break;
        }
        return mdiFrameProc(hwnd_, fhwnd_, umsg_, wp_, lp_, b);
    }

    LRESULT MDIApp::mdiChildProc_(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        bool b = false;
        switch (umsg_)
        {
            case WM_SIZING:
                {
                    try
                    {
                        long id = ::GetWindowLong(hwnd_, GWL_ID);
                        if ((id >= ID_TAB_IDX) && (id < (ID_TAB_IDX + 5)))
                            break;

                        auto a = m_mgr.find(hwnd_);
                        if (!a)
                            return 0;

                        if (a->ispined())
                            return 0;

                        a->setsize();
                        m_mgr.setsize();
                    }
                    catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                    b = true;
                    break;
                }
            case WM_MOVE:
                {
                    m_mgr.moved(hwnd_, LOWORD(lp_), HIWORD(lp_));
                    b = true;
                    break;
                }
            case WM_DESTROY:
                {
                    (void) m_mgr.closed(hwnd_);
                    b = true;
                    break;
                }
            case WM_NCHITTEST:
                {
                    try
                    {
                        auto a = m_mgr.find(hwnd_);
                        if (!a)
                            break;
                        if (a->istitlebar())
                        {
                            RECT rc{};
                            if (!::GetWindowRect(hwnd_, &rc))
                                break;

                            ImageLite::IPOINT<int32_t> ip(LOWORD(lp_), HIWORD(lp_));
                            ImageLite::IRECT<int32_t>  ir;
                            ir.convert(rc);
                            if (ir.empty())
                                break;
                            if (ir.contains(ip, 36))
                                return HTCAPTION;
                        }
                    }
                    catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                    break;
                }
            case WM_NOTIFY:
                {
                    if (!lp_)
                        break;

                    try
                    {
                        LPNMHDR lph = reinterpret_cast<LPNMHDR>(lp_);
                        /* EDITBOX Control notify */
                        if (lph->idFrom == ID_EDITBOX)
                        {
                            b = true;
                            auto a = m_mgr.find(
                                        static_cast<uint32_t>(
                                            MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_EDIT
                                        )
                                    );
                            if (!a)
                                break;

                            a->event(
                                    reinterpret_cast<void*>(lp_)
                                );
                            return 1;
                        }
                        /* TAB Control change */
                        else if ((lph->idFrom == ID_TAB_CTRL) && (lph->code == TCN_SELCHANGE))
                        {
                            auto a = m_mgr.find(hwnd_);
                            if (!a)
                                break;

                            a->show();
                            b = true;
                            break;
                        }
                        /* OLE DRAG & DROP from TreeView */
                        else if ((lph->idFrom == (ID_TAB_IDX + static_cast<int32_t>(Editor::ToolBox::TabIndex::ITAB_API))) &&
                                 (lph->code == TVN_BEGINDRAG))
                        {
                            do
                            {
                                auto a = m_mgr.find(hwnd_);
                                if (!a)
                                    break;
                                //
                                std::string s;
                                if (!a->event(lph->hwndFrom, lp_, s))
                                    break;
                                if (s.empty())
                                    break;
                                //
                                GameDev::OLEDropSource::BeginDrag(s);
                                return 1;
                            }
                            while (0);
                            return 0;
                        }
                    }
                    catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                    break;
                }
            case WM_COMMAND:
                {
                    switch (LOWORD(wp_))
                    {
                        case (TitleBar::HandleType::HT_DEFAULT_ID + TitleBar::HandleType::HT_BTN_ID_PIN):
                            {
                                try
                                {
                                    auto a = m_mgr.find(hwnd_);
                                    if (!a)
                                        return 0;

                                    a->setpined(!a->ispined());
                                }
                                catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                                return 0;
                            }
                        case (TitleBar::HandleType::HT_DEFAULT_ID + TitleBar::HandleType::HT_BTN_ID_DEFAULT):
                            {
                                try
                                {
                                    auto a = m_mgr.find(hwnd_);
                                    if (!a)
                                        return 0;

                                    if (a->ispined())
                                        return 0;

                                    a->setsizedefault();
                                    m_mgr.setsize();
                                }
                                catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
                                return 0;
                            }
                    }
                    break;
                }
            default: break;
        }
        return mdiChildProc(hwnd_, umsg_, wp_, lp_, b);
    }
};
