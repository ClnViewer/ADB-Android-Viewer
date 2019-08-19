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

#include "../../ADBViewer.h"
#include <sstream>
#include <iterator>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static std::string l_FileDescription(fs::path const & file)
    {
        std::stringstream ss;
        ss << file.generic_string();

        try
        {
            std::uintmax_t sz = fs::file_size(file);
            if (sz != static_cast<uintmax_t>(-1))
                ss << " [" << AppConfig::instance().cnf_adb.FileSize(sz).c_str() << "]";

#           if defined(OS_WIN)
            /// not work!
            /*
            auto aft = fs::last_write_time(file);
            FILETIME sft;
            SYSTEMTIME stm{};
            ::memcpy(&sft, &aft, sizeof(stm));

            if (::FileTimeToSystemTime(&sft, &stm))
                ss << " [" << stm.wYear
                   << "/"  << stm.wMonth
                   << "/"  << stm.wDay
                   << " "  << stm.wHour
                   << ":"  << stm.wMinute
                   << "]";
            */
#           endif
        }
        catch (...) {}
        return ss.str();
    }

static std::string l_DirectoryDescription(fs::path const & dir)
    {
        std::stringstream ss;

        try
        {
            fs::space_info inf = fs::space(dir);
            ss << dir.generic_string().c_str() << " [";

            /// total
            if (inf.capacity == static_cast<uintmax_t>(-1))
                ss << "-/";
            else
                ss << AppConfig::instance().cnf_adb.FileSize(inf.capacity).c_str() << "/";

            /// used
            if ((inf.capacity == static_cast<uintmax_t>(-1)) || (inf.available == static_cast<uintmax_t>(-1)))
                ss << "-/";
            else
                ss << AppConfig::instance().cnf_adb.FileSize(inf.capacity - inf.available).c_str() << "/";

            /// free
            if (inf.available == static_cast<uintmax_t>(-1))
                ss << "-";
            else
                ss << AppConfig::instance().cnf_adb.FileSize(inf.available).c_str();

            ss << "]";
        }
        catch (...) {}
        return ss.str();
    }

std::string AppBrowserParse::basedir(GameDev::ADBDriver::DirItem const & di, AppBrowserPage::MenuInput mi)
    {
        do
        {
            if (di.cmds.empty())
                break;

            fs::path l_root{ di.cmds };

            switch (di.type)
            {
                case GameDev::DriverConst::FileType::FILETYPE_DIR:
                case GameDev::DriverConst::FileType::FILETYPE_ROOT:
                case GameDev::DriverConst::FileType::FILETYPE_BACK:
                    {
                        switch (mi)
                        {
                            case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                {
                                    if (fs::is_directory(l_root))
                                        return l_root.generic_string();
                                    else if (fs::is_regular_file(l_root))
                                        return l_root.parent_path().generic_string();
                                    break;
                                }
                            case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                {
                                    return l_root.generic_string();
                                }
                            default:
                                break;
                        }
                        break;
                    }
                case GameDev::DriverConst::FileType::FILETYPE_FILE:
                case GameDev::DriverConst::FileType::FILETYPE_SYMLINK:
                    {
                        switch (mi)
                        {
                            case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                {
                                    if (fs::is_regular_file(l_root))
                                        return l_root.parent_path().generic_string();
                                    else if (fs::is_directory(l_root))
                                        return l_root.generic_string();
                                    break;
                                }
                            case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                {
                                    return l_root.parent_path().generic_string();
                                }
                            default:
                                break;
                        }
                        break;
                    }
                default:
                    break;
            }
        }
        while (0);

        return "/";
    }

std::string AppBrowserParse::filename(GameDev::ADBDriver::DirItem const & di)
    {
        do
        {
            if (di.cmds.empty())
                break;

            if ((di.type != GameDev::DriverConst::FileType::FILETYPE_FILE) &&
                (di.type != GameDev::DriverConst::FileType::FILETYPE_SYMLINK))
                break;

            fs::path l_path{ di.cmds };
            return l_path.filename().generic_string();
        }
        while (0);
        return std::string();
    }

std::string AppBrowserParse::filepath(GameDev::ADBDriver::DirItem const & di, std::string const & outdir)
    {
        std::string fname = filename(di);
        if (fname.empty())
            return fname;

        fs::path l_path{ outdir };
        l_path /= fname.c_str();
        return l_path.generic_string();
    }

bool AppBrowserParse::apk_name(std::string const & s, GameDev::ADBDriver::DirItem & di)
    {
        size_t sp;
        if ((sp = s.find_last_of("=")) != std::wstring::npos)
        {
            di.desc = s.substr(0, sp);
            di.cmds = s.substr((sp + 1), (s.length() - sp));
        }
        else
            di.desc = s;

        if ((sp = di.desc.find_last_of(":")) != std::wstring::npos)
            di.desc = di.desc.substr((sp + 1), di.desc.length() - sp);

        if ((sp = di.desc.find_last_of("/")) != std::wstring::npos)
        {
            di.s = di.desc.substr((sp + 1), di.desc.length() - sp);
            if (!di.s.compare(0U, di.s.length(), "base.apk"))
            {
                std::string ss = di.desc.substr(0U, sp);
                if ((sp = ss.find_last_of("/")) != std::wstring::npos)
                {
                    ss = ss.substr((sp + 1), ss.length() - sp);
                    di.s.insert(0U, "/");
                    di.s.insert(0U, ss);
                }
            }
        }
        else
            di.s = di.desc;

        if (!di.cmds.empty())
        {
            di.cmds.erase(
                std::remove_if(
                    di.cmds.begin(),
                    di.cmds.end(),
                    [](char c)
                        {
                            return ((c == '\r') || (c == '\n'));
                        }
                    ),
                    di.cmds.end()
                );

            if (!di.cmds.empty())
            {
                di.desc += " [";
                di.desc += di.cmds.c_str();
                di.desc += "]";
            }
        }
        return true;
    }

