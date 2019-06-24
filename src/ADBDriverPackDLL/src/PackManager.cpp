
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
    const char   **outpath;
    const size_t   opsz;
} BinData_s;

namespace Resources
{

static inline const char *outdir = "remotebin";
static inline const char *outfile = "remotebin\\ascreencap";

INCBIN(arm64v8a, "../../../AscreenCap/dist/arm64-v8a/ascreencap");
INCBIN(armeabiv7a, "../../../AscreenCap/dist/armeabi-v7a/ascreencap");
INCBIN(x86, "../../../AscreenCap/dist/x86/ascreencap");
INCBIN(x86_64, "../../../AscreenCap/dist/x86_64/ascreencap");

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

static inline BinData_s l_binbase[] =
{
    {
        .datab = bin_arm64v8aData,
        .datae = bin_arm64v8aEnd,
        .dsz  = bin_arm64v8aSize,
        .outpath = l_path_arm64v8a,
        .opsz = __NELE(l_path_arm64v8a)
    },
    {
        .datab = bin_armeabiv7aData,
        .datae = bin_armeabiv7aEnd,
        .dsz  = bin_armeabiv7aSize,
        .outpath = l_path_armeabiv7a,
        .opsz = __NELE(l_path_armeabiv7a)
    },
    {
        .datab = bin_x86Data,
        .datae = bin_x86End,
        .dsz  = bin_x86Size,
        .outpath = l_path_x86,
        .opsz = __NELE(l_path_x86)
    },
    {
        .datab = bin_x86_64Data,
        .datae = bin_x86_64End,
        .dsz  = bin_x86_64Size,
        .outpath = l_path_x86_64,
        .opsz = __NELE(l_path_x86_64)
    }
};

static uint32_t f_strToUint(std::string const opt)
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

PackManager::PackManager() {}
PackManager::~PackManager() {}

bool PackManager::checkbin(std::string & binlist, std::string & vsdk)
{
    do
    {
        if (
            (!binlist.length()) ||
            (!vsdk.length())
           )
           break;

        if (f_strToUint(vsdk) < MIN_ASDK_VERSION)
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
                        savebin(l_binbase[i].datab, l_binbase[i].dsz);
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
