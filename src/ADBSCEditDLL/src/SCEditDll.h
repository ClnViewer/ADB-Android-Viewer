#pragma once

#if !defined(ADBSCEDI_DECLARE_H)
#define ADBSCEDI_DECLARE_H 1

#  if !defined(_WIN32_IE)
#    define _WIN32_IE 0x0600
#  endif
#  if !defined(_WIN32_WINNT)
#    define _WIN32_WINNT 0x0502
#  endif

#  if !defined(DLL_EXPORT)
#    ifdef _BUILD_DLL
#      define DLL_EXPORT __declspec(dllexport)
#    else
#      define DLL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  if !defined(ISOLATION_AWARE_ENABLED)
#    define ISOLATION_AWARE_ENABLED 1
#  endif

#  include <winsock2.h>
#  include <windows.h>

#  if defined(__cplusplus)
#  include <string>
extern "C"
{
#endif

bool DLL_EXPORT RunEdit(std::string const&, std::string const&, bool, HICON);
bool DLL_EXPORT RunEditOpen(std::string const&, bool, HICON);
bool DLL_EXPORT RunEditEmpty(bool, HICON);

#if defined(__cplusplus)
}
#endif

#endif
