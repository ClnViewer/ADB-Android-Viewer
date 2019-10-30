_CMD_BUILD(IDM_MAIN_DBGMODE,

    if (!m_luae.getrunbreak())
    {
        m_luae.setdbgdump(true);
        break;
    }
    m_luae.setdbgstep(
            !m_luae.getdbgstep()
        );
    m_tb.event(id, m_luae.getdbgstep());
)
