/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

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
        lua_pop(m_lua, 1);
        __LUA_IF_LINT(
            LuaLint::print_lua(m_lua, LuaLint::ColorPrint::LuaFunctionPrint);
        )
        ::lua_pushnil(m_lua);
        return 0;
    }
    __LUA_FUNC_CODE_HEADER(dprint)
    {
        lua_pop(m_lua, 1);
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
#   define LFUN__(S,A,H1,H2,I,O,F) __LUA_FUNC_CLASS_TEMPLATE(A,I,O,H1 H2,F)
#   include "LuaPluginFunction.h"

};

