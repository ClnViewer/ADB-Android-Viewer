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

#include "DriverInternal.h"

#if defined(_BUILD_WARNING_0)
#  define ERR_SET0(A) data.assign(A)
#else
#  define ERR_SET0(A)
#endif

namespace GameDev
{

template<typename T>
bool DLL_EXPORT ADBDriver::ManagerT(T const & cmd, T & data)
    {
        data.clear();

#       if defined(OS_WIN)

        T exepath = AdbBin.Get<T>();
        if constexpr (std::is_same<T, std::wstring>::value)
        {
            if (exepath.empty())
            {
                ERR_SET0(DriverConst::lw_errorSet);
                return false;
            }

            if (::_waccess(exepath.c_str(), F_OK) < 0)
            {
                ERR_SET0(DriverConst::lw_errorBin);
                return false;
            }
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            if (exepath.empty())
            {
                ERR_SET0(DriverConst::ls_errorSet);
                return false;
            }

            if (::_access(exepath.c_str(), F_OK) < 0)
            {
                ERR_SET0(DriverConst::ls_errorBin);
                return false;
            }
        }

        BOOL ret;
        HANDLE hOutR, hOutW;
        SECURITY_ATTRIBUTES sa;
        PROCESS_INFORMATION pi;

        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&sa, sizeof(sa));

        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        if ((!::CreatePipe(&hOutR, &hOutW, &sa, 0U)) ||
            (!::SetHandleInformation(hOutR, HANDLE_FLAG_INHERIT, 0)))
        {
            if constexpr (std::is_same<T, std::wstring>::value)
                ERR_SET0(DriverConst::lw_errorPipe);
            else if constexpr (std::is_same<T, std::string>::value)
                ERR_SET0(DriverConst::ls_errorPipe);
            return false;
        }

        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        if constexpr (std::is_same<T, std::wstring>::value)
        {
            STARTUPINFOW si;
            ZeroMemory(&si, sizeof(si));

            si.cb = sizeof(si);
            si.hStdInput = NULL;
            si.hStdOutput = hOutW;
            si.hStdError = hOutW;
            si.dwFlags = STARTF_USESTDHANDLES;

            std::wstringstream adbcmd;
            adbcmd << exepath.c_str() << DriverConst::lw_punct_space << cmd.c_str();

            ret = ::CreateProcessW(
                   exepath.c_str(),
                   const_cast<wchar_t*>(adbcmd.str().c_str()),
                   NULL,
                   NULL,
                   TRUE,
                   DETACHED_PROCESS | CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW,
                   NULL,
                   NULL,
                   &si,
                   &pi
               );
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            STARTUPINFOA si;
            ZeroMemory(&si, sizeof(si));

            si.cb = sizeof(si);
            si.hStdInput = NULL;
            si.hStdOutput = hOutW;
            si.hStdError = hOutW;
            si.dwFlags = STARTF_USESTDHANDLES;

            std::stringstream adbcmd;
            adbcmd << exepath.c_str() << DriverConst::ls_punct_space << cmd.c_str();

            ret = ::CreateProcessA(
                   exepath.c_str(),
                   const_cast<char*>(adbcmd.str().c_str()),
                   NULL,
                   NULL,
                   TRUE,
                   DETACHED_PROCESS | CREATE_NO_WINDOW,
                   NULL,
                   NULL,
                   &si,
                   &pi
               );
        }

        ::CloseHandle(hOutW);

        do
        {
            if (!ret)
                break;

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            DWORD sz;
            auto buf = std::make_unique<char[]>(256);

            while ((::ReadFile(hOutR, buf.get(), 256U, &sz, NULL)) && (sz > 0U))
                data.append(buf.get(), buf.get() + sz);
        }
        while (0);

        ::CloseHandle(hOutR);

        if (!ret)
        {
            if constexpr (std::is_same<T, std::wstring>::value)
                ERR_SET0(DriverConst::lw_errorRun);
            else if constexpr (std::is_same<T, std::string>::value)
                ERR_SET0(DriverConst::ls_errorRun);
            return false;
        }

        return true;
        //
#       elif defined(_BUILD_WARNING_1)
        //
        if constexpr (std::is_same<T, std::wstring>::value)
            data.assign(DriverConst::lw_notSupport);
        else if constexpr (std::is_same<T, std::string>::value)
            data.assign(DriverConst::ls_notSupport);
        return false;
        //
#       else
        //
        return false;
        //
#       endif
    }
    template DLL_EXPORT bool ADBDriver::ManagerT<std::wstring>(std::wstring const&, std::wstring&);
    template DLL_EXPORT bool ADBDriver::ManagerT<std::string> (std::string  const&, std::string&);
}
