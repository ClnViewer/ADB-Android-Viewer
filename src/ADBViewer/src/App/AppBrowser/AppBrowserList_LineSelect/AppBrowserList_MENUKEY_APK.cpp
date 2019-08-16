
case AppBrowserPage::MenuKey::MENUKEY_APK:
{
    switch (popupmenu_apk())
    {
        case -1:
            break;
        case ID_CMD_POP_MENU263:
            {
                AppSysDialog::cliptextset(
                        guiBase::GetGui<SDL_Window>(),
                        ((m_drawitems[src->sel].desc.empty()) ?
                         m_drawitems[src->sel].s :
                         m_drawitems[src->sel].desc
                        )
                );
                break;
            }
        case ID_CMD_POP_MENU264:
            {
                if (m_drawitems[src->sel].cmds.empty())
                    break;

                std::string rs;
                std::stringstream ss;

                if (AppConfig::instance().cnf_adb.UnInstallApk(
                                m_drawitems[src->sel].cmds,
                                rs
                        ))
                {
                    ss << ResManager::stringload(
                            ResManager::IndexStringResource::RES_STR_BROWSER_APK_OK,
                            AppConfig::instance().cnf_lang
                        ) << "'" <<  m_drawitems[src->sel].cmds << "'";

                    listdelete(src);
                    draw();
                }
                else
                {
                    ss << ResManager::stringload(
                            ResManager::IndexStringResource::RES_STR_BROWSER_APK_ERROR,
                            AppConfig::instance().cnf_lang
                        ) << "'" <<  m_drawitems[src->sel].cmds << "'!";
                }
                if (!rs.empty())
                    ss << " - " << rs.c_str();
                if (!ss.str().empty())
                    AddMessageQueue(ss.str(), 5U, -1);
                break;
            }
        default:
            break;
    }
    break;
}
