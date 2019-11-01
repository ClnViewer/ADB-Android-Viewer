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
#include "Scintilla.h"
#include <windowsx.h>
#include <LuaLint.h>

namespace Editor
{
    void SCEApp::_OnCommand(
                    [[maybe_unused]] HWND hwnd,
                    [[maybe_unused]] UINT,
                    [[maybe_unused]] WPARAM wParam,
                    [[maybe_unused]] LPARAM lParam)
    {
        int32_t id = LOWORD(wParam);
        try
        {
            auto tbc = m_mgr.find(
                static_cast<uint32_t>(
                    MDIWin::BaseData::MDIWinType::MWTYPE_TOOLBAR
                )
            );
            if (!tbc)
                return;

            auto ebc = MDIWin::Config::instance()
                                .getclass<Editor::EditBox, MDIWin::Config::HandleType::CLASS_EDIT>();
            if (!ebc)
                return;

#           pragma GCC diagnostic push
#           pragma GCC diagnostic ignored "-Wpedantic"
            switch (id)
            {
#               include "Command/cmd_MAIN_Command.cxx"
#               include "Command/cmd_LUAE_Command.cxx"
#               include "Command/cmd_EDITBOX_Control.cxx"
#               include "Command/cmd_TOOLBOX_Control.cxx"
                default: break;
            }
#           pragma GCC diagnostic pop
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
    }
};
