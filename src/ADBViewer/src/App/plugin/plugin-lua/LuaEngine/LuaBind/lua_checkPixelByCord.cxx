
    __LUA_FUNC_EXT_TEMPLATE(checkPixelByCord,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            bool isargs = false;

            switch (args)
            {
                case 3:
                    {
                        if ((lua_istable(L, -1))  && // { R, G, B }
                            (lua_isnumber(L, -2)) && // position y
                            (lua_isnumber(L, -3)))   // position x
                            isargs = true;
                        break;
                    }
                case 5:
                    {
                        if ((lua_isnumber(L, -1)) && // B
                            (lua_isnumber(L, -2)) && // G
                            (lua_isnumber(L, -3)) && // R
                            (lua_isnumber(L, -4)) && // position y
                            (lua_isnumber(L, -5)))   // position x
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
            ImageLite::IPOINT<int32_t> p{};
            p.set<int32_t>(
                    lua_tointeger(L, -args),
                    lua_tointeger(L, -(args - 1))
                );
            if ((p.x < 0) || (p.y < 0))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
                )
                break;
            }

            switch (args)
            {
                case 3:
                    {
                        std::vector<uint8_t> vpix = LuaObject::getT<uint8_t>(L);
                        if (vpix.size() < 3)
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
                case 5:
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
            ImageLite::IPIX3 ipix = le->m_imgdefault.getpixel(p);
            //
            __LUA_IF_LINT(
                LuaLint::print_ipointT<int32_t>(p);
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
