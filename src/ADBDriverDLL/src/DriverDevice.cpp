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
#include "Dialog/WINAPI/StyleDialog.h"
#include "Dialog/WINAPI/AdbMgrDialog/AdbMgrDialog.h"
#include "Dialog/WINAPI/AdbListDialog/AdbListDialog.h"
#include "../../ADBDriverPackDLL/src/PackManager.h"

using namespace std::placeholders;

namespace GameDev
{

void ADBDriver::deviceinfo()
    {
        if (m_device_abi.empty())
            m_device_abi = GetProperties(ADBDriver::DeviceInfoType::DI_CPU_LIST);
        if (m_device_sdk.empty())
            m_device_sdk = GetProperties(ADBDriver::DeviceInfoType::DI_VERSION_SDK);
        if (m_device_rel.empty())
            m_device_rel = GetProperties(ADBDriver::DeviceInfoType::DI_VERSION_RELEASE);
    }

std::string ADBDriver::GetProperties(ADBDriver::DeviceInfoType t)
{
    std::string result;
    std::stringstream ss;
    ss << "getprop " << DriverConst::ls_arrayPropertis[t];

    if (AdbRawT<std::string>(ss.str(), DriverConst::ls_cmd_shell, result, false))
    {
        string_trimn<std::string>(result);
        return result;
    }

    return std::string{};
}

std::string  ADBDriver::GetDeviceInfo()
    {
        deviceinfo();

        std::stringstream ss;
        ss << "Android " << m_device_rel;
        ss << " [" << m_device_sdk;
        ss << "] (" << m_device_abi << ")";
        return ss.str();
    }

std::string  ADBDriver::GetDriverInfo()
    {
        std::string result;
        do
        {
            if (!AdbRawT<std::string>(DriverConst::ls_driverInfo, DriverConst::ls_cmd_shell, result, false))
                break;
            if (result.empty())
                break;

            string_trimn<std::string>(result);
            std::stringstream ss;
            ss << "Driver: " << result;
            return ss.str();
        }
        while (0);

        result = "Driver unknown..";
        return result;
    }

SelectedList ADBDriver::GetDeviceList()
{
    std::wstring response;
    SelectedList devices;

    try
    {
        if (
            (!AdbRawT<std::wstring>(L"devices-l", DriverConst::lw_cmd_host, response, true)) ||
            (!response.length())
           )
            return devices;
    }
    catch (...)
    {
        return devices;
    }

    size_t pos;
    std::wstring wsline;
    std::wistringstream lstream(response);

    while (std::getline(lstream, wsline, L'\n'))
    {
        if ((pos = wsline.find(' ')) != std::wstring::npos)
            devices.insert(
                std::make_pair(
                    string_trimr<std::wstring>(wsline.substr(0, pos)),
                    string_trimr<std::wstring>(wsline.substr(pos))
                )
            );
        else
            devices.insert(
                std::make_pair(
                    string_trimr<std::wstring>(wsline),
                    L"-"
                )
            );
    }
    return devices;
}


bool ADBDriver::InitRemote()
{
    do
    {
        std::string result;

        if (!AdbRawT<std::string>(DriverConst::ls_checkFile, DriverConst::ls_cmd_shell, result, false))
            break;

        string_trimn<std::string>(result);

        if (string_end<std::string>(result, DriverConst::ls_foundFile))
            return true;
        if (!string_end<std::string>(result, DriverConst::ls_errorFile))
            break;

        deviceinfo();

        if (!Resources::PackManager::checkbin(m_device_abi, m_device_sdk, m_device_rel))
            break;

        if (!SendFile(DriverConst::ls_src_ascreencap, DriverConst::ls_path_dir, GameDev::ADBDriver::FilePermissionType::PERM_RWXRWXRWX))
            break;
        return true;
    }
    while (0);
    return false;
}

bool ADBDriver::InstallApk(std::string const & fpath)
{
    do
    {
        if (!SendFile(fpath, DriverConst::ls_path_dir, GameDev::ADBDriver::FilePermissionType::PERM_RWXRWXRWX))
            break;

        std::string fname = GameDev::filename<std::string>(fpath);
        std::stringstream ss;
        ss << DriverConst::ls_apk_install << fname.c_str();

        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
        return true;
    }
    while (0);

    return false;
}

bool ADBDriver::UnInstallApk(std::string const & name, std::string & sr)
{
    if (name.empty())
        return false;

    std::stringstream ss;
    ss << DriverConst::ls_apk_uninstall << " " << name.c_str();

    /*
    m_cmdasync.add<std::string>(
        ss.str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
    );
    */

    if (!AdbRawT<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            sr,
            false
        ))
        return false;

    if (sr.empty())
        return false;

    clearend<std::string>(sr);
    return true;
}

bool ADBDriver::GetDeviceListUI()
{
    try
    {
        SelectedList sl = GetDeviceList();

        if (!sl.size())
        {
            /// ADB start/stop DIALOG
            GameDev::AdbMgrDialog mgrd(
                        hmodule_get(),
                        IDD_ADBMGRDLG,
                        __STYLE_BLACK_TXT,
                        __STYLE_BLACK_BG
                    );
            if (mgrd.Show(this))
                sl = GetDeviceList();

            if (!sl.size())
                return false;
        }

        /// ADB select active device DIALOG
        GameDev::AdbListDialog ld(
                    hmodule_get(),
                    IDD_ADBLISTDLG,
                    __STYLE_BLACK_TXT,
                    __STYLE_BLACK_BG
            );

        int32_t id = static_cast<int32_t>(ld.Show(sl, 140));
        if (id >= 0)
        {
            m_deviceid = ld.SelectedKey();
            return (!m_deviceid.empty());
        }
        return false;
    }
    catch (...) { return false; }
}

bool ADBDriver::GetDeviceSetupUI()
{
    try
    {
        GameDev::AdbMgrDialog mgrd(
                    hmodule_get(),
                    IDD_ADBMGRDLG,
                    __STYLE_BLACK_TXT,
                    __STYLE_BLACK_BG
                );
        return mgrd.Show(this);
    }
    catch (...) { return false; }
}

}
