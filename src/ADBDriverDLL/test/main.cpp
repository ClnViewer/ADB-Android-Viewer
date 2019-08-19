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

#include "../../ADBViewer/src/loader.h"
#include "../src/ADBDriver.h"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main()
{
    GameDev::ADBDriver m_adb;

    /*
    std::wstring response;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_CPU_LIST);
    std::wcout << L"GetProperties: " << response << std::endl;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_VERSION_RELEASE);
    std::wcout << L"GetProperties: " << response << std::endl;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_VERSION_SDK);
    std::wcout << L"GetProperties: " << response << std::endl;
    */

    //m_adb.InitRemote();
    bool ret;
    std::string sr;

    /*
    ret = m_adb.GetDeviceSetupUI();
    std::cout << "GetDeviceSetupUI: " << ret << std::endl;
    */

    /*
    ret = m_adb.GetDeviceListUI();
    std::cout << "GetDeviceListUI: " << ret << std::endl;
    */

    /*
    ret = m_adb.GetDeviceListUI();
    std::wstring wdid = m_adb.DeviceId.GetID<std::wstring>();
    std::string  sdid = m_adb.DeviceId.GetID<std::string>();
    std::wcout << L"GetDeviceListUI: " << ret;
    std::wcout << L" - " << wdid.c_str();
    std::wcout << L" - " << sdid.c_str();
    std::wcout << L" -*- " << std::endl;
    */

    /*
    std::cout << m_adb.GetDeviceInfo() << std::endl;
    std::cout << m_adb.GetDriverInfo() << std::endl;
    std::cout << m_adb.GetProperties(GameDev::DriverConst::DeviceInfoType::DI_CPU_LIST) << std::endl;
    */

    /*
    /// NOT print
    //ret = m_adb.SendTextT<std::string>("и утф, текст! \"(тест)\"", true);
    /// OK print
    ret = m_adb.SendTextT<std::string>("Test, Send Text \"(option)\".", true);
    std::cout << "Result (SendTextT): " << ret << std::endl;
    if (ret) /// async thread wait
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    */

    /*
    ret = m_adb.SendToShell("ls -l", sr, false, GameDev::DriverConst::ClearType::CLEARTYPE_NONE);
    std::cout << "Result (ls -l): " << ret << std::endl;
    std::cout << sr.c_str() << std::endl;
    */

    std::vector<GameDev::ADBDriver::DirItem> vdi;
    ret = m_adb.DirList("/data/local/tmp", vdi, sr);
    //ret = m_adb.DirList("/", vdi, sr);
    std::cout << "Result (ListDir): " << ret << std::endl;

    if (!sr.empty())
        std::cout << "Message (ListDir): " << sr.c_str() << std::endl << std::endl;

    for (auto & di : vdi)
    {
        std::cout << di.s.c_str() << "\n\t: "
            << di.desc.c_str() << "\n\t\t: "
                << di.cmds.c_str() << "\n\t\t\t: "
                    << di.type << " : " << di.pmode << std::endl;
    }
    return 0;
}
