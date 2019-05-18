
#pragma once

namespace GameDev
{

class DLL_EXPORT DriverExcept : public std::runtime_error
{
public:
    DWORD ErrorLast;
    int32_t ErrorSoket;
    int32_t Line;
    std::wstring Msg;

    DriverExcept(const std::wstring & _msg)
        : std::runtime_error("Error"), Msg(_msg) {}

    DriverExcept(DWORD _el, int _es, int _l)
       : std::runtime_error("Error"), ErrorLast(_el), ErrorSoket(_es), Line(_l) {}

    DriverExcept(DWORD _es, int _l)
       : std::runtime_error("Error"), ErrorLast(0U), ErrorSoket(_es), Line(_l) {}

    const wchar_t * Message() const
       { return Msg.c_str(); }
};

}
