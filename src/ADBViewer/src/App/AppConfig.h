#pragma once

#include "plugin/AppIPluginCallBack.h"

class AppConfig
{
public:
    //
    enum ConfigIdType
    {
        CNF_PLUGINS_ENABLE = 0,
        CNF_ADB_PATH,
        CNF_ADB_DEVICE,
        CNF_LANGUAGE,
        CNF_SAVE_TYPE,
        CNF_DISP_WIDTH,
        CNF_DISP_HEIGHT,
        CNF_DISP_RATIO,
        CNF_DISP_ROTATE,
        CNF_DISP_BENDER,
        CNF_TERM_BOTTOM,
        CNF_TERM_TABCMD,
        CNF_BROWSER_DIRLOCAL,
        CNF_BROWSER_DIRDEVICE
    };

private:
    //
    Plugins::PluginCallBack_s  cnf_cbcmd;
    std::vector<std::string>   cnf_f_empty;
    std::map<std::string, std::vector<std::string>> cnf_f_config;
    std::once_flag             cnf_once;

    AppConfig();
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    bool GetFromFile();
    void OnceUpdateFromFile();
    void OnceUpdateLang(std::wstring const &);

    template<typename T>
    bool GetFromSection(const char*, T&);

public:
    //
    std::atomic<bool>        cnf_isrun;
    std::atomic<bool>        cnf_isstop;
    std::atomic<bool>        cnf_ispos;
    std::atomic<bool>        cnf_isfullscreen;
    std::atomic<bool>        cnf_adbinit;
    std::atomic<bool>        cnf_isfcnf;
    std::atomic<bool>        cnf_disp_bender;
    std::atomic<uint32_t>    cnf_disp_ratio;
    std::atomic<uint32_t>    cnf_disp_rotate;
    std::atomic<uint32_t>    cnf_compress;
    std::atomic<uint32_t>    cnf_term_bottom_pad;
    std::atomic<uint32_t>    cnf_term_cmd_idx;
    SDL_Point                cnf_input_point;
    SDL_Point                cnf_disp_point;
    uint32_t                 cnf_uevent;
    int32_t const            cnf_keymod_ctrl[3];
    int32_t const            cnf_keymod_alt[3];
    int32_t const            cnf_keymod_disabled[6];
    std::string              cnf_root_path;
    std::string              cnf_save_fmt;
    std::string              cnf_browser_dir_local;
    std::string              cnf_browser_dir_device;
    std::vector<std::string> cnf_term_cmd;

    GameDev::ADBDriver          cnf_adb;
    GameDev::ADBDriver::Swipe_s cnf_adb_rect;
    ResManager::IndexLanguageResource cnf_lang = ResManager::IndexLanguageResource::LANG_DEFAULT;

    static const uint32_t cnf_display_fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP;


    std::vector<std::string> & GetFileConfig(std::string const &);
    const char * GetFileConfigId(ConfigIdType);

    void init(std::string const&);
    static AppConfig& instance();
    const void * GetAdbCb() const;
    const char * GetImageSaveFmt(uint32_t) const;
    void SetDisplaySize(uint32_t, uint32_t);
    void SaveToFile();

    bool IsUtf8(std::string const&);
    bool IsUtf8(std::string const&, int32_t*);
    bool IsUtf8c(std::string const&);
};
