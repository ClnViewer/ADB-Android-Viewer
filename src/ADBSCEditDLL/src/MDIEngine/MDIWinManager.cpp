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

namespace MDIWin
{
    Manager::Manager()
    {
        m_count = static_cast<uint32_t>(BaseData::MDIWinType::MWTYPE_CHILD_OTHER);
        Config::instance().setclass<Config::HandleType::CLASS_MGR>(this);
    }

    Manager::~Manager()
    {
        Config::instance().setclass<Config::HandleType::CLASS_MGR>(nullptr);
    }

    uint32_t Manager::getwinid(HWND hw_)
    {
        do
        {
            if (!::IsWindow(hw_))
                break;

            void *v = (void*)::GetWindowLongPtr(hw_, GWLP_USERDATA);
            if (!v)
                break;

            uint32_t id = (uint32_t)v;
            if ((id > USHRT_MAX) ||
                (id < static_cast<uint32_t>(BaseData::MDIWinType::MWTYPE_CHILD)))
                break;

            return id;
        }
        while (0);
        return 0U;
    }

    uint32_t Manager::iswinid(HWND hw_, BaseData::MDIWinType t_)
    {
        uint32_t id = getwinid(hw_);
        if ((id) && (id == static_cast<uint32_t>(t_)))
            return id;
        return 0;
    }

    bool Manager::init(std::string const & s, void *obj)
    {
        m_class_main    = s;
        m_class_child   = s;
        m_class_control = s;
        m_class_main.append("ClassWFrame");
        m_class_child.append("ClassWChild");
        m_class_control.append("ClassTBControl");

        do
        {
            if (!BaseData::regclass(
                        m_class_main,
                        &MDIApp::mdiFrameRedirect,
                        Config::instance().getbrush(Config::BRUSH_GRAY)
                ))
                break;

            if (!BaseData::regclass(
                        m_class_child,
                        &MDIApp::mdiChildRedirect,
                        Config::instance().getbrush(Config::BRUSH_GRAY)
                ))
                break;

            if (!BaseData::regclass(
                        m_class_control,
                        TitleBar::wndControlRedirect,
                        Config::instance().getbrush(Config::BRUSH_GRAY)
                ))
                break;

            MDIWin::Base *a = add<Editor::MainWin>(nullptr, obj);
            if (!a)
                break;

            HWND hwnd = a->gethandle();
            if ((!hwnd) || (!::IsWindow(hwnd)))
                break;

            Config::instance().sethandle<Config::HandleType::HWND_MAIN>(hwnd);
            setsize();

            return true;
        }
        while (0);
        return false;
    }
};

