
#pragma once

#if !defined(__APPLOADER_H)
#define __APPLOADER_H 1

#if !defined(_WIN32_IE)
#  define _WIN32_IE 0x0600
#endif
#if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT 0x0502
#endif

#ifdef _BUILD_DLL
#  define DLL_EXPORT __declspec(dllexport)
#else
#  define DLL_EXPORT __declspec(dllimport)
#endif

#if defined(_MSC_VER)
#  error "GCC/clang only support build"
#endif

#if ( \
        defined(_WIN32) || defined(__WIN32__) || defined(_Windows) || \
        defined(__WINNT) || defined(__WINNT__) || defined(WINNT) || \
        defined(_WIN64) || defined(__WIN64__) || \
        defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__))

#   define OS_WIN 1

#  if (defined(_WIN32) || defined(__WIN32__) || defined(_Windows))
#      define OS_WIN32 1
#  elif (defined(_WIN64) || defined(__WIN64__))
#      define OS_WIN64 1
#  else
#      define OS_OTHER 1
#  endif

#endif

#if defined(__GNUC__)

#  if defined(OS_WIN)
#    if (__GNUC__ >= 9)
#      if __has_include(<filesystem>)
#         define OS_CPP_FILESYSTEM 1
#      endif
#    endif
#  else
#    if (__GNUC__ >= 7)
#      if __has_include(<filesystem>)
#         define OS_CPP_FILESYSTEM 1
#      endif
#    endif
#  endif

#endif

#if !defined(OS_WIN)
#   define _access access
#   define _mkdir mkdir
#   define _stat stat
    typedef const char* LPCSTR;
#endif

#define __NELE(a) (sizeof(a) / sizeof(a[0]))
#define __CSZ(a) __extension__ ({uint32_t r = __NELE(a); r = ((r > 0) ? (r - 1U) : 0U ); r; })
#define __AUTO(x) __attribute__((cleanup(x)))
#define __ATTR_UNUSED __attribute__ (( __unused__ ))

#ifdef __cplusplus

#  if !defined(NO_CPP_HEADERS)

#  include <iostream>
#  include <atomic>
#  include <memory>
#  include <algorithm>
//#  include <iomanip>
//#  include <codecvt>
#  include <string>
#  include <vector>
#  include <map>
#  include <queue>
#  include <thread>
#  include <functional>
#  include <sstream>
#  include <fstream>
#  include <future>
#  include <valarray>

#  else

#  include <cstdio>

#  endif

#endif

#include <winsock2.h>
#include <windows.h>
#include <commctrl.h>
#include <time.h>
#include <io.h>
#include "version.h"

#if defined(_DEBUG)
#  include "exchndl.h"
#endif

static inline void __attribute__((always_inline)) __autofile(void *v)
{
    if (v)
    {
#       if defined(__cplusplus)
        void *x = static_cast<void*>(*static_cast<void**>(v));
#       else
        void *x = (void*)(*(void**)v);
#       endif
        if (x)
        {
#           if defined(__cplusplus)
            fclose(static_cast<FILE*>(x));
#           else
            fclose((FILE*)x);
#           endif
            x = NULL;
        }
    }
}

#endif
