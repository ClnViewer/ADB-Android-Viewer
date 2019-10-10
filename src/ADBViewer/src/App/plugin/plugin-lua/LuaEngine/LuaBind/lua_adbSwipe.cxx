
    __LUA_FUNC_EXT_TEMPLATE(adbSwipe,

        __LUA_ALL_UNUSED(ret)
        __LUA_LINT_UNUSED(le)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 5)            ||
                (!lua_isnumber(L, -5)) ||
                (!lua_isnumber(L, -4)) ||
                (!lua_isnumber(L, -3)) ||
                (!lua_isnumber(L, -2)) ||
                (!lua_isnumber(L, -1)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            __LUA_IF_LINT(

                ImageLite::IRECT<int32_t> r{};
                r.set<int32_t>(
                    lua_tointeger(L, -5),
                    lua_tointeger(L, -4),
                    lua_tointeger(L, -3),
                    lua_tointeger(L, -2)
                );
                if ((r.x < 0) || (r.y < 0) || (r.h < 0) || (r.w < 0))
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);

                std::stringstream ss;
                ss << "duration -> " << ::lua_tointeger(L, -1);
                ss << ", position: ";
                LuaLint::stream_print_irectT<int32_t>(ss, r);
                LuaLint::print_traceT(LuaLint::g_lint_str_31, ss.str());
            )
            __LUA_IF_NOT_LINT(

                if (!le->m_adbcmd.load())
                    break;

                GameDev::ADBDriver::Swipe_s s = {
                    static_cast<int32_t>(lua_tointeger(m_lua, -5)),
                    static_cast<int32_t>(lua_tointeger(m_lua, -4)),
                    static_cast<int32_t>(lua_tointeger(m_lua, -3)),
                    static_cast<int32_t>(lua_tointeger(m_lua, -2)),
                    static_cast<int32_t>(lua_tointeger(m_lua, -1))
                };
                if ((s.x1 < 0) || (s.y1 < 0) || (s.x2 < 0) || (s.y2 < 0))
                    break;
                le->m_adbcmd.load()->swipe(&s);
            )
        }
        LINT_CATCH__
    )
