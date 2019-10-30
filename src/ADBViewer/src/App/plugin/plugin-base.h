#pragma once

#ifndef __PLUGIN_BASE_H__
#define __PLUGIN_BASE_H__

#include <windows.h>

#if defined(_BUILD_DLL)
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __declspec(dllimport)
#endif

#if !defined(__NELE)
#  define __NELE(a) (sizeof(a) / sizeof(a[0]))
#endif
#if !defined(__ATTR_UNUSED)
#  define __ATTR_UNUSED __attribute__ (( __unused__ ))
#endif

#ifdef __cplusplus

# include <atomic>
# include <string>
# include <sstream>
# include <vector>
# include <functional>
# include <cassert>
# include <ctime>

namespace GameDev
{
    class DriverConst
    {
    public:
        enum KeysType
        {
                KEYS_ANDROID,
                KEYS_SDL,
                KEYS_WINAPI
        };
    };
    class ADBDriver
    {
    public:

        enum KeysType
        {
            KEYS_ANDROID,
            KEYS_SDL,
            KEYS_WINAPI
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
    };
}

# include "AppIPlugin.h"

extern "C"
{
#endif

void * DLL_EXPORT CreatePlugin(const char*, const void*);
void   DLL_EXPORT DestroyPlugin(void);

#ifdef __cplusplus
}
#endif

#endif
