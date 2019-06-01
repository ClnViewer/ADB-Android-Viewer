
#include "../../plugin-base.h"
#include <array>
#include <fstream>
#include <streambuf>

#include <sys/stat.h>
#include <lua.hpp>

#include "LuaObject.h"
#include "LuaObjectBind.h"

#if defined(_DEBUG)
// #  include <iostream>
#endif

#include "LuaObject.h"
#include "LuaObjectBind.h"

LuaObject::LuaObject()
    : m_lua(nullptr), m_watch(0), m_laststate(0.0),
      m_adbcmd(nullptr), m_vfb(nullptr),
      sleep(0), point{} {}

LuaObject::LuaObject(std::string const & fname)
    : m_lua(nullptr), m_watch(0), m_laststate(0.0),
      m_adbcmd(nullptr), m_vfb(nullptr),
      sleep(0), point{}
    {
        open(fname);
    }

LuaObject::~LuaObject() noexcept
    {
        close();
    }

void LuaObject::adbset(Plugins::PluginCallBack_s const *adbcmd)
    {
        m_adbcmd = adbcmd;
    }

void LuaObject::fbset(std::vector<uint8_t> const *vfb)
    {
        m_vfb = vfb;
    }

int32_t LuaObject::getstate() const
    {
        return static_cast<int32_t>(m_laststate);
    }

lua_State * LuaObject::instance() const
    {
        return m_lua;
    }

LuaObject * LuaObject::instance(lua_State *L)
    {
        lua_getglobal(L, "LuaObject");
        if (!lua_istable(L, -1))
            return nullptr;

        lua_pushstring(L, "_self");
        lua_gettable(L, -2);

        if (!lua_isuserdata(L, -1))
            return nullptr;

        LuaObject *p = (LuaObject*)lua_touserdata(L, -1);
        lua_pop(L, 1);
        return p;
    }

bool LuaObject::init()
    {
        close();

        if (!(m_lua = luaL_newstate()))
            return false;
        luaL_openlibs(m_lua);

        lua_createtable(m_lua, 0, 2);
        lua_pushlightuserdata(m_lua, this);
        lua_setfield(m_lua, -2, "_self");

        for (uint32_t i = 0; i < __NELE(l_func); i++)
        {
            lua_pushcfunction(m_lua, l_func[i].fun);
            lua_setfield(m_lua, -2, l_func[i].name);
        }

        lua_setglobal(m_lua, "LuaObject");
        return true;
    }

void LuaObject::close()
    {
        if (m_lua)
            lua_close(m_lua);
        m_lua = nullptr;
    }

void LuaObject::clean()
    {
        if (m_lua)
            lua_pop(m_lua, lua_gettop(m_lua));
    }

bool LuaObject::check()
    {
        return (m_lua);
    }

bool LuaObject::getfun(std::string const & fun)
    {
        if ((fun.empty()) || (!check()))
            return false;

        lua_getglobal(m_lua, fun.c_str());
        return lua_isfunction(m_lua, -1);
    }

bool LuaObject::open(std::string const & fname)
    {
        if (fname.empty())
            return false;

        m_path.assign(fname);
        return load();

    }

bool LuaObject::load()
    {
        do
        {
            struct _stat st{};

            if (m_path.empty())
                break;

            if (::_stat(m_path.c_str(), &st) < 0)
                break;

            if (m_watch == st.st_mtime)
                return true;

            m_watch = st.st_mtime;

            if (!init())
                break;

            if (luaL_loadfile(m_lua, m_path.c_str()))
                break;

            if (lua_pcall(m_lua, 0, 0, 0))
                break;

            return true;
        }
        while (0);

#       if defined(_DEBUG)
        // std::cout << lua_tostring(m_lua, 1) << std::endl;
        MessageBoxA(NULL, lua_tostring(m_lua, 1), __func__, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
#       endif

        close();
        m_path.assign("");
        return false;
    }

bool LuaObject::getstack_(std::string const & vname)
    {
        int32_t idx = 0;
        std::string var = "";

        if (!check())
            return false;

        for (uint32_t i = 0U; i < vname.size(); i++)
        {
            if (vname.at(i) == '.')
            {
                if (idx == 0)
                    lua_getglobal(m_lua, var.c_str());
                else
                    lua_getfield(m_lua, -1, var.c_str());

                if (lua_isnil(m_lua, -1))
                    return false;
                else
                {
                    var = "";
                    idx++;
                }
            }
            else
                var += vname.at(i);
        }

        if (idx == 0)
            lua_getglobal(m_lua, var.c_str());
        else
            lua_getfield(m_lua, -1, var.c_str());

        if (lua_isnil(m_lua, -1))
            return false;
        return true;
    }

void LuaObject::dump()
    {
#       if defined(_DEBUG)
        int32_t i, n = lua_gettop(m_lua);
        // std::cout << " ---------------- Lua Stack Dump ----------------" << std::endl;

        if (!check())
            return;

        for (i = 0U; i < n; i++)
        {
            int32_t t = lua_type(m_lua, i);
            switch (t)
            {
                case LUA_TSTRING:
                    {
                        // std::cout << "[" << i << "] [string] : " << lua_tostring(m_lua, i) << std::endl;
                        MessageBoxA(NULL, lua_tostring(m_lua, i), __func__, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
                        break;
                    }
                case LUA_TBOOLEAN:
                    {
                        // std::cout << "[" << i << "] [boolean] : " << lua_toboolean(m_lua, i) << std::endl;
                        std::string info(std::to_string(lua_toboolean(m_lua, i)));
                        MessageBoxA(NULL, info.c_str(), __func__, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
                        break;
                    }
                case LUA_TNUMBER:
                    {
                        // std::cout << "[" << i << "] [number]  : " << lua_tonumber(m_lua, i) << std::endl;
                        std::string info(std::to_string(lua_tonumber(m_lua, i)));
                        MessageBoxA(NULL, info.c_str(), __func__, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
                        break;
                    }
                default:
                    {
                        //std::cout << "[" << i << "] [unknown]" << lua_typename(m_lua, i) << std::endl;
                        std::string info("[unknown]");
                        MessageBoxA(NULL, info.c_str(), __func__, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
                        break;
                    }
            }
        }
#   endif
    }
