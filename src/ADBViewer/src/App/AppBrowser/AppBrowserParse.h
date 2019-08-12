#pragma once

#include "../../ADBViewer.h"

class AppBrowserParse
{
public:
    //
    bool inf_name(std::string const&, AppBrowserPage::DrawItem&);
    bool apk_name(std::string const&, AppBrowserPage::DrawItem&);
    bool dir_list_local(std::string const&, std::vector<AppBrowserPage::DrawItem>&);
    bool dir_list_device(std::string const&, std::vector<AppBrowserPage::DrawItem>&, std::string const&);    //    std::string basedir(AppBrowserPage::DrawItem const &, AppBrowserPage::MenuInput);

private:
    //
    std::string m_rootpath_local;
    std::string m_rootpath_device;
    //
    bool dir_item_device(std::string const&, AppBrowserPage::DrawItem&);
};

