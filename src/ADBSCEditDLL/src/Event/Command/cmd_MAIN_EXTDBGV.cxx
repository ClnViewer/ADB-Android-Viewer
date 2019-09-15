_CMD_BUILD(IDM_MAIN_EXTDBGV,

    m_luae.setdbgview(
            !m_luae.getdbgview()
        );
    m_tb.event(id, m_luae.getdbgview());
)
