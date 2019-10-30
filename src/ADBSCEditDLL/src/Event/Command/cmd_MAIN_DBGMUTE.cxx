_CMD_BUILD(IDM_MAIN_DBGMUTE,

    bool b = !(LuaLint::istrace());
    LuaLint::settrace(b);
    m_tb.event(id, !(b));
)
