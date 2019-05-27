
#include "plugin-lua.h"
#include <chrono>
#include <thread>

#include "plugin-lua-stack.cpp"

namespace Plugins
{

#if defined(__WIN32__)
  static inline const char *l_pluginPath = "\\plugins\\";
#else
  static inline const char *l_pluginPath = "/plugins/";
# define MAX_PATH 1024
#endif

    PluginLua::PluginLua(const char *s, const void *vcb)
        : m_lua(nullptr), m_luastate(0.0)
    {
        m_adbcmd = static_cast<Plugins::PluginCallBack_s const*>(vcb);
        m_priority = 5;
        m_name.assign(s);

        m_script.resize(MAX_PATH);

#       if defined(__WIN32__)
        if (::GetModuleFileName(NULL, m_script.data(), MAX_PATH))
#       else
        if (!::getcwd(m_script.data(), MAX_PATH))
#       endif
        {
            size_t pos = m_script.find_last_of("/\\");
            if (pos != std::string::npos)
                m_script = m_script.substr(0, pos);
            else
                m_script = ".";
        }
        else
            m_script = ".";

        m_script.append(l_pluginPath);
        m_script.append(m_name.c_str());
        m_script.append(".lua");

        if (!loadLuaScript(m_script.c_str()))
            m_isready = false;
        else
            m_isready = true;
        adbcmd = m_adbcmd;
    }

    PluginLua::~PluginLua() noexcept
    {
        if (m_lua)
            lua_close(m_lua);
        m_lua = nullptr;
    }

    void PluginLua::go(std::vector<uint8_t> const & v, uint32_t w, uint32_t h)
    {
        if (!m_lua)
        {
            m_isready = false;
            return;
        }

        lua_getglobal(m_lua, "main");
        if (!lua_isfunction(m_lua, -1))
        {
            m_isready = false;
            return;
        }

        adbbuffer = &v;
        //...
        std::this_thread::sleep_for(std::chrono::seconds(5));


        lua_pushnumber(m_lua, m_luastate);
        lua_pushinteger(m_lua, w);
        lua_pushinteger(m_lua, h);
        lua_pcall(m_lua, 3, 1, 0);

        if (!lua_isnil(m_lua, -1))
        {
            m_luastate = lua_tonumber(m_lua, -1);
            lua_pop(m_lua, 1);
        }
        adbbuffer = nullptr;
    }

    bool PluginLua::loadLuaScript(std::string const & fname)
    {
        do
        {
            m_lua = luaL_newstate();
            if (!m_lua)
                break;

            luaL_openlibs(m_lua);

            if (luaL_loadfile(m_lua, fname.c_str()))
                break;

            if (lua_pcall(m_lua, 0, 0, 0))
                break;

            lua_register(m_lua, "AdbClick",  &lua_Click);
            lua_register(m_lua, "AdbSwipe",  &lua_Swipe);
            lua_register(m_lua, "AdbKey",    &lua_Key);
            lua_register(m_lua, "AdbText",   &lua_Text);
            lua_register(m_lua, "AdbScreen", &lua_Screen);

            return true;
        }
        while (0);

        if (m_lua)
            lua_close(m_lua);
        m_lua = nullptr;

        return false;
    }

}



