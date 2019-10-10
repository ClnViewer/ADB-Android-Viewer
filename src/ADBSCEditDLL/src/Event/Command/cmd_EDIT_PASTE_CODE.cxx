
_CMD_BUILD_RANGE(IDM_EDIT_PASTE_CODE_0, IDM_EDIT_PASTE_CODE_12,

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
    m_hedit.textinsert(s);
)
