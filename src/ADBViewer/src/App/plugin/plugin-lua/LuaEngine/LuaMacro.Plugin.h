
#pragma once

#define __LUA_LINT_TOSTRING(A) # A
#define __LUA_LINT_UNUSED(...)
#define __LUA_ALL_UNUSED(A) (void) A;
#define __LUA_IF_LINT(A) if (LuaLint::istrace()) { A }
#define __LUA_IF_PLUGIN(A) A
#define __LUA_BIND_CLASS() Plugins

#define LINT_TRY__                                                                                              \
    try
#define LINT_CATCH__                                                                                            \
    catch (std::exception const & ex_) {}                                                                       \
    catch (...) {}

///

#define __LUA_FUNC_CLASS_TEMPLATE(funname, args_in, args_out, helpstr, ext_fun)                                 \
    int32_t LuaEngine::funname ()                                                                               \
    {                                                                                                           \
        ::lua_pop(m_lua, 1);                                                                                    \
        int32_t args  = (::lua_gettop(m_lua) - 1),                                                              \
                nargs = __LUA_FUNC_ARGS_ ## args_in (),                                                         \
                ret   = 0;                                                                                      \
        __LUA_FUNC_CALL_DECLARE(funname)                                                                        \
        return ret;                                                                                             \
    }

/// C function

#define __LUA_FUNC_CALL_DECLARE(funname)                                                                        \
    f_ext_ ## funname (this, m_lua, args, nargs, ret);

#define __LUA_FUNC_STATIC_TEMPLATE(funname, body)                                                               \
    static void f_ext_ ## funname (                                                                             \
                    LuaEngine *le,                                                                              \
                    lua_State *L,                                                                               \
                    int32_t args,                                                                               \
                    int32_t nargs,                                                                              \
                    int32_t & ret                                                                               \
    ) {                                                                                                         \
        static const char functionname[] = __LUA_LINT_TOSTRING(funname);                                        \
        do                                                                                                      \
        {                                                                                                       \
            if (nargs != args)                                                                                  \
                break;                                                                                          \
                                                                                                                \
            { body }                                                                                            \
        }                                                                                                       \
        while (0);                                                                                              \
        ret = 0;                                                                                                \
        ::lua_pushnil(L);                                                                                       \
    }
