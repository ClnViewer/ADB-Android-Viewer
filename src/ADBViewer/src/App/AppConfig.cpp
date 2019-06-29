/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../ADBViewer.h"

static inline const char *cnf_l_file = "ADBViewer.ini";
static inline const char *cnf_l_file_tag[] =
{
    "plugins-enable",
    "adb-path",
    "adb-device",
    "language",
    "save-format",
    "display-width",
    "display-height",
    "display-ratio",
    "display-rotate",
    "display-bender"
};

static inline const wchar_t *cnf_l_file_lang[] =
{
    L"ru",
    L"en",
    L"dm",
    L"cn"
};

static inline const char *cnf_l_file_savefmt[] =
{
    ".bmp",
    ".png"
};

template<typename T>
static uint32_t l_strToUint(T const & str)
{
    try
    {
        return stoul(str);
    }
    catch (std::invalid_argument&)
    {
        return 0U;
    }
    catch (std::out_of_range&)
    {
        return 0U;
    }
    catch (...)
    {
        return 0U;
    }
}

AppConfig::AppConfig()
        : cnf_isrun(false), cnf_isstop(true), cnf_ispos(false),
          cnf_isfullscreen(false), cnf_adbinit(false), cnf_isfcnf(false),
          cnf_disp_bender(true), cnf_disp_ratio(2U), cnf_disp_rotate(360U), cnf_compress(9),
          cnf_input_point{}, cnf_disp_point{},
          cnf_uevent(0U), cnf_adb_rect{}
    {
        using namespace std::placeholders;

        SetDisplaySize(__W_default, __H_default);
        cnf_save_fmt = cnf_l_file_savefmt[0];

        cnf_adb_rect.t  = 100U;
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
        cnf_input_point = { 50, (cnf_disp_point.y - 40) };
    }

AppConfig& AppConfig::instance()
    {
        static AppConfig m_instance{};
        return m_instance;
    }

void AppConfig::init()
    {

    }

const char * AppConfig::GetImageSaveFmt(uint32_t idx) const
    {
        idx = ((idx < __NELE(cnf_l_file_savefmt)) ? idx : 0);
        return cnf_l_file_savefmt[idx];
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

void AppConfig::SetDisplaySize(uint32_t w, uint32_t h)
    {
        cnf_disp_point.x = ((w) ? (static_cast<int32_t>(w) + __MENU_W_default) : cnf_disp_point.x);
        cnf_disp_point.y = ((h) ? static_cast<int32_t>(h) : cnf_disp_point.y);
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

        for (uint32_t i = 0U; i < __NELE(cnf_l_file_lang); i++)
        {
            if (wstr.compare(cnf_l_file_lang[i]) == 0)
            {
                cnf_lang = static_cast<ResManager::IndexLanguageResource>(i);
                break;
            }
        }
    }

void AppConfig::OnceUpdateFromFile()
    {
        std::wstring wstr;
        uint32_t val;

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_PATH), wstr))
            cnf_adb.SetAdbBin(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_DEVICE), wstr))
            cnf_adb.SetDeviceID(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_LANGUAGE), wstr))
            OnceUpdateLang(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_WIDTH), wstr))
            if (((val = l_strToUint<std::wstring>(wstr))) && (val >= 200))
                SetDisplaySize(val, 0U);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_HEIGHT), wstr))
            if (((val = l_strToUint<std::wstring>(wstr))) && (val >= 200))
                SetDisplaySize(0U, val);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_RATIO), wstr))
            if (((val = l_strToUint<std::wstring>(wstr))) && (val < 6))
                cnf_disp_ratio = val;

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_ROTATE), wstr))
            if (((val = l_strToUint<std::wstring>(wstr))) && (val <= 360))
                cnf_disp_rotate = val;

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_BENDER), wstr))
        {
            val = l_strToUint<std::wstring>(wstr);
            cnf_disp_bender = static_cast<bool>(val);
        }
        ///
        {
            std::string sstr;
            if (GetFromSection<std::string>(GetFileConfigId(ConfigIdType::CNF_SAVE_TYPE), sstr))
                if (!sstr.empty())
                    for (uint32_t i = 0U; i < __NELE(cnf_l_file_savefmt); i++)
                        if (sstr.compare(0U, sstr.length(), cnf_l_file_savefmt[i]) == 0)
                        {
                            cnf_save_fmt = cnf_l_file_savefmt[i];
                            break;
                        }
        }
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

void AppConfig::SaveToFile()
    {
        std::wstring wstr;
        std::wofstream l_file(cnf_l_file, std::ofstream::out);
        if (!l_file.is_open())
            return;

        for (uint32_t i = 0U; i < __NELE(cnf_l_file_tag); i++)
        {
            wstr = L"";

            switch (i)
            {
                case ConfigIdType::CNF_PLUGINS_ENABLE:
                    {
                        std::vector<Plugins::AppPluginManager::Plugin_s> plist =
                                Plugins::AppPluginManager::instance().listplugin();

                        for (uint32_t n = 0U; n < plist.size(); n++)
                            if (plist[n].state)
                                if (!plist[n].name.empty())
                                    l_file << cnf_l_file_tag[i] << "=" << plist[n].name.c_str() << L"\n";
                        continue;
                    }
                case ConfigIdType::CNF_ADB_PATH:
                    {
                        wstr = cnf_adb.GetAdbBin();
                        break;
                    }
                case ConfigIdType::CNF_ADB_DEVICE:
                    {
                        wstr = cnf_adb.GetDeviceID();
                        break;
                    }
                case ConfigIdType::CNF_LANGUAGE:
                    {
                        if (cnf_lang < __NELE(cnf_l_file_lang))
                            wstr = cnf_l_file_lang[cnf_lang];
                        break;
                    }
                case ConfigIdType::CNF_SAVE_TYPE:
                    {
                        if (!cnf_save_fmt.empty())
                            wstr.assign(cnf_save_fmt.begin(), cnf_save_fmt.end());
                        break;
                    }
                case ConfigIdType::CNF_DISP_WIDTH:
                    {
                        wstr = std::to_wstring(cnf_disp_point.x - __MENU_W_default);
                        break;
                    }
                case ConfigIdType::CNF_DISP_HEIGHT:
                    {
                        wstr = std::to_wstring(cnf_disp_point.y);
                        break;
                    }
                case ConfigIdType::CNF_DISP_RATIO:
                    {
                        wstr = std::to_wstring(cnf_disp_ratio);
                        break;
                    }
                case ConfigIdType::CNF_DISP_ROTATE:
                    {
                        wstr = std::to_wstring(cnf_disp_rotate);
                        break;
                    }
                case ConfigIdType::CNF_DISP_BENDER:
                    {
                        wstr = std::to_wstring(cnf_disp_bender);
                        break;
                    }
            }
            if (!wstr.empty())
                l_file << cnf_l_file_tag[i] << "=" << wstr.c_str() << L"\n";
        }
    }
