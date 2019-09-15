/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

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
#include <ctime>

namespace Editor
{
    static void f_time_print(std::stringstream & ss)
    {
        auto tt = std::time(nullptr);
        auto ttm = *std::localtime(&tt);
        ss << std::put_time(&ttm, "%H:%M:%S");
    }

    PrintBox::PrintBox()
    {
    }

    void PrintBox::init(Editor::InfoBox *winfo_)
    {
        if (winfo_)
            m_winfo = winfo_;
    }

    PrintBox& PrintBox::instance()
    {
        static PrintBox m_instance{};
        return m_instance;
    }

    Editor::InfoBox * PrintBox::getinfobox()
    {
        return m_winfo;
    }

    void PrintBox::colorBox(std::string const & msg, COLORREF const & color, std::string const & func, int32_t line)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        if (!func.empty())
        {
            ss << func.c_str();
            if (line != -1)
                ss << ":" << line;
            ss << " - ";
        }
        ss << msg.c_str();
        BoxT<COLORREF>(ss.str(), color);
    }

    void PrintBox::errorBox(std::string const & func, int32_t line, uint32_t err)
    {
        if (!err)
            err = ::GetLastError();

        if (err)
        {
            auto dbuf = std::make_unique<char[]>(1024);
            DWORD sz = ::FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                0, err, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                dbuf.get(), 1024, 0
            );
            std::stringstream ss;
            f_time_print(ss);
            ss << " ";
            ss << func.c_str() << ":" << line;
            if (sz)
                ss << " - " << dbuf.get();
            BoxT<InfoBox::TextType>(ss.str(), InfoBox::TextType::IB_TYPE_ERROR);
        }
    }

    void PrintBox::alertBox(std::string const & msg, std::string const & func, int32_t line)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        ss << func.c_str() << ":" << line << " - " << msg.c_str();
        BoxT<InfoBox::TextType>(ss.str(), InfoBox::TextType::IB_TYPE_ALERT);
    }

    void PrintBox::infoBox(std::string const & msg)
    {
        std::stringstream ss;
        f_time_print(ss);
        ss << " ";
        ss << msg.c_str();
        BoxT<InfoBox::TextType>(msg, InfoBox::TextType::IB_TYPE_INFO);
    }

    void PrintBox::defaultBox(std::string const & msg)
    {
        if (m_winfo)
            m_winfo->text(msg);
    }

    void PrintBox::clearBox()
    {
        if (m_winfo)
            m_winfo->text();
    }

    template <typename T>
    void PrintBox::BoxT(std::string const & msg, T const & val)
    {
        if (m_winfo)
            m_winfo->text(msg, val);
        else
            ::MessageBoxA(
                    Config::instance().gethandle(),
                    static_cast<LPCSTR>(msg.c_str()),
                    SCEdit::appName, MB_OK | MB_ICONERROR
                );
    }
    template void PrintBox::BoxT<InfoBox::TextType>(std::string const&, InfoBox::TextType const&);
    template void PrintBox::BoxT<COLORREF>(std::string const&, COLORREF const&);

};
