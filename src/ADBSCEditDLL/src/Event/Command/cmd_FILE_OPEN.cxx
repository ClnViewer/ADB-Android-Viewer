_CMD_BUILD(IDM_FILE_OPEN,
                                std::string fn = m_hedit.fileopen();
                                if (!fn.empty())
                                    m_savepath = fn;
)
