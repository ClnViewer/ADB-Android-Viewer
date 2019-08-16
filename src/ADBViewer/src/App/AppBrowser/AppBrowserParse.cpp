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

static const char *l_permd             = " (Permission denied)";
static const char *l_perr              = "parser not calculate symlink location";static const char *l_datalocaltmp_txt  = "-> /data/local/tmp";
static const char *l_datalocaltmp_cmd  = "/data/local/tmp";
static const char *l_datalocaltmp_desc = "hardcore link to /data/local/tmp";

static std::string l_CalcSize(uintmax_t sz)
    {
        std::stringstream ss;
        double b = sz;
        double k = (sz    / 1024.0);
        double m = ((sz   / 1024.0) / 1024.0);
        double g = (((sz  / 1024.0) / 1024.0) / 1024.0);
        double t = ((((sz / 1024.0) / 1024.0) / 1024.0) / 1024.0);

        if (t > 1)
            ss << t << " Tb";
        else if (g > 1)
            ss << g << " Gb";
        else if (m > 1)
            ss << m << " Mb";
        else if (k > 1)
            ss << k << " Kb";
        else
            ss << b << " Bytes";

        return ss.str();
    }

static std::string l_CalcSize(std::string const & s)
    {
        try
        {
            uintmax_t sz = std::stoull(s);
            if (sz)
                return l_CalcSize(sz);
            return std::string();
        }
        catch (...)
        {
            return std::string();
        }
    }

static std::string l_FileDescription(fs::path const & file)
    {
        std::stringstream ss;
        ss << file.generic_string();

        try
        {
            std::uintmax_t sz = fs::file_size(file);
            if (sz != static_cast<uintmax_t>(-1))
                ss << " [" << l_CalcSize(sz).c_str() << "]";

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
                ss << l_CalcSize(inf.capacity).c_str() << "/";

            /// used
            if ((inf.capacity == static_cast<uintmax_t>(-1)) || (inf.available == static_cast<uintmax_t>(-1)))
                ss << "-/";
            else
                ss << l_CalcSize(inf.capacity - inf.available).c_str() << "/";

            /// free
            if (inf.available == static_cast<uintmax_t>(-1))
                ss << "-";
            else
                ss << l_CalcSize(inf.available).c_str();

            ss << "]";
        }
        catch (...) {}
        return ss.str();
    }
std::string AppBrowserParse::basedir(AppBrowserPage::DrawItem const & di, AppBrowserPage::MenuInput mi)    {        do        {            if (di.cmds.empty())                break;            fs::path l_root{ di.cmds };            switch (di.type)            {                case AppBrowserPage::FileType::FILETYPE_DIR:                case AppBrowserPage::FileType::FILETYPE_ROOT:                case AppBrowserPage::FileType::FILETYPE_BACK:                    {                        switch (mi)                        {                            case AppBrowserPage::MenuInput::MENUINPUT_PC:                                {                                    if (fs::is_directory(l_root))                                        return l_root.generic_string();                                    else if (fs::is_regular_file(l_root))                                        return l_root.parent_path().generic_string();                                    break;                                }                            case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:                                {                                    return l_root.generic_string();                                }                            default:                                break;                        }                        break;                    }                case AppBrowserPage::FileType::FILETYPE_FILE:                case AppBrowserPage::FileType::FILETYPE_SYMLINK:                    {                        switch (mi)                        {                            case AppBrowserPage::MenuInput::MENUINPUT_PC:                                {                                    if (fs::is_regular_file(l_root))                                        return l_root.parent_path().generic_string();                                    else if (fs::is_directory(l_root))                                        return l_root.generic_string();                                    break;                                }                            case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:                                {                                    return l_root.parent_path().generic_string();                                }                            default:                                break;                        }                        break;                    }                default:                    break;            }        }        while (0);        return "/";    }

bool AppBrowserParse::apk_name(std::string const & s, AppBrowserPage::DrawItem & di)
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

bool AppBrowserParse::inf_name(std::string const & s, AppBrowserPage::DrawItem & di)
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

