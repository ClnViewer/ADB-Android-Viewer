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
