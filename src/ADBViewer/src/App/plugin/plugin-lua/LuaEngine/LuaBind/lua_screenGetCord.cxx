
    __LUA_FUNC_EXT_TEMPLATE(screenGetCord,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            ImageLite::IRECT<int32_t> r{};
            //
            switch (args)
            {
                case 1:
                    {
                        if (!lua_istable(L, -1))  // { x, y }
                            break;

                        std::vector<int32_t> vcord = LuaObject::getT<int32_t>(L);
                        if (vcord.size() < 4)
                        {
                            __LUA_IF_LINT(
                                LuaLint::print_traceT(LuaLint::g_lint_str_26, vcord.size());
                            )
                            break;
                        }
                        r.x = vcord[0];
                        r.y = vcord[1];
                        r.w = vcord[2];
                        r.h = vcord[3];
                        break;
                    }
                case 4:
                    {
                        if ((!lua_isnumber(L, -1)) || // position y
                            (!lua_isnumber(L, -2)))   // position x
                            break;

                        r.x = ::lua_tointeger(L, -4);
                        r.y = ::lua_tointeger(L, -3);
                        r.w = ::lua_tointeger(L, -2);
                        r.h = ::lua_tointeger(L, -1);
                        break;
                    }
                default:
                    {
                        __LUA_IF_LINT(
                            LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                        )
                        break;
                    }
            }
            //
            if ((r.x < 0) || (r.y < 0) || (r.w <= 0) || (r.h <= 0))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
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
            //
            ImageLite::ImageRGBbuffer img(le->m_imgdefault.getbuffer(), r);
            if (img.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_34, 0);
                )
                break;
            }
            if (!LuaObject::setformatLuaImage(L, img))
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
                    ss << functionname << "() return ->  {table} -> new image: ";
                    LuaLint::stream_print_ipointT<int32_t>(ss, img.getbuffer().point);
                    ss << " -> from screen: ";
                    LuaLint::stream_print_ipointT<int32_t>(ss, le->m_imgdefault.getbuffer().point);
                    ss << " -> by cord: ";
                    LuaLint::stream_print_irectT<int32_t>(ss, r);
                    LuaLint::print_traceT(LuaLint::g_lint_str_29, ss.str());
                }
            )
            return;
        }
        LINT_CATCH__
    )
