
#pragma once

#define __LUA_FUNC_TEMPLATE(ERRSTR_)                                                                            \
    static int32_t f_safeBindRuner_(lua_State *L, lua_CFunction fun)                                            \
    {                                                                                                           \
        int32_t ret = 0;                                                                                        \
        try { ret = fun(L); }                                                                                   \
        catch(std::exception const & ex_) { luaL_error(L, ex_.what()); }                                        \
        catch(...)                        { luaL_error(L, ERRSTR_); }                                           \
        return ret;                                                                                             \
    }                                                                                                           \
    template <lua_CFunction fun>                                                                                \
    int32_t f_callSafeBind_(lua_State *L) { return f_safeBindRuner_(L, fun); }

#define __LUA_FUNC_CLASS(funname)                                                                               \
    int32_t funname ();

#define __LUA_FUNC_TABLE(funname)                                                                               \
    { #funname, f_callSafeBind_<&f_bind_ ## funname> }

#define __LUA_FUNC_CODE_WRAP(funname)                                                                           \
    extern "C" int32_t f_bind_ ## funname (lua_State *L)                                                        \
    {                                                                                                           \
        LuaEngine *le = LuaEngine::instance(L);                                                                 \
        if (!le) { lua_pushnil(L); return 1; }                                                                  \
        return le->funname();                                                                                   \
    }

#define __LUA_FUNC_CODE_HEADER(funname)                                                                         \
    int32_t LuaEngine::funname ()

#define __LUA_FUNC_CODE_BODY(funname, args_in, args_out, helpstr)                                               \
    int32_t LuaEngine::funname ()                                                                               \
    {                                                                                                           \
        lua_pop(m_lua, 1);                                                                                      \
        std::stringstream ss;                                                                                   \
        bool ishelp  = false;                                                                                   \
        int32_t args = 0,                                                                                       \
                ret  = 0,                                                                                       \
                n    = (lua_gettop(m_lua) - 1);                                                                 \
        f_fun_print_caller(ss, __PRETTY_FUNCTION__, __FILE__, __LINE__);                                        \
        f_fun_print_called(ss, #funname, n);                                                                    \
        {                                                                                                       \
            int32_t nargs = __LUA_FUNC_ARGS_ ## args_in ();                                                     \
            if (nargs != n)                                                                                     \
            {                                                                                                   \
                f_fun_print_argwarn(ss, #funname, n, nargs);                                                    \
                ishelp = true;                                                                                  \
            }                                                                                                   \
            __LUA_FUNC_BODY_ ## args_in (funname)                                                               \
            __LUA_FUNC_BODY_ ## args_out ()                                                                     \
        }                                                                                                       \
        f_fun_print_return(ss, #funname, ret);                                                                  \
        DebugPrintTraceC(ss.str());                                                                                      \
        if (ishelp)                                                                                             \
            DebugPrintHelp("\nSee help: " helpstr "\n");                                                                    \
        return ret;                                                                                             \
    }

/// INPUT_

#define __LUA_FUNC_BODY_INPUT_NONE(funname)                                                                     \
        (void) args;

#define __LUA_FUNC_BODY_INPUT_STRING(funname)                                                                   \
    ++args;                                                                                                     \
    if (!lua_isstring(m_lua, -n))                                                                               \
        f_fun_print_error(ss, "string", args);                                                                  \
    else                                                                                                        \
        f_fun_print_ok<const char*>(ss, lua_tostring(m_lua, -n), args);                                         \
    n--;

#define __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                   \
    ++args;                                                                                                     \
    if (!lua_isnumber(m_lua, -n))                                                                               \
        f_fun_print_error(ss, "number", args);                                                                  \
    else                                                                                                        \
        f_fun_print_ok<int32_t>(ss, lua_tonumber(m_lua, -n), args);                                             \
    n--;

#define __LUA_FUNC_BODY_INPUT_BOOL(funname)                                                                     \
    ++args;                                                                                                     \
    if (!lua_isboolean(m_lua, -n))                                                                              \
        f_fun_print_error(ss, "boolean", args);                                                                 \
    else                                                                                                        \
        f_fun_print_ok<bool>(ss, lua_toboolean(m_lua, -n), args);                                               \
    n--;

/// RETURN_

#define __LUA_FUNC_BODY_RETURN_NONE()                                                                           \
    lua_pushnil(m_lua);

#define __LUA_FUNC_BODY_RETURN_NUMBER()                                                                         \
    lua_pushinteger(m_lua, 100); ret++;

#define __LUA_FUNC_BODY_RETURN_BOOL()                                                                           \
    lua_pushboolean(m_lua, 1); ret++;

#define __LUA_FUNC_BODY_RETURN_STRING()                                                                         \
    lua_pushstring(m_lua, "test string return");

/// FROM BASE EXTENSION
/// RETURN_

#define __LUA_FUNC_BODY_RETURN_NUMBER_1()                                                                       \
    __LUA_FUNC_BODY_RETURN_NUMBER()

#define __LUA_FUNC_BODY_RETURN_NUMBER_2()                                                                       \
    __LUA_FUNC_BODY_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_BODY_RETURN_NUMBER()

#define __LUA_FUNC_BODY_RETURN_NUMBER_3()                                                                       \
    __LUA_FUNC_BODY_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_BODY_RETURN_NUMBER()                                                                         \
            __LUA_FUNC_BODY_RETURN_NUMBER()

#define __LUA_FUNC_BODY_RETURN_NUMBER_4()                                                                       \
    __LUA_FUNC_BODY_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_BODY_RETURN_NUMBER()                                                                         \
            __LUA_FUNC_BODY_RETURN_NUMBER()                                                                     \
                __LUA_FUNC_BODY_RETURN_NUMBER()

#define __LUA_FUNC_BODY_RETURN_OBJECT()                                                                         \
    __LUA_FUNC_BODY_RETURN_NONE() // TODO: return table

#define __LUA_FUNC_BODY_RETURN_NUMBER_2_OBJECT()                                                                \
    __LUA_FUNC_BODY_RETURN_NUMBER_2() // TODO: return table

/// FROM BASE EXTENSION
/// INPUT_

#define __LUA_FUNC_BODY_INPUT_STRING_1(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_STRING(funname)

#define __LUA_FUNC_BODY_INPUT_STRING_2(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_STRING(funname)

#define __LUA_FUNC_BODY_INPUT_STRING_3(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_STRING(funname)                                                                   \
            __LUA_FUNC_BODY_INPUT_STRING(funname)

#define __LUA_FUNC_BODY_INPUT_NUMBER_1(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_NUMBER(funname)

#define __LUA_FUNC_BODY_INPUT_NUMBER_2(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_NUMBER(funname)

#define __LUA_FUNC_BODY_INPUT_NUMBER_3(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_BODY_INPUT_NUMBER(funname)

#define __LUA_FUNC_BODY_INPUT_NUMBER_4(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                               \
                __LUA_FUNC_BODY_INPUT_NUMBER(funname)

#define __LUA_FUNC_BODY_INPUT_NUMBER_5(funname)                                                                 \
    __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                               \
                __LUA_FUNC_BODY_INPUT_NUMBER(funname)                                                           \
                    __LUA_FUNC_BODY_INPUT_NUMBER(funname)


/// INPUT ARGUMENTS NUMBER

#define __LUA_FUNC_ARGS_INPUT_NONE()     0
#define __LUA_FUNC_ARGS_INPUT_STRING()   1
#define __LUA_FUNC_ARGS_INPUT_STRING_1() 1
#define __LUA_FUNC_ARGS_INPUT_STRING_2() 2
#define __LUA_FUNC_ARGS_INPUT_STRING_3() 3
#define __LUA_FUNC_ARGS_INPUT_STRING_4() 4
#define __LUA_FUNC_ARGS_INPUT_STRING_5() 5
#define __LUA_FUNC_ARGS_INPUT_NUMBER()   1
#define __LUA_FUNC_ARGS_INPUT_NUMBER_1() 1
#define __LUA_FUNC_ARGS_INPUT_NUMBER_2() 2
#define __LUA_FUNC_ARGS_INPUT_NUMBER_3() 3
#define __LUA_FUNC_ARGS_INPUT_NUMBER_4() 4
#define __LUA_FUNC_ARGS_INPUT_NUMBER_5() 5

