
    __LUA_FUNC_EXT_TEMPLATE(stateSleep,

        __LUA_LINT_UNUSED(ret);
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
        )
        //
        int32_t sl;

        if ((args != 1)            ||
            (!lua_isnumber(L, -1)) ||
            ((sl = lua_tonumber(L, -1)) <= 0))
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
            )
            break;
        }

        lua_pushnil(L);
        //
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_23, sl);
        )
        // thread sleep user defined second
        LINT_TRY__
        {
            auto s_start = std::chrono::high_resolution_clock::now();
            auto s_end = (s_start + std::chrono::seconds(sl));

            while (std::chrono::high_resolution_clock::now() < s_end)
            {
                if (le->getdbgbreak())
                    return;
                std::this_thread::yield();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return;
        }
        LINT_CATCH__
        return;
    )

