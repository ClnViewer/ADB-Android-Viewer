
#pragma once

#if !defined(_WIN32_IE)
#  define _WIN32_IE 0x0600
#endif

#if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0502
#endif

#if !defined(DLL_EXPORT)
#  ifdef _BUILD_DLL
#    define DLL_EXPORT __declspec(dllexport)
#  else
#    define DLL_EXPORT __declspec(dllimport)
#  endif
#endif

#include "ColorDialog.h"
#include "CustomDialog.h"
#include "FileDialog.h"
