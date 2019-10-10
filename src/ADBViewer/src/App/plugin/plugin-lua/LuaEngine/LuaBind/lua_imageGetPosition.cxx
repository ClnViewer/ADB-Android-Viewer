
    __LUA_FUNC_EXT_TEMPLATE(imageGetPosition,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 2)            ||
                (!lua_isnumber(L, -1)) ||
                (!lua_isnumber(L, -2)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }
            //
            if (!f_image_fb_check(le, ""))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_39, le->m_imgdefault_name);
                    LuaLint::print_traceT(LuaLint::g_lint_str_20, LuaLint::g_lint_str_45);
                )
                break;
            }

            ImageLite::IPOINT<int32_t> p{};
            p.set<int32_t>(
                    lua_tonumber(L, -2),
                    lua_tonumber(L, -1)
                );
            if ((p.x < 0) || (p.y < 0))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
                )
                break;
            }

            int32_t pos = le->m_imgdefault.getpos(p);
            __LUA_IF_LINT(
                LuaLint::print_ipointT<int32_t>(p);
                LuaLint::print_traceT(LuaLint::g_lint_str_29, pos);
            )

            ret++;
            ::lua_pushinteger(L, pos);
            return;
        }
        LINT_CATCH__
    )
