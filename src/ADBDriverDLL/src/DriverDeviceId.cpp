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

bool ADBDriverDeviceId::IsID() const
    {
        return !(m_deviceid.empty());
    }

void ADBDriverDeviceId::ClearID()
    {
        m_deviceid.clear();
    }

const std::wstring & ADBDriverDeviceId::operator = (const ADBDriverDeviceId & obj) const
    {
        return obj.m_deviceid;
    }

template<typename T>
void DLL_EXPORT ADBDriverDeviceId::SetID(T const & s)
    {
        if (s.empty())
            return;

        if constexpr (std::is_same<T, std::wstring>::value)
            m_deviceid = s;
        else if constexpr (std::is_same<T, std::string>::value)
            m_deviceid.assign(s.begin(), s.end());
    }
    template DLL_EXPORT void    ADBDriverDeviceId::SetID<std::wstring>(std::wstring const&);
    template DLL_EXPORT void    ADBDriverDeviceId::SetID<std::string> (std::string const&);

template<typename T>
T DLL_EXPORT ADBDriverDeviceId::GetID()
    {
        if constexpr (std::is_same<T, const wchar_t*>::value)
            return m_deviceid.c_str();
        else if constexpr (std::is_same<T, std::wstring>::value)
            return m_deviceid;
        else if constexpr (std::is_same<T, std::string>::value)
            return string_to_utf8(m_deviceid);
    }
    template DLL_EXPORT const wchar_t * ADBDriverDeviceId::GetID<const wchar_t*>();
    template DLL_EXPORT std::wstring    ADBDriverDeviceId::GetID<std::wstring>();
    template DLL_EXPORT std::string     ADBDriverDeviceId::GetID<std::string>();

}
