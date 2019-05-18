
#include "DriverInternal.h"
#include "DriverConst.h"
#include "Dialog/WINAPI/StyleDialog.h"
#include "Dialog/WINAPI/AdbMgrDialog/AdbMgrDialog.h"
#include "Dialog/WINAPI/AdbListDialog/AdbListDialog.h"
#include "../../ADBDriverPackDLL/src/PackManager.h"

using namespace std::placeholders;

namespace GameDev
{

std::string ADBDriver::GetProperties(ADBDriver::DeviceInfoType t)
{
    std::string result;
    std::stringstream ss;
    ss << "getprop " << DriverConst::ls_arrayPropertis[t];

    if (AdbRawT<std::string>(ss.str(), DriverConst::ls_cmd_shell, result))
        return result;
    return std::string{};
}

SelectedList ADBDriver::GetDeviceList()
{
    std::wstring response;
    SelectedList devices;

    try
    {
        if (
            (!AdbRawT<std::wstring>(L"devices-l", DriverConst::lw_cmd_host, response)) ||
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
                    wstring_trim(wsline.substr(0, pos)),
                    wstring_trim(wsline.substr(pos))
                )
            );
        else
            devices.insert(
                std::make_pair(
                    wstring_trim(wsline),
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

        if (!AdbRawT<std::string>(DriverConst::ls_checkFile, DriverConst::ls_cmd_shell, result))
            break;
        if (string_end<std::string>(result, DriverConst::ls_foundFile))
            return true;
        if (!string_end<std::string>(result, DriverConst::ls_errorFile))
            break;

        std::string wabi = GetProperties(ADBDriver::DeviceInfoType::DI_CPU_LIST);
        std::string wsdk = GetProperties(ADBDriver::DeviceInfoType::DI_VERSION_SDK);

        if (!Resources::PackManager::checkbin(wabi, wsdk))
            break;

        std::stringstream ss;
        ss << DriverConst::ls_path_file << DriverConst::ls_name_ascreencap << DriverConst::ls_mode_file;
        if (!SendFile(DriverConst::ls_src_ascreencap, ss.str()))
            break;

        ss.str(std::string());
        ss << DriverConst::ls_cmod_file << DriverConst::ls_name_ascreencap;
        if (!AdbRawT<std::string>(ss.str(), DriverConst::ls_cmd_shell, result))
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
        if (::_access(fpath.c_str(), F_OK) < 0)
            break;

        size_t pos;
        std::string fname;
        std::stringstream ss{};

        ss << DriverConst::ls_path_file;
        if ((pos = fpath.find_last_of("/\\")) == std::string::npos)
            fname = fpath.c_str();
        else
            fname = fpath.substr((pos + 1), (fpath.length() - 1)).c_str();

        ss << fname.c_str() << DriverConst::ls_mode_file;
        if (!SendFile(fpath, ss.str()))
            break;

        ss.str(std::string());
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
