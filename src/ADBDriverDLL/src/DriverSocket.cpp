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
#include "DriverConst.h"

namespace GameDev
{

bool ADBDriver::AdbCheck()
{
    return m_net.Check();
}

//#include "DriverSocket/DriverSocketMake.cpp"
//#include "DriverSocket/DriverSocketRawCmd.cpp"
//#include "DriverSocket/DriverSocketCapture.cpp"

template<typename T>
bool DLL_EXPORT ADBDriver::AdbRawT(T cmd, T const & addtype, T & result, DriverConst::ClearType type)
    {
        SOCKET l_client = INVALID_SOCKET;
        result.clear();

        do
        {
            if ((l_client = m_net.Connect()) == INVALID_SOCKET)
                break;

            if (!m_net.SyncTargetSend(l_client, DeviceId.Get<std::wstring>()))
                break;

            cmd.insert(0, addtype.c_str());

            if (!m_net.SyncStringReceive<T>(l_client, cmd, result))
                break;

            if (result.empty())
                break;

            switch (type)
            {
                case DriverConst::ClearType::CLEARTYPE_NONE:
                    {
                        break;
                    }
                case DriverConst::ClearType::CLEARTYPE_4:
                    {
                        if (result.length() >= 4)
                            result.erase(0, 4);
                        break;
                    }
                case DriverConst::ClearType::CLEARTYPE_8:
                    {
                        if (result.length() >= 8)
                            result.erase(0, 8);
                        break;
                    }
            }
            m_net.Close(l_client);
            return true;
        }
        while (0);

        m_net.Close(l_client);
        return false;
    }
    template DLL_EXPORT bool ADBDriver::AdbRawT<std::wstring>(std::wstring, std::wstring const &, std::wstring&, DriverConst::ClearType);
    template DLL_EXPORT bool ADBDriver::AdbRawT<std::string> (std::string,  std::string  const &, std::string&,  DriverConst::ClearType);

}
