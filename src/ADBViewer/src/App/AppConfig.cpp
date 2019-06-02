

#include "../ADBViewer.h"

AppConfig::AppConfig()
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

AppConfig& AppConfig::instance()
    {
        static AppConfig m_instance{};
        return m_instance;
    }

const void * AppConfig::GetAdbCb() const
    {
        return (const void*)&cnf_cbcmd;
    }
