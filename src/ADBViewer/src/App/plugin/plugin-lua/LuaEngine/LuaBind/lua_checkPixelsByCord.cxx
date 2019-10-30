
    __LUA_FUNC_STATIC_TEMPLATE(checkPixelsByCord,

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if (!lua_istable(L, -1))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            if (!f_image_fb_check(le, ""))
                break;

            bool                       b = true;
            std::vector<int32_t>       vtbl;
            //
            if (!LuaObject::getpixelsarray<5>(L, vtbl))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_26, vtbl.size());
                )
                break;
            }
            for (uint32_t i = 0; i < vtbl.size(); i += 5)
            {
                ImageLite::IPOINT<int32_t> p;
                p.set<int32_t>(vtbl[(i + 0)], vtbl[(i + 1)]);
                if ((p.x < 0) || (p.y < 0))
                {
                    __LUA_IF_LINT(
                        LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_42);
                    )
                    b = false;
                    break;
                }
                ImageLite::IPIX3 upix;
                ImageLite::IPIX3 ipix = le->m_imgdefault.getpixel(p);
                upix.c1 = static_cast<uint8_t>(vtbl[(i + 2)]);
                upix.c2 = static_cast<uint8_t>(vtbl[(i + 3)]);
                upix.c3 = static_cast<uint8_t>(vtbl[(i + 4)]);
                //
                __LUA_IF_LINT(
                    LuaLint::print_ipointT<int32_t>(p);
                    LuaLint::print_ipix3(upix, ipix);
                )
                //
                if ((upix.c1 != ipix.c1) ||
                    (upix.c2 != ipix.c2) ||
                    (upix.c3 != ipix.c3))
                {
                    b = false;
                    break;
                }
            }
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