bool AppBrowserParse::dir_list_local(std::string const & s, std::vector<AppBrowserPage::DrawItem> & vdi)
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
                AppBrowserPage::DrawItem di0(
                        l_root.root_path().generic_string(),
                        l_root.root_path().generic_string(),
                        l_DirectoryDescription(l_root),
                        AppBrowserPage::FileType::FILETYPE_ROOT,
                        { 255, 255, 255, 0 }
                );
                vdi.push_back(di0);

                AppBrowserPage::DrawItem di1(
                        "..",
                        l_root.parent_path().generic_string(),
                        "",
                        AppBrowserPage::FileType::FILETYPE_BACK,
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
                    AppBrowserPage::DrawItem di(
                            p.filename().generic_string(),
                            p.generic_string(),
                            p.generic_string(),
                            AppBrowserPage::FileType::FILETYPE_DIR,
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
                    AppBrowserPage::DrawItem di(
                            p.filename().generic_string(),
                            p.generic_string(),
                            l_FileDescription(p),
                            AppBrowserPage::FileType::FILETYPE_FILE,
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
    std::string const & s, std::vector<AppBrowserPage::DrawItem> & vdi, std::string const & root)
    {
        do
        {
            if (s.empty())
                break;

            std::stringstream ss(s);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, '\n'))
            {
                token.erase(
                    std::remove_if(
                        token.begin(),
                        token.end(),
                        [](char c)
                            {
                                return ((c == '\r') || (c == '\n'));
                            }
                    ),
                    token.end()
                );

                if (token.empty())
                    continue;

                tokens.push_back(token);
            }

            m_rootpath_device = root;

            if  (m_rootpath_device.compare("/"))
            {
                fs::path l_root{ root };

                AppBrowserPage::DrawItem di0(
                        l_root.root_path().generic_string(),
                        l_root.root_path().generic_string(),
                        "Android / root directory",
                        AppBrowserPage::FileType::FILETYPE_ROOT,
                        { 255, 255, 255, 0 }
                );
                vdi.push_back(di0);

                AppBrowserPage::DrawItem di1(
                        "..",
                        l_root.parent_path().generic_string(),
                        l_root.parent_path().generic_string(),
                        AppBrowserPage::FileType::FILETYPE_BACK,
                        { 255, 228, 43, 0 }
                );
                vdi.push_back(di1);                AddMessageQueue(
                    l_root.generic_string().c_str(),
                    10U,
                    -1
                );
            }
            else            {                AppBrowserPage::DrawItem di0(
                        l_datalocaltmp_txt,
                        l_datalocaltmp_cmd,
                        l_datalocaltmp_desc,
                        AppBrowserPage::FileType::FILETYPE_DIR,
                        { 133, 175, 33, 0 }
                );
                vdi.push_back(di0);

                AddMessageQueue(
                    m_rootpath_device.c_str(),
                    10U,
                    -1
                );
            }

            if (!tokens.size())
                return false;

            std::vector<AppBrowserPage::DrawItem> vdirs;
            std::vector<AppBrowserPage::DrawItem> vfiles;

            for (auto & s_ : tokens)
            {
                AppBrowserPage::DrawItem di{};
                if (dir_item_device(s_, di))
                {
                    switch (di.type)
                    {
                        case AppBrowserPage::FileType::FILETYPE_DIR:
                            {
                                vdirs.push_back(di);
                                break;
                            }
                        case AppBrowserPage::FileType::FILETYPE_FILE:
                        case AppBrowserPage::FileType::FILETYPE_SYMLINK:
                            {
                                vfiles.push_back(di);
                                break;
                            }
                        default:
                            break;
                    }
                }
            }
            if (vdirs.size())
                vdi.insert(vdi.end(), vdirs.begin(), vdirs.end());
            if (vfiles.size())
                vdi.insert(vdi.end(), vfiles.begin(), vfiles.end());
        }
        while (0);

        return true;
    }

bool AppBrowserParse::dir_item_device(std::string const & s, AppBrowserPage::DrawItem & di)
    {
        do
        {
            if (s.empty())
                break;

            uint32_t idx = 0U;
            std::stringstream ss(s);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ' '))
            {
                if (token.empty())
                    continue;

                token.erase(
                    std::remove_if(
                        token.begin(),
                        token.end(),
                        [](char c)
                            {
                                return ((c == ' ') || (c == '\t'));
                            }
                    ),
                    token.end()
                );

                if (token.empty())
                    continue;

                tokens.push_back(token);
            }

            if (tokens.size() < 6)
                break;

            if (s.data()[0] == 'l')
                di.type = AppBrowserPage::FileType::FILETYPE_SYMLINK;
            else if (s.data()[0] == 'd')
                di.type = AppBrowserPage::FileType::FILETYPE_DIR;
            else if (s.data()[0] == '-')
                di.type = AppBrowserPage::FileType::FILETYPE_FILE;
            else
                break;

            fs::path l_name{ m_rootpath_device };

            if (di.type == AppBrowserPage::FileType::FILETYPE_SYMLINK)
            {
                idx = (tokens.size() - 3);
                l_name /= tokens[idx];
                di.s = l_name.filename().generic_string();

                if (tokens[(idx + 1)].compare(0U, 2, "->") == 0)
                {
                    di.Description(
                            l_name.filename().generic_string(),
                            "->",
                            tokens[(idx + 2)],
                            { 0, 187, 185, 0 }
                        );
                    di.cmds = tokens[(idx + 2)];
                }
                else
                {
                    di.Description(
                            l_perr,
                            "",
                            "",
                            { 150, 0, 0, 0 }
                        );
                    di.cmds = "";
                }
            }
            else if (di.type == AppBrowserPage::FileType::FILETYPE_DIR)
            {
                idx  = (tokens.size() - 1);
                l_name /= tokens[idx];
                di.s = l_name.filename().generic_string();

                if (tokens[0].compare(0U, 10, "drwx------") == 0)
                {
                    di.Description(
                            l_name.generic_string(),
                            l_permd,
                            "",
                            { 150, 0, 0, 0 }
                        );
                    di.cmds = "";
                }
                else
                {
                    di.Description(
                            l_name.generic_string(),
                            "",
                            tokens[3],
                            { 133, 175, 33, 0 }
                        );
                    di.cmds = l_name.generic_string();
                }
            }
            else if (di.type == AppBrowserPage::FileType::FILETYPE_FILE)
            {
                idx  = (tokens.size() - 1);
                l_name /= tokens[idx];
                di.s = l_name.filename().generic_string();

                if (
                    (tokens[0].compare(0U, 10, "-rwx------") == 0) ||
                    (tokens[0].compare(0U, 10, "-rw-------") == 0))
                {
                    di.Description(
                            l_name.generic_string(),
                            l_permd,
                            "",
                            { 110, 0, 0, 0 }
                        );
                    di.cmds = "";
                }
                else
                {
                    di.Description(
                            l_name.generic_string(),
                            "",
                            l_CalcSize(tokens[3]),
                            { 20, 148, 20, 0 }
                        );
                    di.cmds = l_name.generic_string();
                }
            }
            else
                break;

            return true;
        }
        while (0);
        return false;
    }