bool AppBrowserParse::inf_name(std::string const & s, GameDev::ADBDriver::DirItem & di)
    {
        di.s = s;
        di.s.erase(
            std::remove_if(
                di.s.begin(),
                di.s.end(),
                    [](char c)
                    {
                        return ((c == '[') || (c == ']'));
                    }
            ),
            di.s.end()
        );
        di.desc = di.s;
        return true;
    }

bool AppBrowserParse::dir_list_local(std::string const & s, std::vector<GameDev::ADBDriver::DirItem> & vdi)
    {
        try
        {
            fs::path l_root{ s };
            do
            {
                if (fs::exists(l_root))
                    if (fs::is_directory(l_root))
                        break;

                l_root = "/";
            }
            while (0);

            m_rootpath_local = l_root.root_path().generic_string();

            if  (l_root.generic_string() != m_rootpath_local)
            {
                GameDev::ADBDriver::DirItem di0(
                        l_root.root_path().generic_string(),
                        l_root.root_path().generic_string(),
                        l_DirectoryDescription(l_root),
                        GameDev::DriverConst::FileType::FILETYPE_ROOT,
                        { 255, 255, 255, 0 }
                );
                vdi.push_back(di0);

                GameDev::ADBDriver::DirItem di1(
                        "..",
                        l_root.parent_path().generic_string(),
                        "",
                        GameDev::DriverConst::FileType::FILETYPE_BACK,
                        { 255, 228, 43, 0 }
                );
                vdi.push_back(di1);

                AddMessageQueue(
                    l_root.generic_string().c_str(),
                    10U,
                    -1
                );
            }
            else
                AddMessageQueue(
                    m_rootpath_local.c_str(),
                    10U,
                    -1
                );

            do
            {
                fs::directory_iterator sdir(l_root);
                std::vector<fs::path> vsubdirs;
                std::copy_if(begin(sdir), end(sdir), std::back_inserter(vsubdirs),
                        [](const fs::path & p)
                            {
                                return fs::is_directory(p);
                            }
                );

                if (!vsubdirs.size())
                    break;

                for (auto & p : vsubdirs)
                {
                    GameDev::ADBDriver::DirItem di(
                            p.filename().generic_string(),
                            p.generic_string(),
                            p.generic_string(),
                            GameDev::DriverConst::FileType::FILETYPE_DIR,
                            { 133, 175, 33, 0 }
                    );
                    vdi.push_back(di);
                }
            }
            while (0);

            do
            {
                fs::directory_iterator sfiles(l_root);
                std::vector<fs::path> vfiles;
                std::copy_if(begin(sfiles), end(sfiles), std::back_inserter(vfiles),
                        [](const fs::path & p)
                            {
                                return fs::is_regular_file(p);
                            }
                );
                if (!vfiles.size())
                    break;

                for (auto & p : vfiles)
                {
                    GameDev::ADBDriver::DirItem di(
                            p.filename().generic_string(),
                            p.generic_string(),
                            l_FileDescription(p),
                            GameDev::DriverConst::FileType::FILETYPE_FILE,
                            { 20, 148, 20, 0 }
                            //{ 10, 64, 10, 0 }
                    );
                    vdi.push_back(di);
                }
            }
            while (0);

            return static_cast<bool>(vdi.size());
        }
        catch (const fs::filesystem_error & ex)
        {
            AddMessageQueue(
                ex.what(),
                10U,
                -1
            );
            return false;
        }
        catch (const std::exception & ex)
        {
            AddMessageQueue(
                ex.what(),
                10U,
                -1
            );
            return false;
        }
        catch (...)
        {
            return false;
        }
    }

bool AppBrowserParse::dir_list_device(
    std::string const & root,
    std::vector<GameDev::ADBDriver::DirItem> & vdi)
    {
        std::string s;
        bool ret = AppConfig::instance().cnf_adb.DirList(root, vdi, s);
        if (!s.empty())
            AddMessageQueue(
                s,
                10U,
                -1
            );
        return ret;
    }

bool AppBrowserParse::file_delete_local(std::string const & s)
    {
        try
        {
            fs::path l_file{s};

            do
            {
                if (!fs::exists(l_file))
                    break;
                if (!fs::is_regular_file(l_file))
                    break;

                return fs::remove(l_file);
            }
            while (0);
            return false;
        }
        catch (...) { return false; }
        return false;
    }

bool AppBrowserParse::file_delete_device(std::string const & s)
    {
        std::string rs;
        std::stringstream ss;
        ss << "rm -f " << s.c_str();
        bool ret = AppConfig::instance().cnf_adb.SendToShell(ss.str(), rs, true);

        if (!rs.empty())
            AddMessageQueue(rs, 5U, -1);
        return ret;
    }
