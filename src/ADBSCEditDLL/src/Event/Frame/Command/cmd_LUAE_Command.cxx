
case IDM_BTN_DBGBREAK:
{
    m_luae.setrunbreak(true);
    break;
}
case IDM_BTN_DBGCYCLE:
{
    m_luae.setdbgstep(false);
    tbc->event(id, false);
    break;
}
case IDM_BTN_DBGDUMP:
{
    m_luae.setdbgdump(true);
    break;
}
case IDM_BTN_DBGMODE:
{
    if (!m_luae.getrunbreak())
    {
        m_luae.setdbgdump(true);
        break;
    }
    m_luae.setdbgstep(
            !m_luae.getdbgstep()
    );
    tbc->event(id, m_luae.getdbgstep());
    break;
}
case IDM_BTN_DBGMUTE:
{
    bool b = !(LuaLint::istrace());
    LuaLint::settrace(b);
    tbc->event(id, !(b));
    break;
}
case IDM_BTN_DBGNEXT:
{
    m_luae.stepnextline();
    break;
}
case IDM_BTN_DBGSTEP:
{
    m_luae.setdbgstep(true);
    tbc->event(id, false);
    break;
}
case IDM_BTN_EXTDBGV:
{
    m_luae.setdbgview(
            !m_luae.getdbgview()
    );
    tbc->event(id, m_luae.getdbgview());
    break;
}

case IDM_EVENT_SCRUN_START:
{
    tbc->event(id, m_luae.getdbgstep());
    break;
}
case IDM_EVENT_SCRUN_STOP:
{
    tbc->event(id, false);
    //
    auto a = m_mgr.find(
                static_cast<uint32_t>(
                    MDIWin::BaseData::MDIWinType::MWTYPE_CHILD_TOOLS
                )
        );
    if (!a)
        break;
    a->show(std::vector<std::string>());
    a->close();
    break;
}
case IDM_EVENT_SCRUN_MONITOR:
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
    a->show(
        static_cast<uint32_t>(Editor::ToolBox::TabIndex::ITAB_MONITOR)
    );
    EditBox::AnnotateData *ad =
            reinterpret_cast<EditBox::AnnotateData*>(lParam);
    if (!ad)
        break;
    a->show(ad->data);
    break;
}

case IDM_BTN_SCRUN_STOP:
{
    m_luae.stop();
    break;
}
case IDM_BTN_SCRUN_TEST:
case IDM_BTN_SCRUN_START:
{
    std::string s;
    if (!ebc->gettext<std::string>(s))
    {
        ALERTBox(g_scedit_load);
        break;
    }
    if (s.empty())
    {
        ALERTBox(g_scedit_empty);
        break;
    }
    switch (id)
    {
        case IDM_BTN_SCRUN_TEST:
            {
                m_luae.check(s);
                break;
            }
        case IDM_BTN_SCRUN_START:
            {
                m_luae.start(s);
                break;
            }
    }
    break;
}
case IDM_BTN_PLUGIN_FIRST ... IDM_BTN_PLUGIN_END:
{
    std::string s;
    if (!ebc->gettext<std::string>(s))
    {
        ALERTBox(g_scedit_load);
        break;
    }
    if (s.empty())
    {
        ALERTBox(g_scedit_empty);
        break;
    }
    m_luap.open(id, s);
    m_luap.run();
    m_luap.close();
    break;
}
