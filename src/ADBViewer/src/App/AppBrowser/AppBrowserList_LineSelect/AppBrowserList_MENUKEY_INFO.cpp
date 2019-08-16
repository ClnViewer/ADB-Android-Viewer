
case AppBrowserPage::MenuKey::MENUKEY_INFO:
{
    if (m_drawitems[src->sel].desc.empty())
        break;

    switch (popupmenu_info())
    {
        case -1:
            break;
        case ID_CMD_POP_MENU263:
            {
                AppSysDialog::cliptextset(
                        guiBase::GetGui<SDL_Window>(),
                        m_drawitems[src->sel].desc
                );
                break;
            }
        default:
            break;
    }
    break;
}
