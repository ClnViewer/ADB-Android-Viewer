
case IDM_TOOLBOX_SHOW_HELP:
case IDM_TOOLBOX_SHOW_API:
case IDM_TOOLBOX_SHOW_MONITOR:
{
    auto a = m_mgr.find(
                static_cast<uint32_t>(
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_TOOLS
                )
        );
    if (!a)
        a = MDIApp::m_mgr.add<Editor::ToolBox>();
    if (!a)
        break;

    uint32_t idx;
    switch (id)
    {
        case IDM_TOOLBOX_SHOW_HELP:
            idx = static_cast<uint32_t>(Editor::ToolBox::TabIndex::ITAB_HELP);    break;
        case IDM_TOOLBOX_SHOW_API:
            idx = static_cast<uint32_t>(Editor::ToolBox::TabIndex::ITAB_API);     break;
        case IDM_TOOLBOX_SHOW_MONITOR:
            idx = static_cast<uint32_t>(Editor::ToolBox::TabIndex::ITAB_MONITOR); break;
        default:
            return;
    }
    a->show(idx);
    break;
}
