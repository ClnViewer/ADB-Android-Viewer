
    __LUA_FUNC_EXT_TEMPLATE(adbKey,

        __LUA_ALL_UNUSED(ret)
        __LUA_LINT_UNUSED(le)

        LINT_TRY__
        {
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

            int32_t k = static_cast<int32_t>(::lua_tointeger(L, -1));

            __LUA_IF_LINT(

                if (k < 0)
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);

                std::stringstream ss;
                ss << "Android key -> " << k;
                LuaLint::print_traceT(LuaLint::g_lint_str_31, ss.str());
            )
            __LUA_IF_NOT_LINT(

                if ((!le->m_adbcmd.load()) || (k < 0))
                    break;

                le->m_adbcmd.load()->key(
                    GameDev::DriverConst::KeysType::KEYS_ANDROID,
                    k
                );
            )
        }
        LINT_CATCH__
    )
