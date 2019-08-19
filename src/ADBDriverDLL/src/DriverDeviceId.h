#pragma once

namespace GameDev
{

class DLL_EXPORT ADBDriverDeviceId
{
private:
    //
    std::wstring         m_deviceid;

public:
    //
    bool IsID() const;
    void ClearID();

    template<typename T>
        void SetID(T const&);

    template<typename T>
        T GetID();

    const std::wstring & operator = (const ADBDriverDeviceId&) const;
};

extern template void            ADBDriverDeviceId::SetID<std::wstring>(std::wstring const&);
extern template void            ADBDriverDeviceId::SetID<std::string> (std::string const&);

extern template const wchar_t * ADBDriverDeviceId::GetID<const wchar_t*>();
extern template std::wstring    ADBDriverDeviceId::GetID<std::wstring>();
extern template std::string     ADBDriverDeviceId::GetID<std::string>();

}
