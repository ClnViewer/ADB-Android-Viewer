
    __LUA_FUNC_EXT_TEMPLATE(checkPixelsByPos,

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

            bool                 b = true;
            std::vector<int32_t> vtbl;
            //
            if (!LuaObject::getpixelsarray<4>(L, vtbl))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_26, vtbl.size());
                )
                break;
            }
            for (uint32_t i = 0; i < vtbl.size(); i += 4)
            {
                int32_t pos = vtbl[(i + 0)];
                if (pos < 0)
                {
                    __LUA_IF_LINT(
                        LuaLint::print_traceT(LuaLint::g_lint_str_38, pos);
                    )
                    b = false;
                    break;
                }
                ImageLite::IPIX3 upix;
                ImageLite::IPIX3 ipix = le->m_imgdefault.getpixel(pos);
                upix.c1 = static_cast<uint8_t>(vtbl[(i + 1)]);
                upix.c2 = static_cast<uint8_t>(vtbl[(i + 2)]);
                upix.c3 = static_cast<uint8_t>(vtbl[(i + 3)]);
                //
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_38, pos);
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
