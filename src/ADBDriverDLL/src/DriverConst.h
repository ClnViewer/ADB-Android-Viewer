
#pragma once

namespace GameDev
{

class DriverConst
{
    public:
        static inline const wchar_t *lw_cmd_host  = L"host:";

        static inline const char    *ls_cmd_exec  = "exec:";
        static inline const char    *ls_cmd_shell = "shell:";
        static inline const char    *ls_cmd_sunc  = "sync:";

        static inline const wchar_t *lw_host_transport     = L"host:transport:";
        static inline const wchar_t *lw_host_transport_any = L"host:transport-any";
        static inline const char    *ls_host_transport     = "host:transport:";
        static inline const char    *ls_host_transport_any = "host:transport-any";

        static inline const wchar_t *lw_const_fail = L"FAIL";
        static inline const wchar_t *lw_const_ok   = L"OKAY";
        static inline const char    *ls_const_fail = "FAIL";
        static inline const char    *ls_const_ok   = "OKAY";

        static inline const char    *ls_src_ascreencap  = "remotebin\\ascreencap"; /// push local file
        static inline const char    *ls_path_file       = "/data/local/tmp/";
        static inline const char    *ls_mode_file       = ",0777";
        static inline const char    *ls_cmod_file       = "/system/bin/chmod 0777 /data/local/tmp/";
        static inline const char    *ls_apk_install     = "pm install -r /data/local/tmp/";
        static inline const char    *ls_apk_uninstall   = "pm uninstall";
        static inline const char    *ls_name_ascreencap = "ascreencap";
        static inline const char    *ls_name_screencap  = "screencap";

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

        static inline const char    *ls_arrayPropertis[] =
        {
            "ro.product.cpu.abilist",  /// armeabi-v7a,armeabi
            "ro.build.version.sdk",    /// 22
            "ro.build.version.release" /// 5.1
        };
};

}
