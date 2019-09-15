_CMD_BUILD(IDM_EDIT_SHOW_INDENTG,

    m_isgridshow = !(m_isgridshow.load());

    if (m_isgridshow)
        m_hedit.command(SCI_SETINDENTATIONGUIDES);
    else
        m_hedit.command(SCI_SETINDENTATIONGUIDES, 1);

    m_tb.event(id, m_isgridshow);
)
