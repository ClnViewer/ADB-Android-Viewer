
    __LUA_FUNC_STATIC_TEMPLATE(traceOn,

        __LUA_ALL_UNUSED(args)
        __LUA_ALL_UNUSED(ret)
        __LUA_ALL_UNUSED(le)

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
                LuaLint::print_traceT(LuaLint::g_lint_str_52, "mute Off");
            )
            LuaLint::settrace(true);
        }
        LINT_CATCH__
    )
