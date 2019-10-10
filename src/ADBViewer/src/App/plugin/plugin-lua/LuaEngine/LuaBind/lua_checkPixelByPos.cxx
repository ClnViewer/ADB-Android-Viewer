
    __LUA_FUNC_EXT_TEMPLATE(checkPixelByPos,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            bool isargs = false;

            switch (args)
            {
                case 2:
                    {
                        if ((lua_istable(L, -1))  && // { R, G, B }
                            (lua_isnumber(L, -2)))   // position
                            isargs = true;
                        break;
                    }
                case 4:
                    {
                        if ((lua_isnumber(L, -1)) && // B
                            (lua_isnumber(L, -2)) && // G
                            (lua_isnumber(L, -3)) && // R
                            (lua_isnumber(L, -4)))   // position
                            isargs = true;
                        break;
                    }
                default:
                    break;

            }
            if (!isargs)
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            if (!f_image_fb_check(le, ""))
                break;

            ImageLite::IPIX3 upix{};

            switch (args)
            {
                case 2:
                    {
                        std::vector<uint8_t> vpix = LuaObject::getT<uint8_t>(L);
                        if ((!vpix.size()) || (vpix.size() < 3))
                        {
                            __LUA_IF_LINT(
                                LuaLint::print_traceT(LuaLint::g_lint_str_26, vpix.size());
                            )
                            break;
                        }
                        upix.c3 = vpix[2];
                        upix.c2 = vpix[1];
                        upix.c1 = vpix[0];
                        break;
                    }
                case 4:
                    {
                        upix.c3 = static_cast<uint8_t>(lua_tonumber(L, -1));
                        upix.c2 = static_cast<uint8_t>(lua_tonumber(L, -2));
                        upix.c1 = static_cast<uint8_t>(lua_tonumber(L, -3));
                        break;
                    }
                default:
                    break;
            }

            bool             b;
            int32_t          pos;
            ImageLite::IPIX3 ipix{};

            if ((pos = static_cast<int32_t>(lua_tonumber(L, -args))) <= 0)
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
                    LuaLint::print_traceT(LuaLint::g_lint_str_36, pos);
                )
                break;
            }
            ipix = le->m_imgdefault.getpixel(pos);
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_36, pos);
                LuaLint::print_ipix3(upix, ipix);
            )
            //
            if ((upix.c1 == ipix.c1) &&
                (upix.c2 == ipix.c2) &&
                (upix.c3 == ipix.c3))
                b = true;
            else
                b = false;
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_29, b);
            )
            //
            ret++;
            lua_pushboolean(L, b);
            return;
        }
        LINT_CATCH__
    )
