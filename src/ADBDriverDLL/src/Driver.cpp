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

namespace GameDev
{

std::wstring ADBDriver::AdbStart()
{
    return Manager(L"start-server");
}
std::wstring ADBDriver::AdbStop()
{
    return Manager(L"kill-server");
}

void ADBDriver::SetDeviceID(const std::wstring & ws)
{
    if (!ws.empty())
        m_deviceid.assign(ws.c_str());
}

const wchar_t * ADBDriver::GetDeviceID() const
{
    return m_deviceid.c_str();
}

bool ADBDriver::IsDeviceID() const
{
    return !(m_deviceid.empty());
}

void ADBDriver::SetAdbBin(const std::wstring & ws)
{
    if (!ws.empty())
       m_exepath.assign(ws.c_str());
}

const wchar_t * ADBDriver::GetAdbBin() const
{
    return m_exepath.c_str();
}

uint32_t ADBDriver::GetDisplayWidth()  const { return m_display[0]; }
uint32_t ADBDriver::GetDisplayHeight() const { return m_display[1]; }
uint32_t ADBDriver::GetDisplayType()   const { return m_display[2]; }
uint32_t ADBDriver::GetDisplaySize()   const { return m_display[3]; }

ADBDriver::ADBDriver(std::wstring & exepath)
    : m_display{0L,0L,0L}
{
    if (::_waccess(exepath.c_str(), F_OK) < 0)
        throw DriverExcept(GetLastError(), 0, __LINE__);

    m_exepath.assign(exepath);

    if (m_net.GetInitError())
        throw DriverExcept(GetLastError(), 0, __LINE__);
}

ADBDriver::ADBDriver()
    : m_display{0L,0L,0L}
{
    m_exepath.assign(L"adb.exe");

    if (m_net.GetInitError())
        throw DriverExcept(GetLastError(), 0, __LINE__);
}

}
