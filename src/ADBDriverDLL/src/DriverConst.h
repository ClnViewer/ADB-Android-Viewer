
#pragma once

namespace GameDev
{

class DriverConst
{
    public:

        enum DeviceInfoType
        {
                DI_CPU_LIST,
                DI_VERSION_SDK,
                DI_VERSION_RELEASE
        };

        enum KeysType
        {
                KEYS_ANDROID,
                KEYS_SDL,
                KEYS_WINAPI
        };

        enum CapType
        {
                CAP_RAW,
                CAP_PNG,
                CAP_ACAP_BMP,
                CAP_ACAP_STREAM
        };

        enum FilePermissionType
        {
                PERM_RW = 0,
                PERM_RWX,
                PERM_RWRW,
                PERM_RWXRWX,
                PERM_RWRWRW,
                PERM_RWXRWXRWX,
                PERM_NONE
        };

        enum FileType
        {
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
        };

        enum ClearType
        {
                CLEARTYPE_NONE = 0,
                CLEARTYPE_4,
                CLEARTYPE_8
        };

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
