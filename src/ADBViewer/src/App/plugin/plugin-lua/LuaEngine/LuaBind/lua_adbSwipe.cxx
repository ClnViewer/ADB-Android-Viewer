
    __LUA_FUNC_STATIC_TEMPLATE(adbSwipe,

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

            ImageLite::IRECT<int32_t> r{};
            r.set<int32_t>(
                    lua_tointeger(L, -5),
                    lua_tointeger(L, -4),
                    lua_tointeger(L, -3),
                    lua_tointeger(L, -2)
            );
            int32_t duration = ::lua_tointeger(L, -1);
                    duration = ((duration < 0) ? 0 : duration);

            __LUA_IF_LINT(

                if ((r.x < 0) || (r.y < 0) || (r.w < 0) || (r.h < 0))
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);

                std::stringstream ss;
                ss << "duration -> " << duration;
                ss << ", position: ";
                LuaLint::stream_print_irectT<int32_t>(ss, r);
                LuaLint::print_traceT(LuaLint::g_lint_str_31, ss.str());
            )
            __LUA_IF_PLUGIN(

                if (!le->m_adbcmd.load())
                    break;
                if ((r.x < 0) || (r.y < 0) || (r.w < 0) || (r.h < 0))
                    break;

                GameDev::ADBDriver::Swipe_s sw;
                sw.x0 = r.x;
                sw.y0 = r.y;
                sw.x1 = r.w;
                sw.y1 = r.h;
                sw.t  = duration;

                le->m_adbcmd.load()->swipe(&sw);
            )
        }
        LINT_CATCH__
    )
