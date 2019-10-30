
    __LUA_FUNC_STATIC_TEMPLATE(imageCompareRegion,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 5)            ||
                (!lua_isnumber(L, -1)) ||
                (!lua_isnumber(L, -2)) ||
                (!lua_isnumber(L, -3)) ||
                (!lua_isnumber(L, -4)) ||
                (!lua_isstring(L, -5)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            std::string s;
            ImageLite::IRECT<int32_t> r{};

            r.h = lua_tonumber(L, -1);
            r.w = lua_tonumber(L, -2);
            r.y = lua_tonumber(L, -3);
            r.x = lua_tonumber(L, -4);
            //
            if ((r.w <= 0) ||
                (r.h <= 0) ||
                (r.x <  0) ||
                (r.y <  0))
            {
                __LUA_IF_LINT(

                    std::stringstream ss;
                    LuaLint::stream_print_irectT<int32_t>(ss, r);
                    ss << " -> " << LuaLint::g_lint_str_50;
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, ss.str());
                )
                break;
            }
            s = lua_tostring(L, -5);
            if (s.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_41);
                    LuaLint::print_traceT(LuaLint::g_lint_str_20, LuaLint::g_lint_str_45);
                )
                break;
            }
            if (!f_image_fb_check(le, ""))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_39, le->m_imgdefault_name);
                )
                break;
            }
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_36, s);
                LuaLint::print_irectT<int32_t>(r);
            )
            //
            double d = 0.0;
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
                    ss << functionname << "( \"" << s.c_str() << "\", ";
                    LuaLint::stream_print_irectT<int32_t>(ss, r);
                    ss << " ) return -> " << d;
                    f_image_show(2, le, ss.str(), le->m_imgdefault, img);
                }
            )
            return;
        }
        LINT_CATCH__
    )

