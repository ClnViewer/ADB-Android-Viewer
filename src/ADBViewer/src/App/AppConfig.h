#pragma once

#include "plugin/AppIPluginCallBack.h"

class AppConfig
{
private:
    //
    Plugins::PluginCallBack_s   cnf_cbcmd;

    AppConfig()
        : cnf_isrun(false), cnf_isstop(true), cnf_ispos(false),
          cnf_isfullscreen(false), cnf_adbinit(false),
          cnf_scale(2U), cnf_compress(9),
          cnf_point_input{}, cnf_adb_rect{}
    {
        using namespace std::placeholders;

        cnf_adb_rect.t  = 100U;
        cnf_point_input = { 50, (__H_default - 40) };
        cnf_cbcmd.click =
            [=](ADBDriver::Tap_s *t)
            {
                cnf_adb.Click(t);
            };
        cnf_cbcmd.swipe =
            [=](ADBDriver::Swipe_s *t)
            {
                cnf_adb.Click(t);
            };
        cnf_cbcmd.text =
            [=](std::string const & s)
            {
                cnf_adb.SendTextASCII(s);
            };
        cnf_cbcmd.key =
            [=](ADBDriver::KeysType t, int32_t k)
            {
                cnf_adb.SendSpecialKey(t, k);
            };
    }

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

    ~AppConfig()
    {
        /// cnf_adb.~ADBDriver();
    }

    static AppConfig& instance()
    {
        static AppConfig m_instance{};
        return m_instance;
    }

    const void * GetAdbCb() const
    {
        return (const void*)&cnf_cbcmd;
    }
};
