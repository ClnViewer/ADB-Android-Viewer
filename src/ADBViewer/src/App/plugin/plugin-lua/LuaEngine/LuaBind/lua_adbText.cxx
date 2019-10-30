
    __LUA_FUNC_STATIC_TEMPLATE(adbText,

        __LUA_ALL_UNUSED(ret)
        __LUA_LINT_UNUSED(le)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 1) ||
                (!lua_isstring(L, -1)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }
            //
            std::string s(lua_tostring(L, -1));
            if (s.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, "string empty");
                )
                break;
            }
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_31, s);
            )
            __LUA_IF_NOT_LINT(

                if (!le->m_adbcmd.load())
                    break;

                le->m_adbcmd.load()->text(s);
            )
        }
        LINT_CATCH__
    )
