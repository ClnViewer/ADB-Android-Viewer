
#if defined(_BUILD_LUA_EDITOR)

    __LUA_FUNC_STATIC_TEMPLATE(imageShow,

        __LUA_LINT_UNUSED(ret);

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
            std::string s;
            do
            {
                if (!lua_isstring(L, -1))
                    break;
                s = lua_tostring(L, -1);
            }
            while (0);

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
            ::lua_pushnil(L);

            ImageLite::ImageRGBbuffer img(s);
            if (img.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_34, 0);
                )
                break;
            }

            f_image_show(0, le, s, img, img);
        }
        LINT_CATCH__
        return;
    )
#else
    __LUA_FUNC_STATIC_TEMPLATE(imageShow,

        __LUA_LINT_UNUSED(ret);
        __LUA_LINT_UNUSED(args);
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
        )
        break;
    )
#endif

