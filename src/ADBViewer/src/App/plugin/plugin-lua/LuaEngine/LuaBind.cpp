
#if defined(_BUILD_LUA_EDITOR)
#  include "../../../../loader.h"
#else
#  include "../../plugin-base.h"
#endif

#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <functional>
#include <ctime>
#include <lua.hpp>

#if defined(_BUILD_LUA_EDITOR)
#  define _BUILD_IMAGELITE_DRAW 1
#endif
#include "../ImageLite/ImageLite.h"
#include "LuaObject.h"
#include "LuaLint.h"

#if defined(_BUILD_LUA_EDITOR)
#  include "LuaEngine.Editor.h"
#else
#  include "LuaEngine.Plugin.h"
#endif

namespace __LUA_BIND_CLASS()
{
    __LUA_FUNC_TEMPLATE(LuaLint::g_lint_str_44)
    //
    __LUA_FUNC_CODE_WRAP(print)
    __LUA_FUNC_CODE_WRAP(dprint)
    //
#   define LFUN__(S,A,...) __LUA_FUNC_CODE_WRAP(A)
#   include "LuaPluginFunction.h"

    static struct luaL_Reg l_func_LuaObject[] =
    {
#       define LFUN__(S,A,...) __LUA_FUNC_TABLE(A),
#       include "LuaPluginFunction.h"
        { nullptr, nullptr }
    };

    static struct luaL_Reg l_func_GlobRedefine[] =
    {
        __LUA_FUNC_TABLE(print),
        __LUA_FUNC_TABLE(dprint),
        { nullptr, nullptr }
    };

    void LuaEngine::initbase_()
    {
        m_fun_object      = l_func_LuaObject;
        m_fun_redefine    = l_func_GlobRedefine;
        m_fun_object_sz   = (__NELE(l_func_LuaObject) - 1);
        m_fun_redefine_sz = (__NELE(l_func_GlobRedefine) - 1);
    }

    __LUA_FUNC_CODE_HEADER(print)
    {
        __LUA_IF_LINT(
            LuaLint::print_lua(m_lua, LuaLint::ColorPrint::LuaFunctionPrint);
        )
        ::lua_pushnil(m_lua);
        return 0;
    }
    __LUA_FUNC_CODE_HEADER(dprint)
    {
        __LUA_IF_LINT(
            LuaLint::print_lua(m_lua, LuaLint::ColorPrint::LuaFunctionDprint);
        )
        ::lua_pushnil(m_lua);
        return 0;
    }

    //
    // helper functions
#   include "LuaBind/lua_function_static.cxx"

    //
    // Lua declare extended functions
    // enable flag LFEXT_BYNAME in <LuaPluginFunction.h>
#   include "LuaBind/lua_function.h"

    //
#   define LFUN__(S,A,H,I,O,F) __LUA_FUNC_CLASS_TEMPLATE(A,I,O,H,F)
#   include "LuaPluginFunction.h"

};

