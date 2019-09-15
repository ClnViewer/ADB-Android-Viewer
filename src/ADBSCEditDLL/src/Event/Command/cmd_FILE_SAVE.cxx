_CMD_BUILD2(IDM_FILE_SAVE, IDM_FILE_SAVEAS,

    m_issave = true;
    std::string fname = m_hedit.filesave(
                    ((id == IDM_FILE_SAVE) ? m_savepath : "")
           );
    if (!fname.empty())
        m_savepath = fname;
)

