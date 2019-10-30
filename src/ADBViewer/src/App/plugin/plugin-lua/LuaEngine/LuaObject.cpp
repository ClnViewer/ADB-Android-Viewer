
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <lua.hpp>
#include <ImageLite.h>
#include "LuaObject.h"

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
                        lua_getglobal(L, var.c_str());
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
                lua_getglobal(L, var.c_str());
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

            lua_getglobal(L, s.c_str());
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

        void close_(lua_State **lua)
        {
            lua_State *L = *lua;
            if (L)
                lua_close(L);
            *lua = nullptr;
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
                lua_getglobal(L, "_G");
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
};
