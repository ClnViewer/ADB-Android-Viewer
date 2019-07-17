/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../../ADBViewer/src/loader.h"
#include "PackManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <direct.h>

#define INCBIN_PREFIX bin_
#include "extern/incbin.h"
#define MIN_ASDK_VERSION 20U

typedef struct
{
    const uint8_t *datab;
    const uint8_t *datae;
    const size_t   dsz;
} BinData_s;

typedef struct
{
    BinData_s      data[3];
    const char   **outpath;
    const size_t   opsz;
} BinBase_s;

namespace Resources
{

static inline const char *outdir = "remotebin";
static inline const char *outfile = "remotebin\\ascreencap";

INCBIN(5_arm64v8a, "../../../AscreenCap/dist/5/arm64-v8a/ascreencap");
INCBIN(5_armeabiv7a, "../../../AscreenCap/dist/5/armeabi-v7a/ascreencap");
INCBIN(5_x86, "../../../AscreenCap/dist/5/x86/ascreencap");
INCBIN(5_x86_64, "../../../AscreenCap/dist/5/x86_64/ascreencap");

INCBIN(8_arm64v8a, "../../../AscreenCap/dist/8/arm64-v8a/ascreencap");
INCBIN(8_armeabiv7a, "../../../AscreenCap/dist/8/armeabi-v7a/ascreencap");
INCBIN(8_x86, "../../../AscreenCap/dist/8/x86/ascreencap");
INCBIN(8_x86_64, "../../../AscreenCap/dist/8/x86_64/ascreencap");

INCBIN(9_arm64v8a, "../../../AscreenCap/dist/9/arm64-v8a/ascreencap");
INCBIN(9_armeabiv7a, "../../../AscreenCap/dist/9/armeabi-v7a/ascreencap");
INCBIN(9_x86, "../../../AscreenCap/dist/9/x86/ascreencap");
INCBIN(9_x86_64, "../../../AscreenCap/dist/9/x86_64/ascreencap");

static inline const char *l_path_arm64v8a[] =
{
    "arm64-v8a"
};
static inline const char *l_path_armeabiv7a[] =
{
    "armeabi-v7a", "armeabi"
};
static inline const char *l_path_x86[] =
{
    "x86", "X86"
};
static inline const char *l_path_x86_64[] =
{
    "x86_64", "X86_64"
};

static inline BinBase_s l_binbase[] =
{
    {
        .data  = {
            {
                .datab = bin_5_arm64v8aData,
                .datae = bin_5_arm64v8aEnd,
                .dsz   = bin_5_arm64v8aSize
            },
            {
                .datab = bin_8_arm64v8aData,
                .datae = bin_8_arm64v8aEnd,
                .dsz   = bin_8_arm64v8aSize
            },
            {
                .datab = bin_9_arm64v8aData,
                .datae = bin_9_arm64v8aEnd,
                .dsz   = bin_9_arm64v8aSize
            }
        },
        .outpath = l_path_arm64v8a,
        .opsz = __NELE(l_path_arm64v8a)
    },
    {
        .data  = {
            {
                .datab = bin_5_armeabiv7aData,
                .datae = bin_5_armeabiv7aEnd,
                .dsz   = bin_5_armeabiv7aSize
            },
            {
                .datab = bin_8_armeabiv7aData,
                .datae = bin_8_armeabiv7aEnd,
                .dsz   = bin_8_armeabiv7aSize
            },
            {
                .datab = bin_9_armeabiv7aData,
                .datae = bin_9_armeabiv7aEnd,
                .dsz   = bin_9_armeabiv7aSize
            }
        },
        .outpath = l_path_armeabiv7a,
        .opsz = __NELE(l_path_armeabiv7a)
    },
    {
        .data  = {
            {
                .datab = bin_5_x86Data,
                .datae = bin_5_x86End,
                .dsz   = bin_5_x86Size
            },
            {
                .datab = bin_8_x86Data,
                .datae = bin_8_x86End,
                .dsz   = bin_8_x86Size
            },
            {
                .datab = bin_9_x86Data,
                .datae = bin_9_x86End,
                .dsz   = bin_9_x86Size
            }
        },
        .outpath = l_path_x86,
        .opsz = __NELE(l_path_x86)
    },
    {
        .data  = {
            {
                .datab = bin_5_x86_64Data,
                .datae = bin_5_x86_64End,
                .dsz   = bin_5_x86_64Size
            },
            {
                .datab = bin_8_x86_64Data,
                .datae = bin_8_x86_64End,
                .dsz   = bin_8_x86_64Size
            },
            {
                .datab = bin_9_x86_64Data,
                .datae = bin_9_x86_64End,
                .dsz   = bin_9_x86_64Size
            }
        },
        .outpath = l_path_x86_64,
        .opsz = __NELE(l_path_x86_64)
    }
};

static uint32_t f_strToUint(std::string const & opt)
{
    try
    {
        return stoul(opt);
    }
    catch (std::invalid_argument &)
    {
        return 0U;
    }
    catch (std::out_of_range &)
    {
        return 0U;
    }
    catch (...)
    {
        return 0U;
    }
}

static uint32_t f_releaseToIdx(std::string const & opt)
    {
        if (opt.empty())
            return 1000U;

        const char c[] = { opt.data()[0], '\0' };
        std::string s = c;

        switch (f_strToUint(s))
        {
            case 5U:
            case 6U:
            case 7U: return 0U;
            case 8U: return 1U;
            case 9U: return 2U;
            default: return 1000U;
        }
    }


PackManager::PackManager() {}
PackManager::~PackManager() {}

bool PackManager::checkbin(std::string & binlist, std::string & vsdk, std::string & rel)
{
    do
    {
        if (
            (binlist.empty()) ||
            (vsdk.empty()) ||
            (rel.empty())
           )
           break;

        if (f_strToUint(vsdk) < MIN_ASDK_VERSION)
           break;

        uint32_t nrel = f_releaseToIdx(rel);
        if (nrel >= __NELE(l_binbase[0].data))
           break;

        std::vector<std::string> platforms;
        std::string found(",");
        size_t prev = 0U,
               pos  = 0U;
        do
        {
            pos = binlist.find(found, prev);
            if (pos == std::string::npos)
                pos = binlist.length();

            std::string platform = binlist.substr(prev, (pos - prev));
            if (!platform.empty())
                platforms.push_back(platform);

            prev = pos + found.length();
        }
        while (pos < binlist.length() && prev < binlist.length());

        if (!platforms.size())
            break;

        for (uint32_t i = 0; i < __NELE(l_binbase); i++)
            for (uint32_t n = 0; n < l_binbase[i].opsz; n++)
                for (uint32_t k = 0; k < platforms.size(); k++)
                    if (!platforms[k].compare(0U, platforms[k].length(), l_binbase[i].outpath[n]))
                    {
                        savebin(l_binbase[i].data[nrel].datab, l_binbase[i].data[nrel].dsz);
                        return true;
                    }
    }
    while (0);

    return false;
}

bool PackManager::savebin(const uint8_t *data, size_t dsz)
{
    do
    {
        if ((!data) || (!dsz))
            break;

        FILE __AUTO(__autofile) *fp = nullptr;

        if (::_access(outdir, F_OK) < 0)
            if (::_mkdir(outdir) < 0)
                break;

        if (!(fp = ::fopen(outfile, "wb")))
            break;

        if (::fwrite(data, 1U, dsz, fp) != dsz)
            break;

        return true;
    }
    while (0);

    return false;
}


}
