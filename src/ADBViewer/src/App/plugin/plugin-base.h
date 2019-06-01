#ifndef __BASE_PLUGIN_H__
#define __BASE_PLUGIN_H__

#include <windows.h>

#ifdef _BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus

# include <atomic>
# include <string>
# include <vector>
# include <functional>
# include <cassert>

namespace GameDev
{
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
