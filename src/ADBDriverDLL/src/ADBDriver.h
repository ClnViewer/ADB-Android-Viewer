
#pragma once

#include "DriverExcept.h"
#include "DriverAsync.h"
#include "DriverNet.h"

namespace GameDev
{

typedef std::map<std::wstring, std::wstring> SelectedList;

class DLL_EXPORT ADBDriver
{
private:
    uint32_t             m_display[4];
    std::wstring         m_exepath;
    std::wstring         m_deviceid;
    GameDev::DriverAsync m_cmdasync;
    GameDev::DriverNet   m_net;

public:

    enum DeviceInfoType
    {
        DI_CPU_LIST,
        DI_VERSION_SDK,
        DI_VERSION_RELEASE
    };

    enum KeysType
    {
        KEYS_ANDROID,
        KEYS_SDL,
        KEYS_WINAPI
    };

    enum CapType
    {
        CAP_RAW,
        CAP_PNG,
        CAP_ACAP_BMP,
        CAP_ACAP_STREAM
    };

    struct Tap_s
    {
        int32_t x;
        int32_t y;
    };

    struct Swipe_s
    {
        int32_t x0;
        int32_t y0;
        int32_t x1;
        int32_t y1;
        int32_t t;
    };

    ADBDriver();
    ADBDriver(std::wstring&);

    uint32_t GetDisplayWidth() const;
    uint32_t GetDisplayHeight() const;
    uint32_t GetDisplayType() const;
    uint32_t GetDisplaySize() const;
    bool     IsDeviceID() const;
    void     SetDeviceID(const std::wstring&);
    void     SetAdbBin(const std::wstring&);

    const wchar_t * GetDeviceID() const;
    const wchar_t * GetAdbBin() const;


    bool         AdbCheck();
    std::wstring AdbStart();
    std::wstring AdbStop();
    std::wstring Manager(const std::wstring&);
    bool         InitRemote();

    template <typename T> bool AdbRawT(T, T const&, T&);

    void Click(ADBDriver::Tap_s*);
    void Click(ADBDriver::Swipe_s*);
    void ClickDbl(ADBDriver::Tap_s*);
    void SendTextASCII(std::wstring const &);
    void SendTextASCII(std::string const &);
    void SendSpecialKey(ADBDriver::KeysType, int32_t);

    bool InstallApk(std::string const&);
    bool SendFile(std::string const&, std::string const&);

    SelectedList GetDeviceList();
    bool         GetDeviceListUI();
    bool         GetDeviceSetupUI();
    std::string  GetProperties(ADBDriver::DeviceInfoType);

    std::vector<uint8_t> Capture();
    std::vector<uint8_t> Capture(CapType, uint32_t, uint32_t);
    bool Video(uint32_t, uint32_t, std::function<bool(std::vector<uint8_t>&, uint32_t, uint32_t)>);

};

extern template bool ADBDriver::AdbRawT<std::wstring>(std::wstring, std::wstring const &, std::wstring&);
extern template bool ADBDriver::AdbRawT<std::string>(std::string, std::string const &, std::string&);

}
