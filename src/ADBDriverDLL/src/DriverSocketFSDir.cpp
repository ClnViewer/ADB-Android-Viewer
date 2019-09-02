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

static const char *l_root_desc         = "Android / root directory";
static const char *l_datalocaltmp_txt  = "-> /data/local/tmp";
static const char *l_datalocaltmp_cmd  = "/data/local/tmp";
static const char *l_datalocaltmp_desc = "hardcore link to /data/local/tmp";
static const char *l_permd             = "Permission denied";
static const char *l_root_empty        = "Directory empty..";
static const char *l_root_error        = "Directory get entry error";

#if defined(_BUILD_DIRLIST_BY_SHELL)

static const char *l_perr              = "not calculate symlink location";
static const char  l_data_is_symlink[] = "->";

static const char  l_data_is_directory[] = "drwx------";
static const char  l_data_is_file_access0[] = "root";
static const char  l_data_is_file_access1[] = "-rw-------";
static const char  l_data_is_file_access2[] = "-rw-------";

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

                if (!SendToShell(ss.str(), l_data, DriverConst::ClearType::CLEARTYPE_NONE))
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

                    string_clear_end<std::string>(token);
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
                        DriverConst::FileType::FILETYPE_ROOT,
                        { 255, 255, 255, 0 }
                );
                vdi.push_back(di0);

                ADBDriver::DirItem di1(
                        "..",
                        fs_root.parent_path().generic_string(),
                        fs_root.parent_path().generic_string(),
                        DriverConst::FileType::FILETYPE_BACK,
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
                        DriverConst::FileType::FILETYPE_DIR,
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
                        case DriverConst::FileType::FILETYPE_DIR:
                            {
                                vdirs.push_back(di);
                                break;
                            }
                        case DriverConst::FileType::FILETYPE_FILE:
                        case DriverConst::FileType::FILETYPE_SYMLINK:
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

                string_clear_end<std::string>(token);
                if (token.empty())
                    continue;

                tokens.push_back(token);
            }

            if (tokens.size() < 6)
                break;

            if (s.data()[0] == 'l')
                di.type = DriverConst::FileType::FILETYPE_SYMLINK;
            else if (s.data()[0] == 'd')
                di.type = DriverConst::FileType::FILETYPE_DIR;
            else if (s.data()[0] == '-')
                di.type = DriverConst::FileType::FILETYPE_FILE;
            else
                break;

            fs::path l_name{ root };

            switch (di.type)
            {
                case DriverConst::FileType::FILETYPE_SYMLINK:
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
                case DriverConst::FileType::FILETYPE_DIR:
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
                case DriverConst::FileType::FILETYPE_FILE:
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

#else

static const char *l_type_desc[] =
{
    "",                 // FILETYPE_NONE
    //
    "",                 // FILETYPE_DIR
    ", ",               // FILETYPE_FILE
    "symlink/",         // FILETYPE_SYMLINK
    //
    "pipe file",        // FILETYPE_FIFO
    "character device", // FILETYPE_CHARDEV
    "block device",     // FILETYPE_BLKDEV
    "socket file",      // FILETYPE_SOCK
    //
    l_root_desc,        // FILETYPE_ROOT
    "",                 // FILETYPE_BACK
    "",                 // FILETYPE_INFO
    l_permd             // FILETYPE_READONLY
};

static ADBDriver::DirItemColor l_type_color[] =
{
    { 0, 0, 0, 0 },      // FILETYPE_NONE
    //
    { 133, 175, 33, 0 }, // FILETYPE_DIR
    { 20, 148, 20, 0 },  // FILETYPE_FILE
    { 0, 187, 185, 0 },  // FILETYPE_SYMLINK
    //
    { 128, 0, 128, 0 },  // FILETYPE_FIFO
    { 198, 150, 4, 0 },  // FILETYPE_CHARDEV
    { 198, 77, 4, 0 },   // FILETYPE_BLKDEV
    { 192, 0, 192, 0 },  // FILETYPE_SOCK
    //
    { 255, 255, 255, 0 },// FILETYPE_ROOT
    { 255, 228, 43, 0 }, // FILETYPE_BACK
    { 0, 0, 0, 0 },      // FILETYPE_INFO
    { 182, 0, 0, 0 }     // FILETYPE_READONLY
};

bool ADBDriver::DirList(
    std::string const & root,
    std::vector<ADBDriver::DirItem> & vdi,
    std::string & rs)
    {
        bool ret = false;
        SOCKET l_client = INVALID_SOCKET;

        do
        {
            if ((l_client = m_net.Connect()) == INVALID_SOCKET)
                break;

            if (!m_net.SyncTargetSend(l_client, DeviceId.Get<std::wstring>()))
                break;

            std::string l_root = ((root.empty()) ? "/" : root.c_str());
            fs::path fs_root{ l_root };

            if  (l_root.compare("/"))
            {
                ADBDriver::DirItem di0(
                        fs_root.root_path().generic_string(),
                        fs_root.root_path().generic_string(),
                        l_type_desc[enumtoint(DriverConst::FileType::FILETYPE_ROOT)],
                        DriverConst::FileType::FILETYPE_ROOT,
                        l_type_color[enumtoint(DriverConst::FileType::FILETYPE_ROOT)]
                );
                vdi.push_back(di0);

                ADBDriver::DirItem di1(
                        "..",
                        fs_root.parent_path().generic_string(),
                        fs_root.parent_path().generic_string(),
                        DriverConst::FileType::FILETYPE_BACK,
                        l_type_color[enumtoint(DriverConst::FileType::FILETYPE_BACK)]
                );
                vdi.push_back(di1);
            }
            else
            {
                ADBDriver::DirItem di0(
                        l_datalocaltmp_txt,
                        l_datalocaltmp_cmd,
                        l_datalocaltmp_desc,
                        DriverConst::FileType::FILETYPE_DIR,
                        l_type_color[enumtoint(DriverConst::FileType::FILETYPE_DIR)]
                );
                vdi.push_back(di0);
            }

            uint32_t vsz = vdi.size();

            ret = m_net.SyncDirList(
                    l_client,
                    root,
                    static_cast<void*>(&vdi),
                    [=](DriverNet::ADBSYNCDENT const & dent, std::string const & entry, void *v)
                    {
                        do
                        {
                            if (entry.empty())
                                break;
                            if ((!entry.compare(".")) || (!entry.compare("..")))
                                break;

                            ADBDriver::DirItem di{};
                            std::vector<ADBDriver::DirItem> *pvdi =
                                static_cast<std::vector<ADBDriver::DirItem>*>(v);
                            fs::path fs_entry = fs_root;
                            fs_entry /= entry;

                            di.s     = fs_entry.filename().generic_string();
                            di.cmds  = fs_entry.generic_string();
                            di.Type(dent.mode);
                            di.Permission(dent.mode);

                            bool isreadonly = ((!di.permission[1]) && (!di.permission[2]));
                            ADBDriver::DirItemColor & l_txt_color =
                                ((isreadonly) ?
                                        l_type_color[enumtoint(DriverConst::FileType::FILETYPE_READONLY)] :
                                        l_type_color[enumtoint(di.type)]
                                    );

                            switch (di.type)
                            {
                                case DriverConst::FileType::FILETYPE_DIR:
                                    {
                                        di.Description(
                                            fs_entry.generic_string(),
                                            "",
                                            ((isreadonly) ?
                                                l_type_desc[enumtoint(DriverConst::FileType::FILETYPE_READONLY)] :
                                                di.Permission()
                                            ),
                                            l_txt_color
                                        );
                                        break;
                                    }
                                case DriverConst::FileType::FILETYPE_FILE:
                                    {
                                        std::stringstream ss;
                                        ss << FileSize(dent.bsize);
                                        ss << l_type_desc[enumtoint(di.type)];
                                        if (isreadonly)
                                            ss << l_type_desc[enumtoint(DriverConst::FileType::FILETYPE_READONLY)] << "/";
                                        ss << di.Permission().c_str();

                                        di.Description(
                                            fs_entry.generic_string(),
                                            "",
                                            ss.str(),
                                            l_txt_color
                                        );
                                        break;
                                    }
                                case DriverConst::FileType::FILETYPE_SYMLINK:
                                    {
                                        std::stringstream ss;
                                        ss << l_type_desc[enumtoint(di.type)];
                                        if (isreadonly)
                                            ss << l_type_desc[enumtoint(DriverConst::FileType::FILETYPE_READONLY)] << "/";
                                        ss << di.Permission().c_str();

                                        di.Description(
                                            fs_entry.generic_string(),
                                            "",
                                            ss.str(),
                                            l_txt_color
                                        );
                                        break;
                                    }
                                case DriverConst::FileType::FILETYPE_CHARDEV:
                                case DriverConst::FileType::FILETYPE_BLKDEV:
                                case DriverConst::FileType::FILETYPE_FIFO:
                                case DriverConst::FileType::FILETYPE_SOCK:
                                    {
                                        di.Description(
                                            fs_entry.generic_string(),
                                            "",
                                            l_type_desc[enumtoint(di.type)],
                                            l_txt_color
                                        );
                                        di.cmds.erase();
                                        break;
                                    }
                                default:
                                    return true;
                            }
                            pvdi->push_back(di);
                        }
                        while (0);
                        return true;
                    }
                );

            if ((ret) && (vdi.size() > vsz))
            {
                int32_t cntd = std::count_if(
                                vdi.begin(),
                                vdi.end(),
                                [](ADBDriver::DirItem & di)
                                {
                                    return (di.type == DriverConst::FileType::FILETYPE_DIR);
                                }
                            ),
                        cntf = std::count_if(
                                vdi.begin(),
                                vdi.end(),
                                [](ADBDriver::DirItem & di)
                                {
                                    return (di.type == DriverConst::FileType::FILETYPE_FILE);
                                }
                            ),
                        cntl = std::count_if(
                                vdi.begin(),
                                vdi.end(),
                                [](ADBDriver::DirItem & di)
                                {
                                    return (di.type == DriverConst::FileType::FILETYPE_SYMLINK);
                                }
                            ),
                        cnto = std::count_if(
                                vdi.begin(),
                                vdi.end(),
                                [](ADBDriver::DirItem & di)
                                {
                                    return (
                                            (di.type != DriverConst::FileType::FILETYPE_DIR) &&
                                            (di.type != DriverConst::FileType::FILETYPE_FILE) &&
                                            (di.type != DriverConst::FileType::FILETYPE_SYMLINK)
                                        );
                                }
                            );

                std::stringstream ss;
                ss << fs_root.generic_string().c_str();
                ss << " -> dir: "   << cntd;
                ss << ", files: "   << cntf;
                ss << ", symlink: " << cntl;
                ss << ", other: "   << cnto;
                rs = ss.str();
            }
            else if (ret)
                rs.assign(l_root_empty);
            else
                rs.assign(l_root_error);
        }
        while (0);

        m_net.Close(l_client);
        return ret;
    }

#endif

};

