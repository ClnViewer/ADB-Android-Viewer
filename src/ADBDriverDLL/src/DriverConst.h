
#pragma once

#include "Utils/extEnumPrintable.h"

namespace GameDev
{

class DriverConst
{
    public:

        ENUM_PRINTABLE(DeviceInfoType, int32_t,
                DI_CPU_LIST,
                DI_VERSION_SDK,
                DI_VERSION_RELEASE
        );
        ENUM_PRINTABLE(KeysType, int32_t,
                KEYS_ANDROID,
                KEYS_SDL,
                KEYS_WINAPI
        );
        ENUM_PRINTABLE(CapType, int32_t,
                CAP_RAW,
                CAP_PNG,
                CAP_ACAP_BMP,
                CAP_ACAP_STREAM
        );
        ENUM_PRINTABLE(FilePermissionType, int32_t,
                PERM_RW = 0,
                PERM_RWX,
                PERM_RWRW,
                PERM_RWXRWX,
                PERM_RWRWRW,
                PERM_RWXRWXRWX,
                PERM_NONE
        );
        ENUM_PRINTABLE(FileType, int32_t,
                FILETYPE_NONE = 0,
                FILETYPE_DIR,
                FILETYPE_FILE,
                FILETYPE_SYMLINK,
                FILETYPE_FIFO,
                FILETYPE_CHARDEV,
                FILETYPE_BLKDEV,
                FILETYPE_SOCK,
                FILETYPE_ROOT,
                FILETYPE_BACK,
                FILETYPE_INFO,
                FILETYPE_READONLY
        );
        ENUM_PRINTABLE(ClearType, int32_t,
                CLEARTYPE_NONE,
                CLEARTYPE_4,
                CLEARTYPE_8
        );

#       if defined(_BUILD_DLL)

#       define to_wchar__(A,B) A ## B
#       define to_wchar_(A) to_wchar__(L,A)

        static inline const wchar_t *lw_cmd_host      = L"host:";
        static inline const wchar_t *lw_cmd_devices   = L"devices-l";
        static inline const wchar_t *lw_cmd_adb_start = L"start-server";
        static inline const wchar_t *lw_cmd_adb_stop  = L"kill-server";

        static inline const char    *ls_cmd_exec     = "exec:";
        static inline const char    *ls_cmd_shell    = "shell:";
        static inline const char    *ls_cmd_sunc     = "sync:";
        static inline const char    *ls_cmd_input    = "input ";

        static inline const char    *ls_host_transport     = "host:transport:";
        static inline const char    *ls_host_transport_any = "host:transport-any";

#       define PATH0_ "adbbin\\adb.exe"
#       define PATH1_ "/data/local/tmp"
        static inline const wchar_t *lw_path_adb        = to_wchar_(PATH0_);
        static inline const char    *ls_path_adb        = PATH0_;
        static inline const wchar_t *lw_path_dir        = to_wchar_(PATH1_);
        static inline const char    *ls_path_dir        = PATH1_;

        static inline const char    *ls_cmod_file       = "/system/bin/chmod ";
        static inline const char    *ls_delete_file     = "rm -f ";
        static inline const char    *ls_apk_install     = "pm install -r /data/local/tmp/";
        static inline const char    *ls_apk_uninstall   = "pm uninstall";
        static inline const char    *ls_name_ascreencap = "ascreencap";
        static inline const char    *ls_name_screencap  = "screencap";
        static inline const char    *ls_src_ascreencap  = "remotebin\\ascreencap"; /// push local file

        static inline const char    *ls_cmd_ascreencap_ratio  = " --ratio ";
        static inline const char    *ls_cmd_ascreencap_rotate = " --rotate ";
        static inline const char    *ls_cmd_ascreencap_pack   = " --pack ";
        static inline const char    *ls_cmd_ascreencap_stdout = " --stdout --sdl";
        static inline const char    *ls_cmd_ascreencap_stream = " --stream --sdl";
        static inline const char    *ls_cmd_screencap_jpeg    = " -p";

