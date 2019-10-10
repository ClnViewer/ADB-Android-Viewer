
    __LUA_FUNC_EXT_TEMPLATE(adbClick,

        __LUA_ALL_UNUSED(ret)
        __LUA_LINT_UNUSED(le)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 2)            ||
                (!lua_isnumber(L, -2)) ||
                (!lua_isnumber(L, -1)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            __LUA_IF_LINT(

                ImageLite::IPOINT<int32_t> p{};
                p.set<int32_t>(
                    lua_tointeger(L, -2),
                    lua_tointeger(L, -1)
                );
                if ((p.x < 0) || (p.y < 0))
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);

                std::stringstream ss;
                ss << "position -> ";
                LuaLint::stream_print_ipointT<int32_t>(ss, p);
                LuaLint::print_traceT(LuaLint::g_lint_str_31, ss.str());
            )
            __LUA_IF_NOT_LINT(

                if (!le->m_adbcmd.load())
                    break;

                GameDev::ADBDriver::Tap_s t = {
                    static_cast<int32_t>(lua_tointeger(L, -2)),
                    static_cast<int32_t>(lua_tointeger(L, -1))
                };
                if ((t.x < 0) || (t.y < 0))
                    break;
                le->m_adbcmd.load()->click(&t);
            )
        }
        LINT_CATCH__
    )
