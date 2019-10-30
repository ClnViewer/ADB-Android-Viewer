
#pragma once

#define __LUA_LINT_TOSTRING(A) # A
#define __LUA_LINT_UNUSED(A) (void) A;
#define __LUA_ALL_UNUSED(A) __LUA_LINT_UNUSED(A)
#define __LUA_IF_LINT(A) A
#define __LUA_IF_NOT_LINT(...)
#define __LUA_BIND_CLASS() Editor

#define LINT_TRY__                                                                                              \
    try
#define LINT_CATCH__                                                                                            \
    catch (std::exception const & ex_)                                                                          \
    {                                                                                                           \
        LuaLint::print_traceT<std::string>(LuaLint::g_lint_str_27, ex_.what());                                 \
    }                                                                                                           \
    catch (...)                                                                                                 \
    {                                                                                                           \
        LuaLint::print_traceT<std::string>(LuaLint::g_lint_str_28, __FUNCTION__);                               \
    }

///

#define __LUA_FUNC_CLASS_TEMPLATE(funname, args_in, args_out, helpstr, ext_fun)                                 \
    int32_t LuaEngine::funname ()                                                                               \
    {                                                                                                           \
        ::lua_pop(m_lua, 1);                                                                                    \
        LuaObject::stackdump_(m_lua,                                                                            \
                [=](std::string const & s, int32_t i)                                                           \
                    {                                                                                           \
                        std::stringstream ss;                                                                   \
                        ss << "\t" << s.c_str() << ", index: " << i;                                            \
                        LuaLint::print_traceT(" -- stack:", ss.str());                                          \
                    }                                                                                           \
            );                                                                                                  \
        std::stringstream ss;                                                                                   \
        bool isnotvalid = false;                                                                                \
        int32_t args = 0,                                                                                       \
                ret  = 0,                                                                                       \
                n    = (lua_gettop(m_lua) - 1);                                                                 \
        LuaLint::stream_print_caller(ss, __PRETTY_FUNCTION__, __FILE__, __LINE__);                              \
        LuaLint::stream_print_called(ss, #funname, n);                                                          \
        {                                                                                                       \
            int32_t nargs = __LUA_FUNC_ARGS_ ## args_in ();                                                     \
            if (nargs != n)                                                                                     \
            {                                                                                                   \
                LuaLint::stream_print_arg_warn(ss, #funname, n, nargs);                                         \
                isnotvalid = true;                                                                              \
            }                                                                                                   \
            __LUA_FUNC_LINT_ ## args_in (funname)                                                               \
            __LUA_FUNC_LINT_ ## ext_fun (funname, args_out)                                                     \
        }                                                                                                       \
        LuaLint::stream_print_return(ss, #funname, ret);                                                        \
        LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceC);                                             \
        if (isnotvalid)                                                                                         \
            LuaLint::print("\nSee help: " helpstr "\n", LuaLint::ColorPrint::DebugHelp);                        \
        return ret;                                                                                             \
    }

/// EXT C function

#define __LUA_FUNC_STATIC_TEMPLATE(funname, body)                                                               \
    static void f_ext_ ## funname (                                                                             \
                    LuaEngine *le,                                                                              \
                    lua_State *L,                                                                               \
                    int32_t args,                                                                               \
                    int32_t & ret,                                                                              \
                    bool isnotvalid                                                                             \
    ) {                                                                                                         \
        static const char functionname[] = __LUA_LINT_TOSTRING(funname);                                        \
        do                                                                                                      \
        {                                                                                                       \
            if (isnotvalid)                                                                                     \
                break;                                                                                          \
                                                                                                                \
            { body }                                                                                            \
        }                                                                                                       \
        while (0);                                                                                              \
        lua_pushnil(L);                                                                                         \
    }


/// INPUT_

#define __LUA_FUNC_LINT_INPUT_NONE(funname)                                                                     \
        (void) args;

#define __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                   \
    ++args;                                                                                                     \
    if (!lua_isstring(m_lua, -n))                                                                               \
        LuaLint::stream_print_error(ss, "string", args);                                                        \
    else                                                                                                        \
        LuaLint::stream_print_argT<const char*>(ss, lua_tostring(m_lua, -n), args);                             \
    n--;

#define __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
    ++args;                                                                                                     \
    if (!lua_isnumber(m_lua, -n))                                                                               \
        LuaLint::stream_print_error(ss, "number", args);                                                        \
    else                                                                                                        \
        LuaLint::stream_print_argT<int32_t>(ss, lua_tonumber(m_lua, -n), args);                                 \
    n--;

#define __LUA_FUNC_LINT_INPUT_BOOL(funname)                                                                     \
    ++args;                                                                                                     \
    if (!lua_isboolean(m_lua, -n))                                                                              \
        LuaLint::stream_print_error(ss, "boolean", args);                                                       \
    else                                                                                                        \
        LuaLint::stream_print_argT<bool>(ss, lua_toboolean(m_lua, -n), args);                                   \
    n--;

#define __LUA_FUNC_LINT_INPUT_TABLE(funname)                                                                    \
    ++args;                                                                                                     \
    if (!lua_istable(m_lua, -n))                                                                                \
        LuaLint::stream_print_error(ss, "{table}", args);                                                       \
    else                                                                                                        \
        LuaLint::stream_print_argT<const char*>(ss, "{table}", args);                                           \
    n--;

/// RETURN_

#define __LUA_FUNC_LINT_RETURN_NONE()                                                                           \
    lua_pushnil(m_lua);

#define __LUA_FUNC_LINT_RETURN_NUMBER()                                                                         \
    lua_pushinteger(m_lua, 100); ret++;

#define __LUA_FUNC_LINT_RETURN_BOOL()                                                                           \
    lua_pushboolean(m_lua, 1); ret++;

#define __LUA_FUNC_LINT_RETURN_DOUBLE()                                                                         \
    lua_pusnumber(m_lua, 100.0); ret++;

#define __LUA_FUNC_LINT_RETURN_STRING()                                                                         \
    lua_pushstring(m_lua, "test string return");

#define __LUA_FUNC_LINT_RETURN_NULL()                                                                           \
    lua_pushnil(m_lua);

/// FROM BASE EXTENSION
/// RETURN_
/// -- if/else extended function call

#define __LUA_FUNC_LINT_LFEXT_BYNAME(funname, args_out)                                                         \
    f_ext_ ## funname (this, m_lua, args, ret, isnotvalid);

#define __LUA_FUNC_LINT_LFEXT_NONE(funname, args_out)                                                           \
    __LUA_FUNC_LINT_ ## args_out ()

/// RETURN_ real extended definition

#define __LUA_FUNC_LINT_RETURN_NUMBER_1()                                                                       \
    __LUA_FUNC_LINT_RETURN_NUMBER()

#define __LUA_FUNC_LINT_RETURN_NUMBER_2()                                                                       \
    __LUA_FUNC_LINT_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_LINT_RETURN_NUMBER()

#define __LUA_FUNC_LINT_RETURN_NUMBER_3()                                                                       \
    __LUA_FUNC_LINT_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_LINT_RETURN_NUMBER()                                                                         \
            __LUA_FUNC_LINT_RETURN_NUMBER()

#define __LUA_FUNC_LINT_RETURN_NUMBER_4()                                                                       \
    __LUA_FUNC_LINT_RETURN_NUMBER()                                                                             \
        __LUA_FUNC_LINT_RETURN_NUMBER()                                                                         \
            __LUA_FUNC_LINT_RETURN_NUMBER()                                                                     \
                __LUA_FUNC_LINT_RETURN_NUMBER()

#define __LUA_FUNC_LINT_RETURN_TABLE()                                                                         \
    __LUA_FUNC_LINT_RETURN_NULL() // TODO: return table

/// FROM BASE EXTENSION
/// INPUT_

#define __LUA_FUNC_LINT_INPUT_STRING_1(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_STRING(funname)

#define __LUA_FUNC_LINT_INPUT_STRING_2(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_STRING(funname)

#define __LUA_FUNC_LINT_INPUT_STRING_3(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                   \
            __LUA_FUNC_LINT_INPUT_STRING(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_1(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_2(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_3(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_4(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                               \
                __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_5(funname)                                                                 \
    __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                               \
                __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                           \
                    __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_STRING_1_NUMBER_1(funname)                                                        \
    __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_STRING_1_NUMBER_4(funname)                                                        \
    __LUA_FUNC_LINT_INPUT_STRING(funname)                                                                       \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
            __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                               \
                __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                           \
                    __LUA_FUNC_LINT_INPUT_NUMBER(funname)

#define __LUA_FUNC_LINT_INPUT_NUMBER_4_OR_NUMBER_1_TABLE(funname)                                               \
    if (lua_istable(m_lua, -1))                                                                                 \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_NUMBER(funname)                                                                   \
        __LUA_FUNC_LINT_INPUT_TABLE(funname)                                                                    \
    }                                                                                                           \
    else if (lua_isnumber(m_lua, -1))                                                                           \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_NUMBER_4(funname)                                                                 \
    }

#define __LUA_FUNC_LINT_INPUT_NUMBER_4_OR_TABLE(funname)                                                        \
    if (lua_istable(m_lua, -1))                                                                                 \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_TABLE(funname)                                                                    \
    }                                                                                                           \
    else if (lua_isnumber(m_lua, -1))                                                                           \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_NUMBER_4(funname)                                                                 \
    }


#define __LUA_FUNC_LINT_INPUT_NUMBER_5_OR_NUMBER_2_TABLE(funname)                                               \
    if (lua_istable(m_lua, -1))                                                                                 \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_NUMBER_2(funname)                                                                 \
        __LUA_FUNC_LINT_INPUT_TABLE(funname)                                                                    \
    }                                                                                                           \
    else if (lua_isnumber(m_lua, -1))                                                                           \
    {                                                                                                           \
        __LUA_FUNC_LINT_INPUT_NUMBER_5(funname)                                                                 \
    }

