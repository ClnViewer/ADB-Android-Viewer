
    ///
    ///  class:
    ///     showhelp(),
    ///     setdebugline(),
    ///     findtext(),
    ///     navigate()


    void EditBox::showhelp()
    {
        try
        {
            setannotation(g_scedit_help, 0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }

    }

    void EditBox::gotoline(int32_t ln)
    {
        try
        {
            int32_t pend = cmd(SCI_GETLINECOUNT);
            ln = ((ln >= pend) ? pend : ((ln > 0) ? (ln - 1) : 0));
            cmd(SCI_GOTOLINE, ln);
            cmd(SCI_SETFOCUS, 1);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
    }

    void EditBox::textinsert(std::string const &s)
    {
        try
        {
            if (s.empty())
                return;

            cmd(SCI_INSERTTEXT,  cmd(SCI_GETCURRENTPOS), reinterpret_cast<LPARAM>(s.c_str()));
            cmd(SCI_SETFOCUS, 1);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
    }

    bool EditBox::textreplace(std::string const &s, std::string const &sr)
    {
        try
        {
            do
            {
                if ((s.empty()) || (sr.empty()))
                    break;

                Sci_TextToFind ttf{};
                int32_t endpos = cmd(SCI_GETLENGTH);
                ttf.lpstrText = s.c_str();

                switch (search_data.direct)
                {
                    case SCI_SETSELECTIONSTART:
                        {
                            ttf.chrg.cpMin = search_data.lastpos;
                            ttf.chrg.cpMax = cmd(SCI_GETCURRENTPOS);
                            break;
                        }
                    case SCI_SETSELECTIONEND:
                        {
                            ttf.chrg.cpMin = ((!search_data.lastpos) ? cmd(SCI_GETCURRENTPOS) : search_data.lastpos);
                            ttf.chrg.cpMax = endpos;
                            break;
                        }
                    case SCI_SETSELALPHA:
                        {
                            ttf.chrg.cpMin = search_data.lastpos;
                            ttf.chrg.cpMax = endpos;
                            break;
                        }
                    default:
                        {
                            ALERTBox(l_searchErrorData);
                            return false;
                        }
                }

                if (cmd(SCI_FINDTEXT, search_data.flag, reinterpret_cast<LPARAM>(&ttf)) == -1)
                    break;

                cmd(SCI_SETSEL, ttf.chrgText.cpMin, ttf.chrgText.cpMax);
                cmd(SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(sr.c_str()));
                search_data.lastpos = ((endpos > ttf.chrgText.cpMax) ? (ttf.chrgText.cpMax + 1) : ttf.chrgText.cpMax);
                search_data.wordlen = (ttf.chrgText.cpMax - ttf.chrgText.cpMin);
                //
                return true;
            }
            while (0);
            search_data = {};
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return false;
    }

    void EditBox::textnew(std::string const & s, bool isclear)
    {
        try
        {
            if (s.empty())
                return;

            cmd(SCI_CLEARALL);
            if (isclear)
            {
                cmd(SCI_SETUNDOCOLLECTION, 1);
                cmd(EM_EMPTYUNDOBUFFER);
            }
            cmd(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(s.c_str()));
            cmd(SCI_SETSAVEPOINT);
            cmd(SCI_GOTOPOS, 0);
            cmd(SCI_SETFOCUS, 1);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
    }

    void EditBox::setannotation(EditBox::AnnotateData *ad_)
    {
        if (!ad_)
            return;
        setannotation(ad_->text, ad_->line);
    }

    void EditBox::setannotation(std::string const & s, int32_t line_)
    {
        if ((cmd(SCI_ANNOTATIONGETVISIBLE)) || (s.empty()))
        {
            cmd(SCI_ANNOTATIONSETVISIBLE,  ANNOTATION_HIDDEN);
            cmd(SCI_ANNOTATIONCLEARALL);
        }
        else
        {
            cmd(SCI_ANNOTATIONSETTEXT,     line_, reinterpret_cast<LPARAM>(s.c_str()));
            cmd(SCI_ANNOTATIONSETVISIBLE,  ANNOTATION_BOXED);
        }
    }

    void EditBox::setdebugline(int32_t n)
    {
        int32_t i,
                mask   = 3,
                line   = cmd(SCI_LINEFROMPOSITION, cmd(SCI_GETLENGTH));
                mask   = (1 << mask);

        for (i = 0; i < line; i++)
        {
            if (cmd(SCI_MARKERGET, i) == mask)
            {
                cmd(SCI_MARKERDELETE, i, 3);
                break;
            }
        }
        if ((n > line) || (n < 0))
            return;

        cmd(SCI_MARKERADD, n, 3);
        cmd(SCI_ENSUREVISIBLEENFORCEPOLICY, n);
        cmd(SCI_GOTOLINE, n);
    }

    void EditBox::findtext(std::string const & s)
    {
        int32_t pbegin,
                pend = cmd(SCI_GETLENGTH);

        if (m_txtdata.findText.wordlen)
        {
            int32_t mask = (1 << _FIND_INDICATOR);
            for (pbegin = 0; pbegin < pend; pbegin++)
            {
                if ((mask & cmd(SCI_INDICATORALLONFOR, pbegin)) == mask)
                {
                    cmd(SCI_INDICATORCLEARRANGE, pbegin, m_txtdata.findText.wordlen);
                    pbegin += (m_txtdata.findText.wordlen + 1);
                }
            }
        }

        m_txtdata.findText = {};
        if (s.empty())
            return;

        cmd(SCI_SETTARGETSTART, 0);
        cmd(SCI_SETTARGETEND, pend);
        cmd(SCI_SETSEARCHFLAGS, SCFIND_MATCHCASE);
        cmd(SCI_SETINDICATORCURRENT, _FIND_INDICATOR);

        while ((pbegin = cmd(SCI_SEARCHINTARGET, s.length(), reinterpret_cast<LPARAM>(s.c_str()))) != -1)
        {
            if (!m_txtdata.findText.wordlen)
                m_txtdata.findText.wordlen = static_cast<int32_t>(s.length());
            cmd(SCI_INDICATORFILLRANGE, pbegin, m_txtdata.findText.wordlen);
            cmd(SCI_SETTARGETSTART, (pbegin + m_txtdata.findText.wordlen + 1));
            cmd(SCI_SETTARGETEND, pend);
        }
    }

    LRESULT EditBox::navigate()
    {
        int32_t next = -1,
                pend = cmd(SCI_GETLENGTH),
                line = cmd(SCI_LINEFROMPOSITION, cmd(SCI_GETCURRENTPOS));

        ::SetFocus(m_hwnd);

        /// Find text
        if (m_txtdata.findText.wordlen)
        {
            int32_t mask = (1 << _FIND_INDICATOR);
            for (; m_txtdata.findText.lastpos < pend; m_txtdata.findText.lastpos++)
            {
                if ((mask & cmd(SCI_INDICATORALLONFOR, m_txtdata.findText.lastpos)) == mask)
                {
                    line = cmd(SCI_LINEFROMPOSITION, m_txtdata.findText.lastpos);
                    cmd(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
                    cmd(SCI_GOTOLINE, line);
                    m_txtdata.findText.lastpos += (m_txtdata.findText.wordlen + 1);
                    if (m_txtdata.findText.lastpos >= pend)
                        m_txtdata.findText.lastpos = 0;
                    return 0;
                }
            }
            m_txtdata.findText.lastpos = 0;
            return navigate(); // warning! loop!
        }

        /// Book Marks
        do
        {
            int32_t mask = 1,
                    sline;

            if (pend > line)
                sline = (line + 1);
            else
                sline = 0;

            mask = (1 << mask);
            if ((next = cmd(SCI_MARKERNEXT, sline, mask)) < 0)
            {
                sline = 0;
                if ((next = cmd(SCI_MARKERNEXT, sline, mask)) < 0)
                    break;
            }
            cmd(SCI_ENSUREVISIBLEENFORCEPOLICY, next);
            cmd(SCI_GOTOLINE, next);
            return 0;
        }
        while (0);

        /// Fixed scroll: start -> cursor -> end positions
        switch (m_txtdata.scrollStep)
        {
            case 0:
                {
                    m_txtdata.scrollStep = 1;
                    cmd(SCI_SETFIRSTVISIBLELINE);
                    break;
                }
            case 1:
                {
                    m_txtdata.scrollStep = 2;
                    int32_t dvis = cmd(SCI_LINESONSCREEN);

                    if (dvis >= line)
                        return navigate();

                    cmd(SCI_SCROLLCARET);
                    break;
                }
            case 2:
                {
                    m_txtdata.scrollStep = 0;
                    cmd(SCI_LINESCROLL, 0, cmd(SCI_GETLINECOUNT));
                    break;
                }
            default:
                {
                    m_txtdata.scrollStep = 0;
                    break;
                }
        }
        return 0;
    }
