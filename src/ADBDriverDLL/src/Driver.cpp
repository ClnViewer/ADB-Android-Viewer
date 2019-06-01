
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
