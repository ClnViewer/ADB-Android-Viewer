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

namespace GameDev
{
    ExceptionPrn::except_prn_cb ExceptionPrn::m_prn =
        [](std::string const & s, COLORREF const&)
        {
            (void) ::MessageBoxA(
                        nullptr,
                        s.c_str(),
                        ExceptionPrn::m_msg,
                        MB_OK | MB_ICONWARNING
                    );
        };
    COLORREF const ExceptionPrn::m_color = RGB(250,0,0);

    ///

    void ExceptionPrn::init(ExceptionPrn::except_prn_cb fun)
    {
        ExceptionPrn::m_prn = fun;
    }

    void ExceptionPrn::parse(std::exception_ptr pe, std::string const & sd)
    {
        std::stringstream ss;

        try
        {
            if (pe)
            {
                std::string extype = pe.__cxa_exception_type()->name();
                if (extype.empty())
                    ss << ExceptionPrn::m_msg;
                else
                    ss << extype.c_str() << ": ";
                //
                std::rethrow_exception(pe);
            }
        }
        catch(std::exception const & e) { ss << e.what(); }
        catch(std::string const & s)    { ss << s.c_str(); }
        catch(const char *s)            { ss << s; }
        catch(intmax_t i)               { ss << i; }
        catch(uintmax_t u)              { ss << u; }
        catch(...)
        {
            if (sd.empty())
                ss << ExceptionPrn::m_unk;
            else
                ss << sd.c_str();
        }
        //
        if (ss.str().empty())
            ExceptionPrn::m_prn(ExceptionPrn::m_empty, ExceptionPrn::m_color);
        else
            ExceptionPrn::m_prn(ss.str(), ExceptionPrn::m_color);
    }

};
