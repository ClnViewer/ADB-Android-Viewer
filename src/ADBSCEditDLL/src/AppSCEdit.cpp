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
    void SCEApp::initexcept()
    {
        GameDev::ExceptionPrn::init(
                [](std::string const & s, COLORREF const & c)
                {
                    MDIWin::Config::instance().colorBox(s, c);
                }
            );
    }
    void SCEApp::init(HINSTANCE hinst_, HICON hico_)
    {
        MDIWin::Config::instance().init(hinst_);
        MDIWin::Config::instance().appicon = hico_;
        initexcept();
    }
    void SCEApp::init(std::string const & path_, HINSTANCE hinst_, HICON hico_)
    {
        MDIWin::Config::instance().init(hinst_);
        MDIWin::Config::instance().appicon = hico_;
        MDIWin::Config::instance().savepath = path_;
        initexcept();
    }
    void SCEApp::init(std::string const & path_, std::string const & text_, HINSTANCE hinst_, HICON hico_)
    {
        MDIWin::Config::instance().init(hinst_);
        MDIWin::Config::instance().appicon  = hico_;
        MDIWin::Config::instance().savepath = path_;
        MDIWin::Config::instance().srctext  = text_;
        initexcept();
    }
    void SCEApp::appname(std::string const & s)
    {
        MDIWin::Config::instance().appname = s;
    }

    bool SCEApp::run(bool runtype)
    {
        try
        {
            std::thread thed
            {
                [=]()
                {
                    (void) ::OleInitialize(nullptr);
                    //
                    try         { run_(); }
                    catch (...) { ERRORBox(); }
                    //
                    (void) ::OleUninitialize();
                }
            };
            if (thed.joinable())
            {
                if (runtype)
                    thed.join();
                else
                    thed.detach();
            }
        }
        catch (...)
        {
            ERRORBox();
            return false;
        }
        return true;
    }

    void SCEApp::run_()
    {
        do
        {
            MDIApp::m_call_after_init =
            {
                IDM_BTN_SHOW_VERSION,
                IDM_EVENT_PLUGIN_INIT
            };
            //
            if (!MDIApp::mdirun())
                break;
            MDIApp::mdiloop(ID_ACCEL);
            return;
        }
        while (0);
        ERRORBox();
    }
};

