    //
    // static DebugData output parsers
    //

    static void f_DebugData_set_(lua_State *lua_, Editor::LuaEngine::DebugData & dd)
    {
        lua_Debug ldb{};
        int32_t i, n, l = 0;

        while (lua_getstack(lua_, l++, &ldb) == 1)
        {
            if (!lua_getinfo(lua_, "nlSf", &ldb))
                continue;

            n = lua_gettop(lua_);
            lua_pop(lua_, 1);
            dd.nstack = n;

            for (i = 0; i < n; i++)
            {
                int32_t cnt = 0;
                do
                {
                    const char *vname, *val;

                    if (!(vname = lua_getlocal(lua_, &ldb, i)))
                        break;

                    cnt++;

                    if (::memcmp(vname, static_cast<const void*>(&"(*temporary)"), 12) == 0)
                        break;

                    Editor::LuaEngine::DumpData d{};
                    d.ncount = i;
                    d.stype  = lua_typename(lua_, lua_type(lua_, i));
                    d.sname  = vname;

                    if (!(val = luaL_tolstring(lua_, -1, nullptr)))
                        d.svalue = "?";
                    else
                    {
                        d.svalue = val;
                        cnt++;
                    }
                    dd.data.push_back(d);
                }
                while (0);
                //
                if (cnt)
                    lua_pop(lua_, cnt);
            }
        }
    }

    static std::string f_DebugData_format_ANOTATION(Editor::LuaEngine::DebugData & dd)
    {
        std::stringstream ss;
        ss << "* stack available: " << dd.nstack << "\n";

        for (auto & item : dd.data)
        {
            if (!item.stype.empty())
                ss << "(" << item.stype.c_str() << ") ";
            if (!item.sname.empty())
                ss << item.sname.c_str() << " = [";
            else
                ss << "??? = [";
            if (!item.svalue.empty())
                ss << item.svalue.c_str() << "]";
            else
                ss << "null]";
            ss << "\n";
        }
        return ss.str();
    }

    static std::string f_DebugData_format_DBGVIEW(Editor::LuaEngine::DebugData & dd)
    {
        std::stringstream ss;
        ss << "line: " << dd.nline << ", " << "stack: " << dd.nstack << ", ";

        for (auto & item : dd.data)
        {
            ss << "   " << item.ncount << "] - ";
            if (!item.stype.empty())
                ss << "(" << item.stype.c_str() << ") ";
            if (!item.sname.empty())
                ss << item.sname.c_str() << " = [";
            else
                ss << "??? = [";
            if (!item.svalue.empty())
                ss << item.svalue.c_str() << "]";
            else
                ss << "null]";
            ss << ", ";
        }
        std::string s = ss.str();
        if (!s.empty())
        {
            std::size_t pos = s.find_last_of(",");
            if (pos != std::string::npos)
                s.erase(pos, (s.length() - 1));
        }
        return s;
    }

    static std::vector<std::string> f_DebugData_format_LISTBOX(Editor::LuaEngine::DebugData & dd)
    {
        std::vector<std::string> v;
        {
            std::stringstream ss;
            ss << "- stack available:\t" << dd.nstack;
            v.push_back(ss.str());
        }
        for (auto & item : dd.data)
        {
            std::stringstream ss;
            //
            if (!item.stype.empty())
                ss << "[" << item.stype.c_str() << "] ";
            if (!item.sname.empty())
                ss << item.sname.c_str() << "\t=\t";
            else
                ss << "???\t=\t";
            if (!item.svalue.empty())
                ss << item.svalue.c_str() << "";
            else
                ss << "undefined";
            v.push_back(ss.str());
        }
        return v;
    }

    static void f_DebugData_send_(
                    Editor::LuaEngine::DebugData & dd,
                    LuaDebugDataOut ddout,
                    lua_Debug *ldb = nullptr)
    {
        switch (ddout)
        {
            case LuaDebugDataOut::DDT_ANOTATION:
                {
                    std::string s = f_DebugData_format_ANOTATION(dd);
                    if (s.empty())
                        return;

                    EditBox::AnnotateData ad = { dd.nline, s, {} };
                    //
                    ::SendMessageA(
                        CONF_MAIN_HANDLE(),
                        WM_COMMAND,
                        (WPARAM)IDM_EVENT_EDIT_ANNOTATION,
                        reinterpret_cast<LPARAM>(&ad)
                    );
                    break;
                }
            case LuaDebugDataOut::DDT_DBGVIEW:
                {
                    std::string s = f_DebugData_format_DBGVIEW(dd);
                    if (s.empty())
                        return;

                    ::OutputDebugStringA(s.c_str());
                    break;
                }
            case LuaDebugDataOut::DDT_LISTBOX:
                {
                    std::vector<std::string> v = f_DebugData_format_LISTBOX(dd);
                    if (!v.size())
                        return;

                    EditBox::AnnotateData ad = { dd.nline, std::string(), v };
                    //
                    ::SendMessageA(
                        CONF_MAIN_HANDLE(),
                        WM_COMMAND,
                        (WPARAM)IDM_EVENT_SCRUN_MONITOR,
                        reinterpret_cast<LPARAM>(&ad)
                    );
                    break;
                }
            case LuaDebugDataOut::DDT_TRACELINE:
                {
                    ::SendMessageA(
                        CONF_MAIN_HANDLE(),
                        WM_COMMAND,
                        (WPARAM)IDM_EVENT_EDIT_DBGLINE,
                        (LPARAM)dd.nline
                    );
                    break;
                }
            case LuaDebugDataOut::DDT_TRACEPRN:
                {
                    if (!ldb)
                        return;

                    std::stringstream ss;
                    ss << "\tcall ";
                    //
                    if (ldb->what)
                        ss << ldb->what << ":";
                    if (ldb->namewhat)
                        ss << ldb->namewhat << ":";
                    if (ldb->name)
                        ss << " -> " << ldb->name;
                    if (ldb->linedefined > 0)
                        ss << ":" << (ldb->linedefined + 1);
                    //
                    LuaLint::print(
                            ss.str(),
                            LuaLint::ColorPrint::DebugTraceStack
                    );
                    break;
                }
            default:
                return;
        }
    }
