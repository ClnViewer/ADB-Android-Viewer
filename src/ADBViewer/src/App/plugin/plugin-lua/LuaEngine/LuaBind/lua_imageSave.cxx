
    __LUA_FUNC_STATIC_TEMPLATE(imageSave,

        __LUA_ALL_UNUSED(ret)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 2)            ||
                (!lua_isnumber(L, -1)) ||
                (!lua_isstring(L, -2)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }
            //
            int32_t     cnt = lua_tointeger(L, -1);
            std::string s   = lua_tostring(L, -2);
            if ((s.empty()) || (cnt < 0))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_41);
                )
                break;
            }
            if (le->m_savecount.load() >= cnt)
            {
                __LUA_IF_LINT(

                    std::stringstream ss;
                    ss << LuaLint::g_lint_str_49;
                    ss << le->m_savecount.load() << "/" << cnt;
                    LuaLint::print_traceT(LuaLint::g_lint_str_33, ss.str());
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
            std::string spath = s;
            //
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_31, s);
            )
            //
            le->m_savecount++;
            //
            std::size_t pos;
            uint32_t    uid = static_cast<uint32_t>(std::time(nullptr));
            if ((pos = s.find_first_of('.')) != std::string::npos)
                s = s.substr(0, pos);

            std::stringstream sspath;
            sspath << s.c_str() << "_" << le->m_savecount.load() << "_" << uid << ".png";
            le->m_imgdefault.save(sspath.str(), ImageLite::ImageType::IT_PNG);
            //
            __LUA_IF_LINT(
                {
                    std::stringstream ss;
                    ss << functionname << "( \"" << spath.c_str() << "\", " << cnt << " )";
                    ss << " save -> " << sspath.str().c_str();
                    LuaLint::print_traceT(LuaLint::g_lint_str_29, ss.str());
                }
            )
        }
        LINT_CATCH__
    )

