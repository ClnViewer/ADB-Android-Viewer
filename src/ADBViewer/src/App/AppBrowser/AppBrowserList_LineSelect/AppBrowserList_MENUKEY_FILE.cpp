
case AppBrowserPage::MenuKey::MENUKEY_FILE:
{
    if (m_drawitems[src->sel].cmds.empty())
        break;

    std::stringstream ss;

    switch (m_drawitems[src->sel].type)
    {
        case GameDev::DriverConst::FileType::FILETYPE_DIR:
        case GameDev::DriverConst::FileType::FILETYPE_BACK:
        case GameDev::DriverConst::FileType::FILETYPE_ROOT:
            {
                int32_t sel;
                switch (m_page->mikey)
                {
                    case AppBrowserPage::MenuInput::MENUINPUT_PC:
                        sel = ID_CMD_POP_MENU105; break;
                    case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                        sel = ID_CMD_POP_MENU106; break;
                    default:
                        sel = -1; break;

                }
                if (sel < 0)
                    break;

                clickselectfile(m_drawitems[src->sel], sel);
                break;
            }
        case GameDev::DriverConst::FileType::FILETYPE_FILE:
        case GameDev::DriverConst::FileType::FILETYPE_SYMLINK:
            {
                bool l_isaction = false;
                int32_t l_sel = popupmenu_file(
                                        static_cast<bool>(
                                                m_page->mikey == AppBrowserPage::MenuInput::MENUINPUT_ANDROID
                                        )
                                );
                switch (l_sel)
                {
                    case -1:
                        break;
                    case ID_CMD_POP_MENU260: // Move
                    case ID_CMD_POP_MENU261: // Copy
                        {
                            switch (m_page->mikey)
                            {
                                case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                    {
                                        if (AppConfig::instance().cnf_browser_dir_device.empty())
                                            break;

                                        l_isaction = AppConfig::instance().cnf_adb.FileSend(
                                                        m_drawitems[src->sel].cmds,
                                                        AppConfig::instance().cnf_browser_dir_device,
                                                        GameDev::DriverConst::FilePermissionType::PERM_RWRWRW
                                                );

                                        if ((l_sel == ID_CMD_POP_MENU260) && (l_isaction))
                                        {
                                            l_isaction = m_parser.file_delete_local(
                                                                 m_drawitems[src->sel].cmds
                                                         );

                                            if (!l_isaction)
                                                ss << ResManager::stringload(
                                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_MOVE_ERROR,
                                                        AppConfig::instance().cnf_lang
                                                    ) << m_drawitems[src->sel].cmds.c_str();
                                            else
                                                ss << ResManager::stringload(
                                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_MOVE_OK,
                                                        AppConfig::instance().cnf_lang
                                                    ) << AppConfig::instance().cnf_browser_dir_device.c_str();
                                        }
                                        else
                                        {
                                            if (!l_isaction)
                                                ss << ResManager::stringload(
                                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_SEND_ERROR,
                                                        AppConfig::instance().cnf_lang
                                                    ) << m_drawitems[src->sel].cmds.c_str();
                                            else
                                                ss << ResManager::stringload(
                                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_COPY_OK,
                                                        AppConfig::instance().cnf_lang
                                                    ) << AppConfig::instance().cnf_browser_dir_device.c_str();

                                            l_isaction = false;
                                        }
                                        break;
                                    }
                                case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                    {
                                        if (AppConfig::instance().cnf_browser_dir_local.empty())
                                            break;

                                        l_isaction = AppConfig::instance().cnf_adb.FileReceive(
                                                m_drawitems[src->sel].cmds,
                                                m_parser.filepath(
                                                    m_drawitems[src->sel],
                                                    AppConfig::instance().cnf_browser_dir_local
                                                )
                                            );
                                        l_isaction = false;
                                        break;
                                    }
                                default:
                                    break;
                            }
                            break;
                        }
                    case ID_CMD_POP_MENU262: // Delete
                        {
                            switch (m_page->mikey)
                            {
                                case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                    {
                                        l_isaction = m_parser.file_delete_local(
                                                             m_drawitems[src->sel].cmds
                                                     );
                                        break;
                                    }
                                case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                    {
                                        l_isaction = m_parser.file_delete_device(
                                                             m_drawitems[src->sel].cmds
                                                     );
                                        break;
                                    }
                                default:
                                    break;
                            }
                            if (l_isaction)
                                ss << ResManager::stringload(
                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_DELETE_OK,
                                        AppConfig::instance().cnf_lang
                                    );
                            else
                                ss << ResManager::stringload(
                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_DELETE_ERROR,
                                        AppConfig::instance().cnf_lang
                                    );

                            ss << m_drawitems[src->sel].cmds;
                            break;
                        }
                    case ID_CMD_POP_MENU263: // ClipBoard
                        {
                            AppSysDialog::cliptextset(
                                    guiBase::GetGui<SDL_Window>(),
                                    m_drawitems[src->sel].cmds
                            );
                            break;
                        }
                    case ID_CMD_POP_MENU265: // Android file permission
                    case ID_CMD_POP_MENU266:
                    case ID_CMD_POP_MENU267:
                    case ID_CMD_POP_MENU268:
                    case ID_CMD_POP_MENU269:
                    case ID_CMD_POP_MENU270:
                        {
                            if (m_page->mikey != AppBrowserPage::MenuInput::MENUINPUT_ANDROID)
                                break;

                            GameDev::DriverConst::FilePermissionType fp;

                            switch (l_sel)
                            {
                                case ID_CMD_POP_MENU265:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RW;
                                    break;
                                case ID_CMD_POP_MENU266:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RWX;
                                    break;
                                case ID_CMD_POP_MENU267:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RWRW;
                                    break;
                                case ID_CMD_POP_MENU268:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RWXRWX;
                                    break;
                                case ID_CMD_POP_MENU269:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RWRWRW;
                                    break;
                                case ID_CMD_POP_MENU270:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_RWXRWXRWX;
                                    break;
                                default:
                                    fp = GameDev::DriverConst::FilePermissionType::PERM_NONE;
                                    break;
                            }
                            if (fp == GameDev::DriverConst::FilePermissionType::PERM_NONE)
                                break;

                            std::string rs;
                            l_isaction = AppConfig::instance().cnf_adb.FileChmod(
                                    m_drawitems[src->sel].cmds,
                                    fp,
                                    rs
                            );
                            if (!rs.empty())
                                ss << ResManager::stringload(
                                        ResManager::IndexStringResource::RES_STR_BROWSER_FILE_PERM_STATUS,
                                        AppConfig::instance().cnf_lang
                                    ) << rs.c_str();
                            else
                            {
                                if (l_isaction)
                                    ss << ResManager::stringload(
                                            ResManager::IndexStringResource::RES_STR_BROWSER_FILE_PERM_OK,
                                            AppConfig::instance().cnf_lang
                                        ) << AppConfig::instance().cnf_adb.FilePermission(fp) << " : ";
                                else
                                    ss << ResManager::stringload(
                                            ResManager::IndexStringResource::RES_STR_BROWSER_FILE_PERM_ERROR,
                                            AppConfig::instance().cnf_lang
                                        );

                                ss << m_drawitems[src->sel].cmds;
                            }
                            draw();
                            l_isaction = false;
                            break;
                        }
                    default:
                        break;
                }
                if (!ss.str().empty())
                    AddMessageQueue(ss.str(), 5U, -1);

                if (l_isaction)
                {
                    listdelete(src);
                    draw();
                }
                break;
            }
        default:
            break;
    }
    break;
}
