
    __LUA_FUNC_STATIC_TEMPLATE(screenGet,

        __LUA_ALL_UNUSED(args)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if (!f_image_fb_check(le, ""))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_39, le->m_imgdefault_name);
                    LuaLint::print_traceT(LuaLint::g_lint_str_20, LuaLint::g_lint_str_45);
                )
                break;
            }
            //
            if (!LuaObject::setformatLuaImage(L, le->m_imgdefault))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_33, LuaLint::g_lint_str_46);
                )
                break;
            }

            ret++;

            __LUA_IF_LINT(
                {
                    std::stringstream ss;
                    ss << functionname << "() return ->  {table} -> ";
                    LuaLint::stream_print_ipointT<int32_t>(ss, le->m_imgdefault.getbuffer().point);
                    LuaLint::print_traceT(LuaLint::g_lint_str_29, ss.str());
                }
            )
            return;
        }
        LINT_CATCH__
    )
