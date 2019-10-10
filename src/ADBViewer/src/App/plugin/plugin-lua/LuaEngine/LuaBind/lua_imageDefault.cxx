
#if defined(_BUILD_LUA_EDITOR)

    __LUA_FUNC_EXT_TEMPLATE(imageDefault,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if (args != 1)
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }
            if (!lua_isstring(L, -1))
            {
                ret++;
                lua_pushstring(L, le->m_imgdefault_name.c_str());
                return;
            }

            std::string s = lua_tostring(L, -1);
            if (s.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_41);
                )
                break;
            }
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_36, s);
            )
            //
            (void) f_image_fb_check(le, s);

            ret++;
            lua_pushstring(L, le->m_imgdefault_name.c_str());
            return;
        }
        LINT_CATCH__
    )
#else
    __LUA_FUNC_EXT_TEMPLATE(imageDefault,

        __LUA_LINT_UNUSED(ret);
        __LUA_LINT_UNUSED(args);
        break;
    )
#endif

