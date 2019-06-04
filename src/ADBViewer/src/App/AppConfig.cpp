

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

std::vector<std::string> & AppConfig::GetFileConfig(std::string const & tag)
    {
        do
        {
            if (!cnf_f_config.size())
                if (!GetFromFile())
                    break;

            auto found = cnf_f_config.find(tag);
            if (found == cnf_f_config.end())
                break;
            return found->second;
        }
        while (0);

        return cnf_f_empty;
    }

bool AppConfig::GetFromFile()
    {
        std::lock_guard<std::mutex> l_lock(cnf_lock);

        std::ifstream l_file("ADBViewer.ini");
        if (!l_file.is_open())
            return false;

        std::string line;
        while(getline(l_file, line))
        {
            line.erase(
                std::remove_if(line.begin(), line.end(), isspace), line.end()
            );
            if(line[0] == '#' || line.empty())
                continue;

            auto pos = line.find("=");
            auto tag = line.substr(0, pos);
            auto value = line.substr(pos + 1);

            auto found = cnf_f_config.find(tag);
            if (found != cnf_f_config.end())
            {
                found->second.push_back(value);
            }
            else
            {
                std::vector<std::string> l_list;
                l_list.push_back(value);
                cnf_f_config.insert(make_pair(tag, l_list));
            }
        }
        return true;
    }
