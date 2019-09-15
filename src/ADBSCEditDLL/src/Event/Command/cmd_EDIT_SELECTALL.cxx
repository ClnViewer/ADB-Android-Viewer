_CMD_BUILD(IDM_EDIT_SELECTALL,
                                if (m_isselect)
                                {
                                    m_isselect = false;
                                    m_hedit.command(SCI_SETSEL);
                                }
                                else
                                {
                                    m_isselect = true;
                                    m_hedit.command(SCI_SELECTALL);
                                }
)
