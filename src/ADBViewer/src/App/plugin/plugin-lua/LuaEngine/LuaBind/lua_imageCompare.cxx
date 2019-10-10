
    __LUA_FUNC_EXT_TEMPLATE(imageCompare,

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
            if (!f_image_fb_check(le, ""))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_39, le->m_imgdefault_name);
                    LuaLint::print_traceT(LuaLint::g_lint_str_20, LuaLint::g_lint_str_45);
                )
                break;
            }
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_36, s);
            )
            //
            double d = 0.0;
            ImageLite::IRECT<int32_t> r{};
            ImageLite::ImageRGBbuffer img;
            if ((!f_image_comparable(le, s, r, img, d)) ||
                (img.empty()))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_34, 0);
                )
                break;
            }
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_29, d);
            )
            //
            ret++;
            lua_pushnumber(L, d);
            //
            __LUA_IF_LINT(
                {
                    std::stringstream ss;
                    ss << functionname << "( \"" << s.c_str() << "\" ) return -> " << d;
                    f_image_show(2, le, ss.str(), le->m_imgdefault, img);
                }
            )
            return;
        }
        LINT_CATCH__
    )

