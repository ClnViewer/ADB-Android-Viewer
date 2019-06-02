#pragma once

#include "plugin/AppIPluginCallBack.h"

class AppConfig
{
private:
    //
    Plugins::PluginCallBack_s   cnf_cbcmd;

    AppConfig();
    AppConfig(const AppConfig& apc) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

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

    static AppConfig& instance();
    const void * GetAdbCb() const;
};
