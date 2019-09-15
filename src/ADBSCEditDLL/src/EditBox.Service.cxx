
    ///
    /// static:
    ///     f_baseAutoHighlight(),
    ///     f_descriptionAutoHighlight()
    ///  class:
    ///     serviceAutoAppend(),
    ///     serviceAutoToolTip(),
    ///     serviceAutoToolTipShiftArgs(),
    ///     serviceAddMark(),

    static bool f_baseAutoHighlight(EditBox::AutoHighlight & a_, std::string const & s_)
    {
        do
        {
            a_ = {};

            if (s_.empty())
                break;

            std::string s(s_);
            std::size_t sz;

            if ((sz = s.find_first_of('(')) == std::string::npos)
                break;

            a_.base = (sz += 1U);
            s  = s.substr(sz, s.length() - sz);
            if ((sz = s.find_first_of(')')) == std::string::npos)
                break;

            s  = s.substr(0, sz);
            std::istringstream iss(s);
            while (std::getline(iss, s, ','))
            {
                if (s.empty())
                    break;
                if (a_.count == __NELE(a_.range))
                    break;
                a_.range[a_.count] = s.length() + 1U;
                a_.count++;
            }
            return true;
        }
        while (0);
        a_ = {};
        return false;
    }

    static bool f_descriptionAutoHighlight(EditBox::AutoHighlight & a_, std::string const & s_)
    {
        do
        {
            a_ = {};

            if (s_.empty())
                break;

            std::size_t sz;

            if ((sz = s_.find_first_of(' ')) == std::string::npos)
                break;

            a_.base = (sz + 1U);

            if ((sz = s_.find_first_of('(')) == std::string::npos)
                break;

            a_.range[0] = ((sz - a_.base) + 1U);
            a_.count    = 1U;
            return true;
        }
        while (0);
        a_ = {};
        return false;
    }

    void EditBox::serviceAutoAppend(int32_t pos)
    {
        char cr = 0;

        switch (m_data.lastChar)
        {
            case '(':  cr = ')';  break;
            case '[':  cr = ']';  break;
            case '{':  cr = '}';  break;
            case '\"': cr = '\"'; break;
            case '\'': cr = '\''; break;
            default: return;
        }
        const char para[] = { cr, '\0' };

        if (!pos)
            pos = cmd(SCI_GETCURRENTPOS);

        cmd(SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(para));
        cmd(SCI_SETCURRENTPOS, pos);
        cmd(SCI_SETSEL, pos, pos);
    }

    bool EditBox::serviceAutoToolTip(std::string const & s, int32_t pos, bool isargs)
    {
        do
        {
            auto r = l_toolTip.find(s);
            if (r == l_toolTip.end())
                return false;

            if (!pos)
                pos = cmd(SCI_GETCURRENTPOS);

            cmd(SCI_CALLTIPSHOW, pos, reinterpret_cast<LPARAM>(r->second.data()));

            if (!isargs)
            {
                if (!f_descriptionAutoHighlight(m_data.lightText, r->second))
                    break;
            }
            else
            {
                if (!f_baseAutoHighlight(m_data.lightText, r->second))
                    break;
            }

            if (m_data.lightText.count)
                cmd(
                    SCI_CALLTIPSETHLT,
                    m_data.lightText.base,
                    (m_data.lightText.base + m_data.lightText.range[m_data.lightText.curent++] - 1)
                );
        }
        while (0);
        return true;
    }

    void EditBox::serviceAutoToolTipShiftArgs()
    {
        do
        {
            if (!m_data.lightText.count)
                break;

            if (!m_data.lightText.count)
            {
                cmd(SCI_CALLTIPSETHLT, 0, 0);
                break;
            }

            uint32_t n, off = m_data.lightText.base;
            for (n = 0U; ((n < m_data.lightText.count) && (n < m_data.lightText.curent)); n++)
                off += m_data.lightText.range[n];

            cmd(
                SCI_CALLTIPSETHLT,
                off,
                (off + m_data.lightText.range[m_data.lightText.curent++] - 1)
            );
        }
        while (0);
    }

    int32_t EditBox::serviceAddMark(int32_t n, int32_t m, const char *img[])
    {
        if (!m)
            m = cmd(SCI_GETMARGINMASKN, _INDEX_MARGIN_FOLD);

        cmd(SCI_MARKERDEFINEPIXMAP, n, reinterpret_cast<LPARAM>(img));
        return ((1 << n) | m);
    }

