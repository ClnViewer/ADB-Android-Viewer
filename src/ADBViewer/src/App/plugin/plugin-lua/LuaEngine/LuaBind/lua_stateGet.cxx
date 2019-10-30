
    __LUA_FUNC_STATIC_TEMPLATE(stateGet,

        __LUA_LINT_UNUSED(args);
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
        )
        //
        ::lua_pushinteger(L, le->m_laststate.load());
        ret++;
        //
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_25, le->m_laststate.load());
        )
        return;
    )
