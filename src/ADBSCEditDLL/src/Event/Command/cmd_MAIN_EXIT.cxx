
_CMD_BUILD(IDM_MAIN_EXIT,

    if ((!m_hedit.issaved()) && (!m_issave))
    {
        INFOBox("script NOT saved, press twice to exit without saving..");
        m_issave = true;
        break;
    }
    ::SendMessageA(hwnd, WM_CLOSE, wParam, lParam);
)
