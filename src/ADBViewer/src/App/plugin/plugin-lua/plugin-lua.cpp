/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

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



