
#include "../../ADBViewer/src/loader.h"
#include "../src/ADBDriver.h"

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
    ret = m_adb.GetDeviceSetupUI();
    std::wcout << L"GetDeviceSetupUI: " << ret << std::endl;
    ret = m_adb.GetDeviceListUI();
    std::wcout << L"GetDeviceListUI: " << ret << L" - " << m_adb.GetDeviceID() << L" - " << m_adb.IsDeviceID() << std::endl;

    return 0;
}
