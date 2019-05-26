
#include "plugin-lua.h"
#include <chrono>
#include <thread>

namespace Plugins
{
    PluginLua::PluginLua(const void *vcb)
        : m_lua(nullptr)
    {
        m_adbcmd = static_cast<Plugins::PluginCallBack_s const*>(vcb);
        m_priority = 9;

        if (!loadLuaScript("plugin-script.lua"))
            m_isready = false;
        else
            m_isready = true;
    }

    PluginLua::~PluginLua()
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


        //...
        std::this_thread::sleep_for(std::chrono::seconds(5));

        GameDev::ADBDriver::Tap_s t{};

        t = { 610, 318 };
        m_adbcmd->click(&t);
        m_isready = false;
    }

    bool PluginLua::loadLuaScript(std::string const & fname)
    {
        m_lua = luaL_newstate();
        if (
            (!m_lua) ||
            (luaL_loadfile(m_lua, fname.c_str())) ||
            (lua_pcall(m_lua, 0, 0, 0))
            )
        {
            m_lua = nullptr;
            return false;
        }
        return true;
    }

}



