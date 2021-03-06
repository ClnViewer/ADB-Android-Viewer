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

#if defined(OS_CPP_FILESYSTEM)
#  include <filesystem>
   namespace fs = std::filesystem;
#endif

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
    "display-bender",
    "terminal-bottom",
    "terminal-tab-cmd",
    "browser-dir-local",
    "browser-dir-device"
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
          cnf_term_bottom_pad(0U), cnf_term_cmd_idx(0U),
          cnf_input_point{}, cnf_disp_point{}, cnf_uevent(0U),
          cnf_keymod_ctrl { KMOD_CTRL, KMOD_LCTRL, KMOD_RCTRL },
          cnf_keymod_alt { KMOD_RALT, KMOD_LALT, KMOD_ALT },
          cnf_keymod_disabled { KMOD_CTRL, KMOD_LCTRL, KMOD_RCTRL, KMOD_RALT, KMOD_LALT, KMOD_ALT },
          cnf_adb_rect{}
    {
        using namespace std::placeholders;

        SetDisplaySize(__W_default, __H_default);
        cnf_save_fmt = cnf_l_file_savefmt[0];
        SDL_Point point_img_menu = ResManager::imagesize(
            ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
        );

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
                cnf_adb.SendTextT<std::string>(s);
            };
        cnf_cbcmd.key =
            [=](DriverConst::KeysType t, int32_t k)
            {
                cnf_adb.SendSpecialKey(t, k);
            };

        cnf_browser_dir_local = "/";
        cnf_browser_dir_device = "/";

        OnceUpdateFromFile();
        cnf_input_point = { point_img_menu.x, (cnf_disp_point.y - 40) };

    }

AppConfig& AppConfig::instance()
    {
        static AppConfig m_instance{};
        return m_instance;
    }

void AppConfig::init(std::string const & execpath)
    {
#       if defined(OS_CPP_FILESYSTEM)
        cnf_root_path = fs::canonical(fs::path(execpath)).parent_path().generic_string();
#       else
        size_t pos = execpath.find_last_of("/\\");
        if (pos != std::string::npos)
            cnf_root_path = execpath.substr(0, pos);
        else
            cnf_root_path = ".";
#       endif
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
        SDL_Point point_img_menu = ResManager::imagesize(
            ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
        );
        cnf_disp_point.x = ((w) ? (static_cast<int32_t>(w) + point_img_menu.x) : cnf_disp_point.x);
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

        dstr.clear();
        return false;
    }


bool AppConfig::GetFromFile()
    {
        static const char delim[] = { '\n', '\r', ' ' };
        std::ifstream l_file(cnf_l_file);

        if (!l_file.is_open())
            return false;

        std::string line;
        while(getline(l_file, line))
        {
            if (line.empty())
                continue;

            for (auto & c : delim)
            {
                line.erase(0, line.find_first_not_of(c));
                line.erase(line.find_last_not_of(c) + 1);
            }

            if(line[0] == '#' || line.empty())
                continue;

            auto pos = line.find("=");
            if (pos == std::string::npos)
                continue;

            auto tag = line.substr(0, pos);
            auto val = line.substr(pos + 1);

            auto found = cnf_f_config.find(tag);
            if (found != cnf_f_config.end())
            {
                found->second.push_back(val);
            }
            else
            {
                std::vector<std::string> l_list;
                l_list.push_back(val);
                cnf_f_config.insert(make_pair(tag, l_list));
            }
        }
        return true;
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
        std::string  sstr;
        std::wstring wstr;
        uint32_t val;

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_PATH), wstr))
            cnf_adb.AdbBin.Set<std::wstring>(wstr);

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_ADB_DEVICE), wstr))
            cnf_adb.DeviceId.Set<std::wstring>(wstr);

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

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_TERM_BOTTOM), wstr))
            if ((val = l_strToUint<std::wstring>(wstr)))
                cnf_term_bottom_pad = val;

        if (GetFromSection<std::string>(GetFileConfigId(ConfigIdType::CNF_BROWSER_DIRLOCAL), sstr))
            if (!sstr.empty())
                cnf_browser_dir_local.assign(sstr.c_str());

        if (GetFromSection<std::string>(GetFileConfigId(ConfigIdType::CNF_BROWSER_DIRDEVICE), sstr))
            if (!sstr.empty())
                cnf_browser_dir_device.assign(sstr.c_str());

        if (GetFromSection<std::string>(GetFileConfigId(ConfigIdType::CNF_SAVE_TYPE), sstr))
            if (!sstr.empty())
                for (uint32_t i = 0U; i < __NELE(cnf_l_file_savefmt); i++)
                    if (sstr.compare(0U, sstr.length(), cnf_l_file_savefmt[i]) == 0)
                    {
                        cnf_save_fmt = cnf_l_file_savefmt[i];
                        break;
                    }

        if (GetFromSection<std::wstring>(GetFileConfigId(ConfigIdType::CNF_DISP_BENDER), wstr))
        {
            val = l_strToUint<std::wstring>(wstr);
            cnf_disp_bender = static_cast<bool>(val);
        }
        ///
        {
            std::vector<std::string> & l_tabcmd = GetFileConfig(GetFileConfigId(ConfigIdType::CNF_TERM_TABCMD));
            for (auto & s : l_tabcmd)
                if (!s.empty())
                    cnf_term_cmd.push_back(s.c_str());
        }
    }

void AppConfig::SaveToFile()
    {
        std::wstring wstr;
        std::wofstream l_file(cnf_l_file, std::ofstream::out);
        if (!l_file.is_open())
            return;

        SDL_Point point_img_menu = ResManager::imagesize(
            ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
        );

        cnf_term_cmd_idx = 0U;

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
                case ConfigIdType::CNF_TERM_TABCMD:
                    {
                        for (uint32_t n = 0U; n < cnf_term_cmd.size(); n++)
                            if (!cnf_term_cmd[n].empty())
                                l_file << cnf_l_file_tag[i] << "=" << cnf_term_cmd[n].c_str() << L"\n";
                        continue;
                    }
                case ConfigIdType::CNF_ADB_PATH:
                    {
                        wstr = cnf_adb.AdbBin.Get<std::wstring>();
                        break;
                    }
                case ConfigIdType::CNF_ADB_DEVICE:
                    {
                        wstr = cnf_adb.DeviceId.Get<std::wstring>();
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
                        wstr = std::to_wstring(cnf_disp_point.x - point_img_menu.x);
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
                case ConfigIdType::CNF_TERM_BOTTOM:
                    {
                        wstr = std::to_wstring(cnf_term_bottom_pad);
                        break;
                    }
                case ConfigIdType::CNF_BROWSER_DIRLOCAL:
                    {
                        if (!cnf_browser_dir_local.empty())
                            wstr.assign(cnf_browser_dir_local.begin(), cnf_browser_dir_local.end());
                        break;
                    }
                case ConfigIdType::CNF_BROWSER_DIRDEVICE:
                    {
                        if (!cnf_browser_dir_device.empty())
                            wstr.assign(cnf_browser_dir_device.begin(), cnf_browser_dir_device.end());
                        break;
                    }
            }
            if (!wstr.empty())
                l_file << cnf_l_file_tag[i] << "=" << wstr.c_str() << L"\n";
        }
    }
