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
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#if defined(__WIN32__)
#  include <io.h>
#else
#  include <stdio.h>
#  include <stdlib.h>
#  define _access access
#endif

namespace Plugins
{

#   if defined(__WIN32__)
    static inline const char *l_pluginPath = "\\plugins\\";
#   else
    static inline const char *l_pluginPath = "/plugins/";
#   define MAX_PATH 1024
#   endif
    static inline const char *l_pluginExt0 = ".luac";
    static inline const char *l_pluginExt1 = ".lua";

    static bool f_check_path(std::string & s, std::string const & name, std::string const & ext)
    {
        s = name;
        s.append(ext);
        bool b = !(::_access(s.c_str(), F_OK) < 0);
        if (!b)
            s = "";
        return b;
    }

    PluginLua::PluginLua(const char *s, const void *vcb)
    {
        m_priority = 5;
        m_name.assign(s);
        m_scount = 0U;

        pathLuaScript();

        if (!m_luae.load(m_script.c_str()))
            m_isready = false;
        else
        {
            m_isready = true;
            m_luae.setadb(
                    (Plugins::PluginCallBack_s const*)vcb
                );
        }
    }

    PluginLua::~PluginLua()
    {
    }

    void PluginLua::pathLuaScript()
    {
        std::stringstream ss;
        std::string       s;
                          s.resize(MAX_PATH);

#       if defined(__WIN32__)
        if (::GetModuleFileName(NULL, s.data(), MAX_PATH))
#       else
        if (!::getcwd(s.data(), MAX_PATH))
#       endif
        {
            size_t pos;
            if ((pos = s.find_last_of("/\\")) != std::string::npos)
                ss << s.substr(0, pos);
            else
                ss << ".";
        }
        else
            ss << ".";
        ss << l_pluginPath << m_name.c_str();

        if (!f_check_path(m_script, ss.str(), l_pluginExt0))
            if (!f_check_path(m_script, ss.str(), l_pluginExt1))
                m_script = "";
    }

    void PluginLua::go(std::vector<uint8_t> const & v, uint32_t w, uint32_t h)
    {
        if (!m_isready)
            return;

        do
        {
            if ((m_scount % 10) == 0)
            {
                m_scount = 0U;
                if (!m_luae.load())
                    break;
                if (!m_luae.getfunction("main"))
                    break;
                if (!m_luae.setfb(v, w, h))
                    return;

                ::lua_pushinteger(m_luae.instance(), m_sret.load());
                if (::lua_pcall(m_luae.instance(), 1, 1, 0) != LUA_OK)
                    break;

                if (m_luae.getrunbreak())
                    break;

                if (!lua_isnil(m_luae.instance(), -1))
                {
                    m_sret = ::lua_tointeger(m_luae.instance(), -1);
                    ::lua_pop(m_luae.instance(), 1);
                }

                // ? default wait 1 sec.
                if (m_luae.m_sleep.load())
                {
                    auto t_start = std::chrono::high_resolution_clock::now();
                    auto t_end = (t_start + std::chrono::seconds(m_luae.m_sleep.load()));

                    while (std::chrono::high_resolution_clock::now() < t_end)
                    {
                        if ((!m_isready) || (m_luae.getrunbreak()))
                            break;
                        std::this_thread::yield();
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                }
            }
            return;
        }
        while (0);

        m_isready = false;
        m_luae.close();
    }
}



