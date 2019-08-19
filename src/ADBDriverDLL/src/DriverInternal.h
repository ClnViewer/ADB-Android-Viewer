
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
#    include "Utils/extStringUtils.h"

#    include "DriverExcept.h"
#    include "DriverNet.h"
#    include "ADBDriver.h"

namespace GameDev
{

static inline HMODULE gethmodule()
    {
#   if defined(OS_WIN)
        HMODULE hmd;
#       if defined(_BUILD_DLL)
        if ((!GetModuleHandleEx(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCTSTR>(GameDev::gethmodule),
                &hmd)
        ) || (hmd == INVALID_HANDLE_VALUE))
#       else
        if ((hmd = GetModuleHandle(NULL)) == INVALID_HANDLE_VALUE)
#       endif
            return nullptr;
        return hmd;
#   else
    return nullptr;
#   endif
    }

};
#  endif

#endif
