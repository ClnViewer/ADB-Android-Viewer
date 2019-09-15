_CMD_BUILD(IDM_REBAR_FIND_TEXT,

    std::string s = m_tb.getfind();
    m_hedit.findtext(s);
    m_tb.event(id, !s.empty());
)
