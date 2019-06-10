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
        CNF_ADB_DEVICE
    };

private:
    //
    Plugins::PluginCallBack_s  cnf_cbcmd;
    std::vector<std::string>   cnf_f_empty;
    std::map<std::string, std::vector<std::string>> cnf_f_config;
    std::once_flag             cnf_once;

    AppConfig();
    AppConfig(const AppConfig& apc) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    bool GetFromFile();
    bool GetFromSection(const char*, std::wstring&);
    void OnceUpdateFromFile();

public:
    //
    std::atomic<bool>      cnf_isrun;
    std::atomic<bool>      cnf_isstop;
    std::atomic<bool>      cnf_ispos;
    std::atomic<bool>      cnf_isfullscreen;
    std::atomic<bool>      cnf_adbinit;
    std::atomic<bool>      cnf_isfcnf;
    std::atomic<uint32_t>  cnf_scale;
    std::atomic<uint32_t>  cnf_compress;
    SDL_Point              cnf_point_input;

    GameDev::ADBDriver          cnf_adb;
    GameDev::ADBDriver::Swipe_s cnf_adb_rect;


    std::vector<std::string> & GetFileConfig(std::string const &);
    const char * GetFileConfigId(ConfigIdType);

    static AppConfig& instance();
    const void * GetAdbCb() const;
};
