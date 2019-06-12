

#include "../ADBViewer.h"

static inline const char *cnf_l_file = "ADBViewer.ini";
static inline const char *cnf_l_file_tag[] =
{
    "plugins-enable",
    "adb-path",
    "adb-device",
    "language"
};

AppConfig::AppConfig()
        : cnf_isrun(false), cnf_isstop(true), cnf_ispos(false),
          cnf_isfullscreen(false), cnf_adbinit(false), cnf_isfcnf(false),
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
        OnceUpdateFromFile();
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

const char * AppConfig::GetFileConfigId(ConfigIdType idx)
    {
        if ((uint32_t)idx < __NELE(cnf_l_file_tag))
            return cnf_l_file_tag[idx];
        return nullptr;
    }

std::vector<std::string> & AppConfig::GetFileConfig(std::string const & tag)
    {
        do
        {
            std::call_once(
                    cnf_once,
                    [=]()
                    {
                        cnf_isfcnf = GetFromFile();
                    }
                );
            if ((!cnf_isfcnf.load()) || (!cnf_f_config.size()))
                break;

            auto found = cnf_f_config.find(tag);
            if (found == cnf_f_config.end())
                break;
            return found->second;
        }
        while (0);

        return cnf_f_empty;
    }

template<typename T>
bool AppConfig::GetFromSection(const char *tag, T & dstr)
    {
        do
        {
            std::vector<std::string> & v = GetFileConfig(tag);
            if (!v.size())
                break;

            if constexpr (std::is_same<T, std::wstring>::value)
            {
                std::string str = v[0];
                if (str.empty())
                    break;

                dstr.assign(str.begin(), str.end());
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                dstr = v[0];
                if (dstr.empty())
                    break;
            }
            else
                break;

            return true;
        }
        while (0);

        return false;
    }

void AppConfig::OnceUpdateLang(std::wstring const & wstr)
    {
        if (wstr.empty())
            return;
        if (wstr.compare(L"ru") == 0)
            cnf_lang = ResManager::IndexLanguageResource::LANG_RU;
        else if (wstr.compare(L"en") == 0)
            cnf_lang = ResManager::IndexLanguageResource::LANG_EN;
        else if (wstr.compare(L"cn") == 0)
            cnf_lang = ResManager::IndexLanguageResource::LANG_CN;
    }

void AppConfig::OnceUpdateFromFile()
    {
        std::wstring wstr;

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_PATH), wstr))
            cnf_adb.SetAdbBin(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_DEVICE), wstr))
            cnf_adb.SetDeviceID(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_LANGUAGE), wstr))
            OnceUpdateLang(wstr);
    }

bool AppConfig::GetFromFile()
    {
        std::ifstream l_file(cnf_l_file);
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
