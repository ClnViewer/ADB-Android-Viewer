#pragma once

#include "plugin/AppIPluginCallBack.h"

class AppConfig
{
private:
    //
    Plugins::PluginCallBack_s  cnf_cbcmd;
    std::vector<std::string>   cnf_f_empty;
    std::map<std::string, std::vector<std::string>> cnf_f_config;
    std::mutex                 cnf_lock;

    AppConfig();
    AppConfig(const AppConfig& apc) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    bool GetFromFile();

public:
    //
    std::atomic<bool>      cnf_isrun;
    std::atomic<bool>      cnf_isstop;
    std::atomic<bool>      cnf_ispos;
    std::atomic<bool>      cnf_isfullscreen;
    std::atomic<bool>      cnf_adbinit;
    std::atomic<uint32_t>  cnf_scale;
    std::atomic<uint32_t>  cnf_compress;
    SDL_Point              cnf_point_input;

    GameDev::ADBDriver          cnf_adb;
    GameDev::ADBDriver::Swipe_s cnf_adb_rect;

    std::vector<std::string> & GetFileConfig(std::string const &);

    static AppConfig& instance();
    const void * GetAdbCb() const;
};