        static inline const char    *ls_driverInfo = "/data/local/tmp/ascreencap -i";
        static inline const char    *ls_checkFile = "ls -l /data/local/tmp/ascreencap";
        static inline const char    *ls_errorFile = "No such file or directory";
        static inline const char    *ls_foundFile = ls_name_ascreencap;

#       define PUNCT0_ "\""
#       define PUNCT1_ " "
#       define PUNCT2_ "/"
        static inline const char    *ls_punct_quote = PUNCT0_;
        static inline const char    *ls_punct_space = PUNCT1_;
        static inline const char    *ls_punct_slash = PUNCT2_;

        static inline const wchar_t *lw_punct_quote = to_wchar_(PUNCT0_);
        static inline const wchar_t *lw_punct_space = to_wchar_(PUNCT1_);
        static inline const wchar_t *lw_punct_slash = to_wchar_(PUNCT2_);

#       if defined(_BUILD_WARNING_0)
#       define E0_ "File 'adb.exe' not found, or not access.."
#       define E1_ "Path to 'adb.exe' not set.."
#       define E2_ "Error: launch adb.exe"
#       define E3_ "Error: set process pipe"

        static inline const wchar_t *lw_errorBin  = to_wchar_(E0_);
        static inline const char    *ls_errorBin  = E0_;
        static inline const wchar_t *lw_errorSet  = to_wchar_(E1_);
        static inline const char    *ls_errorSet  = E1_;
        static inline const wchar_t *lw_errorRun  = to_wchar_(E2_);
        static inline const char    *ls_errorRun  = E2_;
        static inline const wchar_t *lw_errorPipe = to_wchar_(E3_);
        static inline const char    *ls_errorPipe = E3_;

#       define SFR0_  "Ok: receive data complete"
#       define SFR00_ "Error: receive data not complete, size wrong.."
#       define SFR1_  "Error: socket read data.."
#       define SFR2_  "Error: ADB device socket not response or bad address"
#       define SFR3_  "Error: limit retry read socket attempt.."
#       define SFR4_  "Error: socket from ADB device is disconnected.."
#       define SFR5_  "Error: block size wrong.."
#       define SFR6_  "Error: ADB packet header is wrong.."
#       define SFR7_  "Error: ADB packet unexpected block size"
#       define SFR8_  "Error: unknowns received tag"
#       define SFR9_  "Error: ADB protocol block size wrong.."
#       define SFR10_ "Error: write block size incomplete.."

        static inline const char    *ls_errorSFR0  = SFR0_;
        static inline const char    *ls_errorSFR00 = SFR00_;
        static inline const char    *ls_errorSFR1  = SFR1_;
        static inline const char    *ls_errorSFR2  = SFR2_;
        static inline const char    *ls_errorSFR3  = SFR3_;
        static inline const char    *ls_errorSFR4  = SFR4_;
        static inline const char    *ls_errorSFR5  = SFR5_;
        static inline const char    *ls_errorSFR6  = SFR6_;
        static inline const char    *ls_errorSFR7  = SFR7_;
        static inline const char    *ls_errorSFR8  = SFR8_;
        static inline const char    *ls_errorSFR9  = SFR9_;
        static inline const char    *ls_errorSFR10 = SFR10_;

#       define DN0_  "Ok, no errors"
#       define DN1_  "Error: get message"

        static inline const char    *ls_errorDN0  = DN0_;
        static inline const char    *ls_errorDN1  = DN1_;
#       endif

#       if defined(_BUILD_WARNING_1)
#       define E4_ "Error: launch binary not support for this build"
        static inline const wchar_t *lw_notSupport = to_wchar_(E4_);
        static inline const char    *ls_notSupport = E4_;
#       endif

        static inline const char    *ls_arrayInput[] =
        {
            "tap ",
            "swipe ",
            "keyevent ",
            "text "
        };
        static inline const char    *ls_arrayPropertis[] =
        {
            "ro.product.cpu.abilist",  /// armeabi-v7a,armeabi
            "ro.build.version.sdk",    /// 22
            "ro.build.version.release" /// 5.1
        };
        static inline const int32_t  ls_arrayModeFile[] =
        {
            600,
            700,
            660,
            770,
            666,
            777,
            400
        };
#       endif

};

}
