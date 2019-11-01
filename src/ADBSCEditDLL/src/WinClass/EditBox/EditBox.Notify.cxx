
    ///
    ///  macro:
    ///     SC_GET_CURRENT_WORD()
    ///  class:
    ///     notify()

#define SC_GET_CURRENT_WORD(A,B,C,D)                                                       \
        {                                                                                  \
            (A).chrg.cpMin = cmd(SCI_WORDSTARTPOSITION, (B));                              \
            (A).chrg.cpMax = cmd(SCI_WORDENDPOSITION,   (B));                              \
            (A).lpstrText  = (C).get();                                                    \
            if (D)                                                                         \
                (A).chrg.cpMax = (((A).chrg.cpMax > 0 ) ? ((A).chrg.cpMax - 1) : 0);       \
            if (!(A).chrg.cpMax)                                                           \
                break;                                                                     \
            cmd(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&(A)));                      \
        }

    void EditBox::event(void *v)
    {
        SCNotification *notify = reinterpret_cast<SCNotification*>(v);
        if (!notify)
            return;

        switch (notify->margin)
        {
            case _INDEX_MARGIN_FOLD:
                {
                    int32_t idx =  cmd(SCI_LINEFROMPOSITION, notify->position, 0);
                    cmd(SCI_TOGGLEFOLD, idx, 0);
                    return;
                }
            default:
                {
                    break;
                }
        }
        switch (notify->nmhdr.code)
        {
            case SCN_SAVEPOINTREACHED:
                {
                    break;
                }
            case SCN_SAVEPOINTLEFT:
                {
                    break;
                }
            case SCN_UPDATEUI:
                {
                    break;
                }
            case SCN_MARGINCLICK:
                {
                    switch (notify->margin)
                    {
                        case _INDEX_MARGIN_LNUM:
                            {
                                int32_t mask = 1,
                                        line = cmd(SCI_LINEFROMPOSITION, notify->position);
                                        mask = (1 << mask);

                                if (cmd(SCI_MARKERGET, line) == mask)
                                    cmd(SCI_MARKERDELETE, line, 1);
                                else
                                    cmd(SCI_MARKERADD, line, 1);
                                break;
                            }
                        default:
                            break;
                    }
                    break;
                }
            case SCN_CHARADDED:
                {
                    m_txtdata.lastChar = static_cast<int32_t>(notify->ch);

                    switch (notify->ch)
                    {
                        case ':':
                        case ',':
                        case ')':
                        case '(':
                        case '\r':
                        case '\n': break;
                        case '[':
                        case '{':
                        case '\"':
                        case '\'': serviceAutoAppend(); return;
                        default:  return;
                    }

                    int32_t pos = 0;
                    Sci_TextRange tr{};
                    auto dbuf = std::make_unique<char[]>(1024);

                    switch (notify->ch)
                    {
                        case ':':
                        case '(':
                            {
                                // base get position, range and text
                                pos = cmd(SCI_GETCURRENTPOS);
                                SC_GET_CURRENT_WORD(tr, (pos - 1), dbuf, true);
                                break;
                            }
                        case '\r':
                        case '\n':
                            {
                                // base get/set auto indent
                                do
                                {
                                    int32_t indent_d,
                                            indent_l,
                                            indent_n = 0,
                                            line_l,
                                            len_l;

                                    line_l = (cmd(SCI_LINEFROMPOSITION, cmd(SCI_GETCURRENTPOS)) - 1);
                                    if (line_l < 0)
                                        break;

                                    indent_d = cmd(SCI_GETINDENT);
                                    if (indent_d <= 0)
                                        break;

                                    indent_l = cmd(SCI_GETLINEINDENTATION, line_l);
                                    len_l    = cmd(SCI_GETLINE, line_l, reinterpret_cast<LPARAM>(dbuf.get()));
                                    if (indent_l < indent_d)
                                    {
                                        if (len_l < 8)
                                        break;

                                        std::string s(dbuf.get());
                                        if (s.compare(0, 8, "function") == 0)
                                            indent_n = indent_d;
                                    }
                                    if (!indent_n)
                                    {
                                        if (len_l < 2)
                                            indent_n = indent_l;
                                        else
                                            switch (dbuf.get()[(len_l - 2)])
                                            {
                                                case '}': indent_n = (indent_l - indent_d); break;
                                                case '{': indent_n = (indent_l + indent_d); break;
                                                default:  indent_n = indent_l;              break;
                                            }
                                    }
                                    line_l += 1;
                                    cmd(SCI_SETLINEINDENTATION, line_l, indent_n);
                                    cmd(SCI_GOTOPOS, cmd(SCI_GETLINEENDPOSITION, line_l));
                                }
                                while (0);
                                return;
                            }
                        default:  break;
                    }

                    switch (notify->ch)
                    {
                        case ':':
                            {
                                // Auto complete - LuaObject: -> ..
                                std::string s(dbuf.get());
                                if (s.compare(l_buildLuaRoot) != 0)
                                    break;

                                cmd(SCI_AUTOCSHOW, 0, reinterpret_cast<LPARAM>(l_autoObject));
                                break;
                            }
                        case ',':
                            {
                                // Tool tip shift highlight arguments
                                serviceAutoToolTipShiftArgs();
                                break;
                            }
                        case '(':
                            {
                                // Tool tip
                                if (!serviceAutoToolTip(dbuf.get(), pos, true))
                                    serviceAutoAppend(pos);
                                break;
                            }
                        case ')':
                            {
                                // Tool tip cancel
                                m_txtdata.lightText = {};
                                if (cmd(SCI_CALLTIPACTIVE))
                                    cmd(SCI_CALLTIPCANCEL);
                                break;
                            }
                        default:
                                break;
                    }
                    break;
                }
            case SCN_USERLISTSELECTION:
                {
                    break;
                }
            case SCN_DWELLSTART:
                {
                    // Tool tip annotation is mouse pause on word
                    Sci_TextRange tr{};
                    auto dbuf = std::make_unique<char[]>(1024);

                    SC_GET_CURRENT_WORD(tr, notify->position, dbuf, false);
                    serviceAutoToolTip(dbuf.get(), notify->position);
                    break;
                }
            case SCN_DWELLEND:
                {
                    if (cmd(SCI_CALLTIPACTIVE))
                        cmd(SCI_CALLTIPCANCEL);
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
