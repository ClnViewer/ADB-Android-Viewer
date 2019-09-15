_CMD_BUILD(IDM_MAIN_WINTOP,

    m_iswintop = !(m_iswintop.load());
    ::SetWindowPos(
            WindowBase::m_hwnd,
            ((m_iswintop) ? HWND_TOPMOST : HWND_NOTOPMOST),
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE
        );

    m_tb.event(id, m_iswintop);

    std::stringstream ss;
    ss << " -- set SCEdit ";
    if (m_iswintop)
        ss << "always top";
    else
        ss << "normal mode";

    DEFAULTBox(ss.str());
)
