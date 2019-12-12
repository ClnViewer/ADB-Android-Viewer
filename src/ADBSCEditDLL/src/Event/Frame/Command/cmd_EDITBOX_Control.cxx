//
// IDM_EDIT_*
//
case IDM_EVENT_EDIT_FINDTEXT:
{
    std::string s = tbc->event();
    ebc->findtext(s);
    tbc->event(id, !s.empty());
    break;
}
case IDM_EVENT_TXT_INSERT:
{
    const char *txt =
            reinterpret_cast<const char*>(lParam);
    if (!txt)
        break;
    ebc->textinsert(txt);
    break;
}
case IDM_EVENT_TXT_NEW:
{
    const char *txt =
            reinterpret_cast<const char*>(lParam);
    if (!txt)
        break;
    ebc->textnew(txt, false);
    break;
}
case IDM_EDIT_NEW:
{
    ebc->filedefault();
    break;
}
case IDM_EDIT_OPEN:
{
    (void) ebc->fileopen();
    break;
}
case IDM_EDIT_SAVE:
case IDM_EDIT_SAVEAS:
{
    MDIWin::Config::instance().issaved = true;
    (void) ebc->filesave(
            ((id == IDM_EDIT_SAVE) ?
             MDIWin::Config::instance().savepath : ""
            )
    );
    break;
}
case IDM_EDIT_REDO:
{
    ebc->command(SCI_REDO);
    break;
}
case IDM_EDIT_UNDO:
{
    ebc->command(WM_UNDO);
    break;
}
case IDM_EDIT_COPY:
{
    ebc->command(WM_COPY);
    break;
}
case IDM_EDIT_CUT:
{
    ebc->command(WM_CUT);
    break;
}
case IDM_EDIT_DELETE:
{
    ebc->command(WM_CLEAR);
    break;
}
case IDM_EDIT_SELECTALL:
{
    if (MDIWin::Config::instance().isselect)
    {
        MDIWin::Config::instance().isselect = false;
        ebc->command(SCI_SETSEL);
    }
    else
    {
        MDIWin::Config::instance().isselect = true;
        ebc->command(SCI_SELECTALL);
    }
    break;
}
case IDM_EDIT_PASTE:
{
    ebc->command(WM_PASTE);
    break;
}
case IDM_EDIT_PASTE_CODE_0 ... IDM_EDIT_PASTE_CODE_12:
{
    std::string s;
    switch (id)
    {
        case IDM_EDIT_PASTE_CODE_0:  s = g_scedit_EDIT_PASTE_CODE_0;  break;
        case IDM_EDIT_PASTE_CODE_1:  s = g_scedit_EDIT_PASTE_CODE_1;  break;
        case IDM_EDIT_PASTE_CODE_2:  s = g_scedit_EDIT_PASTE_CODE_2;  break;
        case IDM_EDIT_PASTE_CODE_3:  s = g_scedit_EDIT_PASTE_CODE_3;  break;
        case IDM_EDIT_PASTE_CODE_4:  s = g_scedit_EDIT_PASTE_CODE_4;  break;
        case IDM_EDIT_PASTE_CODE_5:  s = g_scedit_EDIT_PASTE_CODE_5;  break;
        case IDM_EDIT_PASTE_CODE_6:  s = g_scedit_EDIT_PASTE_CODE_6;  break;
        case IDM_EDIT_PASTE_CODE_7:  s = g_scedit_EDIT_PASTE_CODE_7;  break;
        case IDM_EDIT_PASTE_CODE_8:  s = g_scedit_EDIT_PASTE_CODE_8;  break;
        case IDM_EDIT_PASTE_CODE_9:  s = g_scedit_EDIT_PASTE_CODE_9;  break;
        case IDM_EDIT_PASTE_CODE_10: s = g_scedit_EDIT_PASTE_CODE_10; break;
        case IDM_EDIT_PASTE_CODE_11: s = g_scedit_EDIT_PASTE_CODE_11; break;
        case IDM_EDIT_PASTE_CODE_12: s = g_scedit_EDIT_PASTE_CODE_12; break;
        default:
            return;
    }
    ebc->textinsert(s);
    break;
}
//
// IDM_EDIT_SHOW_*
//
case IDM_EVENT_EDIT_ANNOTATION:
{
    EditBox::AnnotateData *ad =
            reinterpret_cast<EditBox::AnnotateData*>(lParam);
    if (!ad)
        break;
    ebc->setannotation(ad);
    break;
}
case IDM_EVENT_EDIT_DBGLINE:
{
    int32_t n = static_cast<int32_t>(lParam);
    ebc->setdebugline(n);
    break;
}
case IDM_EDIT_SHOW_ENDLINE:
{
    tbc->event(id, ebc->vieweol());
    break;
}
case IDM_EDIT_SHOW_INDENTG:
{
    MDIWin::Config::instance().isgridshow =
            !(MDIWin::Config::instance().isgridshow.load());

    if (MDIWin::Config::instance().isgridshow)
        ebc->command(SCI_SETINDENTATIONGUIDES);
    else
        ebc->command(SCI_SETINDENTATIONGUIDES, 1);

    tbc->event(id, MDIWin::Config::instance().isgridshow.load());
    break;
}
case IDM_EDIT_SHOW_NAVIGATE:
{
    if (m_luae.stepnextline())
        break;
    ebc->navigate();
    break;
}
case IDM_EDIT_GOTO_LINE:
{
    try
    {
        GUI::CustomDialog cdlg;
        cdlg.SetTitle("Go to line:");
        cdlg.SetCtrl(
            {
                { true, IDC_EDIT1, std::string(), "line number" }
            }
        );
        if (cdlg.Show(
                MDIWin::Config::instance().gethinstance(),
                MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
                IDD_DIALOG_1NUM
            ) != IDOK)
            break;

        auto v = cdlg.GetCtrlValue();
        if (!v.size())
            break;

        int32_t ln = std::stoi(v[0].c_str());
        if (ln >= 0)
            ebc->gotoline(ln);
    }
    catch(...) {}
    break;
}
case IDM_EDIT_COLOR_INSERT:
{
    try
    {
        GUI::ColorDialog clrd;
        if (!clrd.Show(
                MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>()
            ))
            break;

		std::string txt = clrd.GetCtrlString();
		if (!txt.empty())
            ebc->textinsert(txt);
    }
    catch(...) {}
    break;
}
case IDM_EDIT_TEXT_REPLACE:
{
    try
    {
        static const uint32_t rb_group[][2] =
        {
            { IDC_RADIO_MWORD, IDC_RADIO_MCASE },
            { IDC_RADIO_DDOWN, IDC_RADIO_DUP }
        };
        std::string s = tbc->event();
        //
        GUI::CustomDialog cdlg;
        cdlg.SetTitle("Search & Replace");
        cdlg.SetCtrl(
            {
                { false, IDC_EDIT1, s, "search" },
                { false, IDC_EDIT2, std::string(), "replace" }
            });

        cdlg.SetOnCreate(
                [&](GUI::CustomDialog *p, HWND, uint32_t, uint32_t)
                {
                    p->RadioBtnSetCheck(rb_group[0][0], true);
                    p->RadioBtnSetCheck(rb_group[1][0], true);
                    ebc->search_data        = {};
                    ebc->search_data.flag   = SCFIND_WHOLEWORD;
                    ebc->search_data.direct = SCI_SETSELECTIONEND;
                    return true;
                });

        cdlg.SetOnCommand(
                IDC_FINDFIRST,
                [&](GUI::CustomDialog *p, HWND, uint32_t, uint32_t)
                {
                    auto s_ = p->GetCtrlText(IDC_EDIT1);
                    if (!s_.empty())
                        ebc->findtext(s_);
                    return true;
                });

        cdlg.SetOnCommand(
                IDC_REPLACEONCE, IDC_REPLACEALL,
                [&](GUI::CustomDialog *p, HWND, uint32_t id, uint32_t)
                {
                    auto ss_ = p->GetCtrlText(IDC_EDIT1);
                    if (ss_.empty())
                        return true;
                    auto sr_ = p->GetCtrlText(IDC_EDIT2);
                    if (sr_.empty())
                        return true;

                    switch (id)
                    {
                        case IDC_REPLACEONCE:
                            {
                                if (!ebc->textreplace(ss_, sr_))
                                {
                                    if (ebc->search_data.direct == SCI_SETSELECTIONEND)
                                        INFOBox(g_scedit_savedearch1);
                                    else if (ebc->search_data.direct == SCI_SETSELECTIONSTART)
                                        INFOBox(g_scedit_savedearch2);
                                    else
                                        INFOBox(g_scedit_savedearch3);
                                }
                                break;
                            }
                        case IDC_REPLACEALL:
                            {
                                ebc->search_data.direct = SCI_SETSELALPHA;
                                uint32_t cnt = 0U;
                                while (ebc->textreplace(ss_, sr_)) { cnt++; }
                                if (!cnt)
                                    INFOBox(g_scedit_savedearch3);
                                p->Command(WM_COMMAND, IDOK);
                                break;
                            }
                        default:
                            break;
                    }
                    return true;
                });

        cdlg.SetOnCommand(
                IDC_RADIO_MWORD, IDC_RADIO_DUP,
                [&](GUI::CustomDialog *p, HWND, uint32_t id, uint32_t cmd)
                {
                    if (cmd != BN_CLICKED)
                        return true;

                    uint32_t x;

                    switch (id)
                    {
                        case IDC_RADIO_MWORD:
                        case IDC_RADIO_MCASE: x = 0; break;
                        case IDC_RADIO_DDOWN:
                        case IDC_RADIO_DUP:   x = 1; break;
                        default: return true;
                    }

                    p->RadioBtnGroupCheck(rb_group[x][0], rb_group[x][1], id);

                    if (x == 0)
                    {
                        if (id == rb_group[0][0])
                            ebc->search_data.flag = SCFIND_WHOLEWORD;
                        else
                            ebc->search_data.flag = SCFIND_MATCHCASE;
                    }
                    else if (x == 1)
                    {
                        if (id == rb_group[1][0])
                            ebc->search_data.direct = SCI_SETSELECTIONEND;
                        else
                            ebc->search_data.direct = SCI_SETSELECTIONSTART;
                    }
                    return true;
                });

        (void) cdlg.Show(
                    MDIWin::Config::instance().gethinstance(),
                    MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
                    IDD_DIALOG_2SR
            );
    }
    catch(...) {}
    break;
}
