
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
    std::string          m_device_abi;
    std::string          m_device_sdk;
    std::string          m_device_rel;

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

    enum FilePermissionType
    {
        PERM_RW = 0,
        PERM_RWX,
        PERM_RWRW,
        PERM_RWXRWX,
        PERM_RWRWRW,
        PERM_RWXRWXRWX,
        PERM_NONE
    };

    enum FileType
    {
        FILETYPE_NONE = 0,
        FILETYPE_DIR,
        FILETYPE_FILE,
        FILETYPE_SYMLINK,
        FILETYPE_ROOT,
        FILETYPE_BACK,
        FILETYPE_INFO,
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
    //
    struct DirItemColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    //
    struct DLL_EXPORT DirItem
    {
        std::string         s;
        std::string         cmds;
        std::string         desc;
        ADBDriver::FileType type;
        DirItemColor ctxt,  cbg;
        int32_t             permission;
        //
        DirItem();
        DirItem(
            void*,
            void*);
        DirItem(
            std::string const&,
            void*,
            void*);
        DirItem(
            std::string const&,
            std::string const&,
            std::string const&,
            ADBDriver::FileType,
            ADBDriver::DirItemColor const&);
        //
        void Description(
            std::string  const&,
            std::string  const&,
            std::string  const&,
            DirItemColor const&);
        //
        template<typename T>
        const T * ColorText() const
        {
            return reinterpret_cast<const T*>(&ctxt);
        }
        template<typename T>
        const T * ColorBg() const
        {
            return reinterpret_cast<const T*>(&cbg);
        }
    };

    ADBDriver();
    ADBDriver(std::wstring&);

    uint32_t     GetDisplayWidth() const;
    uint32_t     GetDisplayHeight() const;
    uint32_t     GetDisplayType() const;
    uint32_t     GetDisplaySize() const;
    bool         IsDeviceID() const;
    void         SetDeviceID(const std::wstring&);

    template <typename T> DLL_EXPORT T GetDeviceID();

    void            SetAdbBin(const std::wstring&);
    const wchar_t * GetAdbBin() const;

    bool         AdbCheck();
    std::wstring AdbStart();
    std::wstring AdbStop();
    std::wstring Manager(const std::wstring&);
    bool         InitRemote();

    template <typename T> bool AdbRawT(T, T const&, T&, bool);

    void Click(ADBDriver::Tap_s*);
    void Click(ADBDriver::Swipe_s*);
    void ClickDbl(ADBDriver::Tap_s*);
    void SendTextASCII(std::wstring const &);
    void SendTextASCII(std::string const &);
    void SendSpecialKey(ADBDriver::KeysType, int32_t);
    bool SendToShell(std::string const&, std::string&, bool = false);

    /// DriverFile
    bool ListDir(std::string const&, std::vector<ADBDriver::DirItem>&, std::string &);
    bool SendFile(std::string const&, std::string const&, GameDev::ADBDriver::FilePermissionType);
    bool ChmodFile(std::string const&, GameDev::ADBDriver::FilePermissionType, std::string&);
    std::string PermissionFile(GameDev::ADBDriver::FilePermissionType);
    std::string SizeFile(uintmax_t);
    std::string SizeFile(std::string const&);

    /// DriverDevice
    bool InstallApk(std::string const&);
    bool UnInstallApk(std::string const&, std::string&);

    SelectedList GetDeviceList();
    bool         GetDeviceListUI();
    bool         GetDeviceSetupUI();
    std::string  GetDeviceInfo();
    std::string  GetDriverInfo();
    std::string  GetProperties(ADBDriver::DeviceInfoType);

    std::vector<uint8_t> Capture();
    std::vector<uint8_t> Capture(CapType, uint32_t, uint32_t);
    bool Video(uint32_t, uint32_t, std::function<bool(std::vector<uint8_t>&, uint32_t, uint32_t)>);
    bool Video(uint32_t, uint32_t, uint32_t, std::function<bool(std::vector<uint8_t>&, uint32_t, uint32_t)>);

private:

    void deviceinfo();
    bool sendfileraw(std::string const&, std::string const&); /// destination is full path include permission
    std::string permission(GameDev::ADBDriver::FilePermissionType);
    bool dirlistitem(std::string const&, std::string const&, ADBDriver::DirItem&);
};

extern template const wchar_t * ADBDriver::GetDeviceID<const wchar_t*>();
extern template std::string     ADBDriver::GetDeviceID<std::string>();
extern template std::wstring    ADBDriver::GetDeviceID<std::wstring>();

extern template bool ADBDriver::AdbRawT<std::wstring>(std::wstring, std::wstring const &, std::wstring&, bool);
extern template bool ADBDriver::AdbRawT<std::string>(std::string, std::string const &, std::string&, bool);

}
