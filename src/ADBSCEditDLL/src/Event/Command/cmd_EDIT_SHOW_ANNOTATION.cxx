_CMD_BUILD(IDM_EDIT_SHOW_ANNOTATION,

    EditBox::AnnotateData *ad =
            reinterpret_cast<EditBox::AnnotateData*>(lParam);
    m_hedit.setannotation(ad);
)
