
#pragma once

#if !defined(ADBDRIVER_INTERNAL_H)
#define ADBDRIVER_INTERNAL_H 1

#define IDD_ADBLISTDLG  4001
#define IDD_ADBMGRDLG   4002

#define DLG_LISTV       4003
#define DLG_LISTI       4004
#define DLG_ADBPATH     4005
#define DLG_ADBINFO     4006
#define DLG_ADBSTAT     4007

#define IDS_LSTV1       3001
#define IDS_LSTV2       3002

#define IDS_ERRA        5000
#define IDS_ERR1        5001
#define IDS_ERR2        5002
#define IDS_ERR3        5003

#define ID_ADBSTART     6000
#define ID_ADBSTOP      6001
#define ID_ADBPATH      6002

#define IDR_CMDEVENT    7001

#  ifdef __cplusplus

#    define BIND_ASYNC(A) std::bind(&GameDev::ADBDriver::AdbRawT<A>, this, _1, _2, _3, _4)

#    include <codecvt>
#    include <iomanip>
#    include <queue>
#    include "../../ADBViewer/src/loader.h"

#    include "DriverExcept.h"
#    include "Utils/stdStringUtils.h"
#    include "ADBDriver.h"

namespace GameDev
{

static inline bool endcheck(char c)
{
    return ((c == '\r') || (c == '\n'));
}
static inline bool endcheck(wchar_t c)
{
    return ((c == L'\r') || (c == L'\n'));
}

template<typename T>
static inline void clearend(T & s)
    {
        s.erase(
            std::remove_if(
                s.begin(),
                s.end(),
                [](auto c)
                    {
                        return endcheck(c);
                    }
            ),
            s.end()
        );
    }

template<typename T>
static inline T filename(T const & s)
    {
        size_t pos;
        T fname;

        if constexpr (std::is_same<T, std::string>::value)
            pos = s.find_last_of("/\\");
        else if constexpr (std::is_same<T, std::wstring>::value)
            pos = s.find_last_of(L"/\\");

        if (pos == std::string::npos)
            fname = s.c_str();
        else
            fname = s.substr((pos + 1), (s.length() - 1)).c_str();
        return fname;
    }

};
#  endif

#endif
