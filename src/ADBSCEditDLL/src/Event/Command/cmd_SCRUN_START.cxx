_CMD_BUILD2(IDM_SCRUN_TEST, IDM_SCRUN_START,

    std::string s;
    if (!m_hedit.gettext<std::string>(s))
    {
        ALERTBox("error load script from editor panel");
        break;
    }
    switch (id)
    {
        case IDM_SCRUN_TEST:
            {
                m_luae.check(s);
                break;
            }
        case IDM_SCRUN_START:
            {
                m_luae.start(s);
                break;
            }
    }
)


