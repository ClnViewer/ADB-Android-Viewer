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

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <functional>
#include <optional>
#include <lua.hpp>
#include <ImageLite.h>
#include "LuaObject.h"
#include "LuaLint.h"

namespace LuaObject
{
        bool getstack_(lua_State *L, std::string const & vname)
        {
            int32_t idx = 0;
            std::string var = "";

            for (uint32_t i = 0U; i < vname.size(); i++)
            {
                if (vname.at(i) == '.')
                {
                    if (idx == 0)
                    {
                        if (lua_getglobal(L, var.c_str()) == LUA_TNIL)
                            return false;
                    }
                    else
                        lua_getfield(L, -1, var.c_str());

                    if (lua_isnil(L, -1))
                        return false;
                    else
                    {
                        var = ""; idx++;
                    }
                }
                else
                    var += vname.at(i);
            }
            if (idx == 0)
            {
                if (lua_getglobal(L, var.c_str()) == LUA_TNIL)
                   return false;
            }
            else
                lua_getfield(L, -1, var.c_str());
            if (lua_isnil(L, -1))
                return false;
            return true;
        }

        void stackdump_(lua_State *L, std::function<void(std::string const&, int32_t)> fun)
        {
            int32_t i = ::lua_gettop(L);
            fun("[Start] ", i);

            while(i)
            {
                int32_t t = ::lua_type(L, i);
                switch (t)
                {
                    case LUA_TSTRING:  fun(::lua_tostring(L, i), i); break;
                    case LUA_TBOOLEAN: fun(((::lua_toboolean(L, i)) ? "true" : "false"), i);  break;
                    case LUA_TNUMBER:  fun(::lua_tostring(L, i), i); break;
                    default:           fun(::lua_typename(L, t), i); break;
                }
                i--;
            }
            fun("[End] ", 0);
        }

        int32_t tablecount_(lua_State *L)
        {
            int32_t idx = 0;
            ::luaL_checktype(L, 1, LUA_TTABLE);
            while (::lua_next(L, 1))
            {
                ::lua_pop(L, 1); idx++;
            }
            return idx;
        }

        bool getfunction_(lua_State *L, std::string const & s)
        {
            if (s.empty())
                return false;

            if (lua_getglobal(L, s.c_str()) == LUA_TNIL)
                return false;
            return lua_isfunction(L, -1);
        }

        std::string trace_(lua_State *L)
        {
            lua_State *L1 = L;
            luaL_traceback(L, L1, nullptr, 1);
            std::string s = lua_tostring(L, -1);
            lua_pop(L, 1);
            return s;
        }

        void clean_(lua_State *L)
        {
            if (L)
                lua_pop(L, lua_gettop(L));
        }

        void close_(std::optional<lua_State*> & mlua)
        {
            if (!mlua.has_value())
                return;

            lua_close(mlua.value());
            mlua.reset();
        }

        void close_(lua_State **lua)
        {
            lua_State *L = *lua;
            if (L)
                lua_close(L);
            *lua = nullptr;
        }

        bool init_(
            std::optional<lua_State*> & mlua,
            std::string const & s,
            struct luaL_Reg *fun_redefine,
            struct luaL_Reg *fun_object,
            int32_t sz,
            void *instance)
        {
            lua_State *L = nullptr;
            if (!init_(&L, s, fun_redefine, fun_object, sz, instance))
                return false;
            mlua.emplace(L);
            return true;
        }

        bool init_(
            lua_State **lua,
            std::string const & s,
            struct luaL_Reg *fun_redefine,
            struct luaL_Reg *fun_object,
            int32_t sz,
            void *instance)
        {
            lua_State *L;
            if (!(L = luaL_newstate()))
            {
                *lua = nullptr;
                return false;
            }
            //
            luaL_checkversion(L);
            luaL_openlibs(L);
            //
            if (fun_redefine)
            {
                if (lua_getglobal(L, "_G") == LUA_TNIL)
                    return false;
                luaL_setfuncs(L, fun_redefine, 0);
                lua_pop(L, 1);
            }
            //
            if (!s.empty())
            {
                int32_t off = ((instance) ? 1 : 0);
                sz = ((sz > 0) ? (sz + off) : off);
                if (!sz)
                    return false;

                lua_createtable(L, 0, sz);
                if (instance)
                {
                    lua_pushlightuserdata(L, instance);
                    lua_setfield(L, -2, "_self");
                }
                if ((sz > 1) && (fun_object))
                    luaL_setfuncs(L, fun_object, 0);
                lua_setglobal(L, s.c_str());
            }
            *lua = L;
            return true;
        }

        void pcall_error_print_(std::optional<lua_State*> const & mlua, int32_t err)
        {
            if (!mlua.has_value())
                pcall_error_print_(nullptr, err);
            else
                pcall_error_print_(mlua.value(), err);
        }

        void pcall_error_print_(lua_State *L, int32_t err)
        {
            LuaLint::print_pcall_error(err);
            if (!L)
                return;

            //
            std::string s = lua_tostring(L, -1);
            if (!s.empty())
                LuaLint::print(s, LuaLint::ColorPrint::DebugError);
            //
            s = trace_(L);
            if (!s.empty())
                LuaLint::print(s, LuaLint::ColorPrint::DebugTraceStack);
        }

};
