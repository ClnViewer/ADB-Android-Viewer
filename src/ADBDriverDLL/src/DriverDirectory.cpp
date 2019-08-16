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

#include "DriverInternal.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace GameDev
{

static const char *l_permd             = " (Permission denied)";
static const char *l_root_desc         = "Android / root directory";
static const char *l_root_empty        = "Directory empty..";
static const char *l_root_error        = "Directory get entry error";
static const char *l_perr              = "not calculate symlink location";
static const char *l_datalocaltmp_txt  = "-> /data/local/tmp";
static const char *l_datalocaltmp_cmd  = "/data/local/tmp";
static const char *l_datalocaltmp_desc = "hardcore link to /data/local/tmp";

static const char  l_data_is_symlink[] = "->";
static const char  l_data_is_directory[] = "drwx------";
static const char  l_data_is_file_access0[] = "root";
static const char  l_data_is_file_access1[] = "-rw-------";
static const char  l_data_is_file_access2[] = "-rw-------";

#   if defined(_BUILD_LISTDIR_BY_ADB)
bool ADBDriver::ListDir(
    std::string const & root,
    std::vector<ADBDriver::DirItem> & vdi,
    std::string & rs)
    {
        return false;
    }

#   else
bool ADBDriver::ListDir(
    std::string const & root,
    std::vector<ADBDriver::DirItem> & vdi,
    std::string & rs)
    {
        do
        {
            std::string l_root = ((root.empty()) ? "/" : root.c_str());
            std::string l_data;
            {
                std::stringstream ss;
                ss << "ls -l " << l_root.c_str();

                if (!SendToShell(ss.str(), l_data))
                {
                    rs = l_root_error;
                    break;
                }
                if (l_data.empty())
                {
                    rs = l_root_empty;
                    break;
                }
            }
            std::vector<std::string> tokens;
            {
                std::string       token;
                std::stringstream ss(l_data);

                while (std::getline(ss, token, '\n'))
                {
                    if (token.empty())
                        continue;

                    GameDev::clearend<std::string>(token);
                    if (token.empty())
                        continue;

                    tokens.push_back(token);
                }
            }
            if  (l_root.compare("/"))
            {
                fs::path fs_root{ l_root };

                ADBDriver::DirItem di0(
                        fs_root.root_path().generic_string(),
                        fs_root.root_path().generic_string(),
                        l_root_desc,
                        ADBDriver::FileType::FILETYPE_ROOT,
                        { 255, 255, 255, 0 }
                );
                vdi.push_back(di0);

                ADBDriver::DirItem di1(
                        "..",
                        fs_root.parent_path().generic_string(),
                        fs_root.parent_path().generic_string(),
                        ADBDriver::FileType::FILETYPE_BACK,
                        { 255, 228, 43, 0 }
                );
                vdi.push_back(di1);
            }
            else
            {
                ADBDriver::DirItem di0(
                        l_datalocaltmp_txt,
                        l_datalocaltmp_cmd,
                        l_datalocaltmp_desc,
                        ADBDriver::FileType::FILETYPE_DIR,
                        { 133, 175, 33, 0 }
                );
                vdi.push_back(di0);
            }

            if (!tokens.size())
            {
                rs = l_root_empty;
                break;
            }

            std::vector<ADBDriver::DirItem> vdirs;
            std::vector<ADBDriver::DirItem> vfiles;

            for (auto & s_ : tokens)
            {
                ADBDriver::DirItem di{};
                if (dirlistitem(l_root, s_, di))
                {
                    switch (di.type)
                    {
                        case ADBDriver::FileType::FILETYPE_DIR:
                            {
                                vdirs.push_back(di);
                                break;
                            }
                        case ADBDriver::FileType::FILETYPE_FILE:
                        case ADBDriver::FileType::FILETYPE_SYMLINK:
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

            //
            {
                std::stringstream ss;
                ss << l_root << " -> directories: " << vdirs.size() << ", files: " << vfiles.size();
                rs = ss.str();
            }
            return true;
        }
        while (0);
        return false;
    }

bool ADBDriver::dirlistitem(std::string const & root, std::string const & s, ADBDriver::DirItem & di)
    {
        do
        {
            std::stringstream        ss(s);
            std::string              token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ' '))
            {
                if (token.empty())
                    continue;

                GameDev::clearend<std::string>(token);
                if (token.empty())
                    continue;

                tokens.push_back(token);
            }

            if (tokens.size() < 6)
                break;

            if (s.data()[0] == 'l')
                di.type = ADBDriver::FileType::FILETYPE_SYMLINK;
            else if (s.data()[0] == 'd')
                di.type = ADBDriver::FileType::FILETYPE_DIR;
            else if (s.data()[0] == '-')
                di.type = ADBDriver::FileType::FILETYPE_FILE;
            else
                break;

            fs::path l_name{ root };

            switch (di.type)
            {
                case ADBDriver::FileType::FILETYPE_SYMLINK:
                    {
                        uint32_t idx = (tokens.size() - 3);
                        l_name /= tokens[idx];
                        di.s = l_name.filename().generic_string();
                        if (tokens[(idx + 1)].compare(0U, __CSZ(l_data_is_symlink), l_data_is_symlink) == 0)
                        {
                            di.Description(
                                l_name.filename().generic_string(),
                                l_data_is_symlink,
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
                        break;
                    }
                case ADBDriver::FileType::FILETYPE_DIR:
                    {
                        uint32_t idx  = (tokens.size() - 1);
                        l_name /= tokens[idx];
                        di.s = l_name.filename().generic_string();

                        if (tokens[0].compare(0U, __CSZ(l_data_is_directory), l_data_is_directory) == 0)
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
                        break;
                    }
                case ADBDriver::FileType::FILETYPE_FILE:
                    {
                        uint32_t idx  = (tokens.size() - 1);
                        l_name /= tokens[idx];
                        di.s = l_name.filename().generic_string();

                        if (
                            (tokens[1].compare(0U,  __CSZ(l_data_is_file_access0), l_data_is_file_access0) == 0) &&
                            ((tokens[0].compare(0U, __CSZ(l_data_is_file_access1), l_data_is_file_access1) == 0) ||
                             (tokens[0].compare(0U, __CSZ(l_data_is_file_access2), l_data_is_file_access2) == 0)
                            ))
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
                            std::stringstream ss;
                            ss << SizeFile(tokens[3]);
                            ss << ", " << tokens[0].c_str() << "/"  << tokens[1].c_str();
                            di.Description(
                                l_name.generic_string(),
                                "",
                                ss.str(),
                                { 20, 148, 20, 0 }
                            );
                            di.cmds = l_name.generic_string();
                        }
                        break;
                    }
                default:
                    return false;
            }
            return true;
        }
        while (0);
        return false;
    }
#   endif

};

