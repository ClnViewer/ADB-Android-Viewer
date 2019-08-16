#pragma once

#include "../../ADBViewer.h"

class AppBrowserParse
{
public:
    //
    bool inf_name(std::string const&, GameDev::ADBDriver::DirItem&);
    bool apk_name(std::string const&, GameDev::ADBDriver::DirItem&);
    //
    bool dir_list_local(std::string const&, std::vector<GameDev::ADBDriver::DirItem>&);
    bool dir_list_device(std::string const&, std::vector<GameDev::ADBDriver::DirItem>&);
    //
    bool file_delete_local(std::string const&);
    bool file_delete_device(std::string const&);
    //
    std::string basedir(GameDev::ADBDriver::DirItem const &, AppBrowserPage::MenuInput);

private:
    //
    std::string m_rootpath_local;
    std::string m_rootpath_device;
};

