
    __LUA_FUNC_EXT_TEMPLATE(stateSet,

        __LUA_LINT_UNUSED(ret);
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
        )
        //
        if ((args != 1) ||
            (!lua_isnumber(L, -1)))
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
            )
            break;
        }
        le->m_laststate = lua_tonumber(L, -1);
        //
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_24, le->m_laststate.load());
        )
    )

