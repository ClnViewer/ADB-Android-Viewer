
    __LUA_FUNC_STATIC_TEMPLATE(checkTime,

        __LUA_LINT_UNUSED(le)
        //
        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if ((args != 2)            ||
                (!lua_isstring(L, -1)) ||
                (!lua_isstring(L, -2)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }

            int32_t tdiff = -1;
            std::time_t t = std::time(nullptr);
            std::tm *tml = std::localtime(&t);
            std::tm  tms{};
            std::tm  tme{};

            if ((!tml)                                        ||
                (!f_string_to_time(lua_tostring(L, -2), tme)) ||
                (!f_string_to_time(lua_tostring(L, -1), tms)))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_40, tdiff);
                )
                ret++;
                ::lua_pushinteger(L, tdiff);
                return;
            }
            do
            {
                tdiff = 0;

                if ((*tml).tm_hour < tms.tm_hour)
                    break;
                if ((tms.tm_min) && ((*tml).tm_min < tms.tm_min))
                    break;
                if ((tms.tm_sec) && ((*tml).tm_sec < tms.tm_sec))
                    break;
                //
                if ((*tml).tm_hour >= tme.tm_hour)
                    break;
                if ((tme.tm_min) && ((*tml).tm_min >= tme.tm_min))
                    break;
                if ((tme.tm_sec) && ((*tml).tm_sec >= tme.tm_sec))
                    break;

                tdiff = 1;
            }
            while (0);

            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_29, tdiff);
            )

            ret++;
            ::lua_pushinteger(L, tdiff);
            return;
        }
        LINT_CATCH__
    )
