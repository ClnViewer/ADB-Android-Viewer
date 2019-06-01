
#include "plugin-lua.h"
#include <chrono>
#include <thread>

namespace Plugins
{

#if defined(__WIN32__)
  static inline const char *l_pluginPath = "\\plugins\\";
#else
  static inline const char *l_pluginPath = "/plugins/";
# define MAX_PATH 1024
#endif
  static inline const char *l_pluginExt = ".lua";

    PluginLua::PluginLua(const char *s, const void *vcb)
    {
        m_priority = 5;
        m_name.assign(s);
        m_wcount = 0U;

        pathLuaScript();

        if (!m_luaobj.open(m_script.c_str()))
            m_isready = false;
        else
        {
            m_isready = true;
            m_luaobj.adbset(
                    (Plugins::PluginCallBack_s const*)vcb
                );
        }
    }

    PluginLua::~PluginLua()
    {
    }

    void PluginLua::pathLuaScript()
    {
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
        m_script.append(l_pluginExt);
    }

    void PluginLua::go(std::vector<uint8_t> const & v, uint32_t w, uint32_t h)
    {
        if (!m_isready)
            return;

        if ((m_wcount % 10) == 0)
        {
            m_wcount = 0U;

            if (!m_luaobj.load())
                return;
        }

        if (!m_luaobj.getfun("main"))
        {
            m_isready = false;
            return;
        }

        m_luaobj.point =
        {
            static_cast<int32_t>(w),
            static_cast<int32_t>(h)
        };
        m_luaobj.fbset(&v);

        lua_pushinteger(m_luaobj.instance(), m_luaobj.getstate());
        lua_pcall(m_luaobj.instance(), 1, 1, 0);

        if (!lua_isnil(m_luaobj.instance(), -1))
        {
            int lua_state = lua_tonumber(m_luaobj.instance(), -1);
            (void) lua_state;
            lua_pop(m_luaobj.instance(), 1);
        }

        if (m_luaobj.sleep)
        {
            auto s_start = std::chrono::high_resolution_clock::now();
            auto s_end = (s_start + std::chrono::seconds(m_luaobj.sleep));
            m_luaobj.sleep = 0U;

            while (std::chrono::high_resolution_clock::now() < s_end)
            {
                if (!m_isready)
                    break;
                std::this_thread::yield();
            }
        }

        m_luaobj.fbset(nullptr);
        m_luaobj.point = { 0, 0 };
    }
}



