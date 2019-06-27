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

#include "../../ADBViewer.h"

#if defined(OS_WIN)
#  include "IPluginLoader.WINAPI.h"
#else
#  include "IPluginLoader.LINUX.h"
#endif

using namespace std::placeholders;

namespace Plugins
{

typedef Plugins::AppIPlugin * (*f_Fn_construct)(const char*, const void*);
typedef void (*f_Fn_destruct)(void);

static inline const char *l_nameCreatePlugin = "CreatePlugin";
static inline const char *l_nameDestroyPlugin = "DestroyPlugin";

AppPluginManager::AppPluginManager()
        : m_isrun(false)
    {
        m_loadfunc = std::make_unique<Plugins::PluginLoader>();
    }

AppPluginManager::~AppPluginManager()
    {
        m_isrun = false;

        if (m_thr.joinable())
            m_thr.join();

        m_isrun = true;
        freeplugins();
    }

AppPluginManager& AppPluginManager::instance()
    {
        static AppPluginManager m_instance{};
        return m_instance;
    }

void AppPluginManager::init()
    {
        if (m_plugins.size())
            return;

        m_isrun = true;

        std::thread thr
        {
            [=]()
            {
                m_loadfunc->PluginFind(
                        std::bind(&Plugins::AppPluginManager::addplugin, this, _1, _2, _3)
                    );

                if (!m_isrun.load())
                    return;

                if (m_plugins.size() > 1U)
                {
                    std::sort(
                            m_plugins.begin(), m_plugins.end(),
                            [=](const auto& lplg, const auto& rplg)
                            {
                                if (!lplg.iplug)
                                    return false;
                                return (lplg.iplug->priority() < rplg.iplug->priority());
                            }
                        );
                }
                m_isrun = false;
            }
        };
        m_thr = move(thr);
    }

void AppPluginManager::run(std::vector<uint8_t> &v, uint32_t w, uint32_t h)
    {
        if (m_isrun.load())
            return;

        if (m_thr.joinable())
            m_thr.join();

        if (!m_plugins.size())
            return;

        /// lock
        {
            bool isready = false;

            for (auto &plg : m_plugins)
            {
                if ((!plg.state) && (plg.handle))
                    disableplugin(plg);
                if ((!plg.state) || (!plg.iplug))
                    continue;
                {
                    std::lock_guard<std::mutex> l_lock(m_lock);
                    if ((isready = plg.iplug->ready()))
                        break;
                }
            }
            if (!isready)
                return;
        }

        m_isrun = true;

        std::vector<uint8_t> vt(v);
        std::thread thr
        {
            [=]()
            {
                for (auto &plg : m_plugins)
                {
                    if (!m_isrun.load())
                        break;
                    if ((!plg.state) || (!plg.iplug))
                        continue;
                    {
                        std::lock_guard<std::mutex> l_lock(m_lock);
                        if (plg.iplug->ready())
                            plg.iplug->go(vt, w, h);
                    }
                }
                m_isrun = false;
            }
        };
        m_thr = move(thr);
    }

bool AppPluginManager::isplugin(std::string const & s)
    {
        //std::lock_guard<std::mutex> l_lock(m_lock);
        auto plg = find_if(
                    m_plugins.begin(),
                    m_plugins.end(),
                    [=](Plugin_s pl)
                    {
                        return (pl.name.compare(0, pl.name.length(), s) == 0);
                    }
            );
        return (plg != m_plugins.end());
    }

AppPluginManager::Plugin_s * AppPluginManager::findplugin(std::string const & s)
    {
        // std::lock_guard<std::mutex> l_lock(m_lock);
        auto plg = find_if(
                    m_plugins.begin(),
                    m_plugins.end(),
                    [=](AppPluginManager::Plugin_s pl)
                    {
                        return (pl.name.compare(s) == 0);
                    }
            );
        if (plg == m_plugins.end())
            return nullptr;
        return &plg[0];
    }

void AppPluginManager::addplugin(std::string const & sp, std::string const & sn, bool isenable)
    {
        if (isplugin(sn))
            return;

        AppPluginManager::Plugin_s plg{};
        plg.state = false;

#       if defined(OS_WIN)
        int32_t off = 4;
#       elif defined(__APPLE__)
        int32_t off = 6;
#       else
        int32_t off = 3;
#       endif

        std::stringstream ss;
        ss << sp.c_str() << "\\" << sn.c_str();
        plg.path = ss.str();
        plg.name = sn.substr(0, sn.length() - off);

        if (isenable)
        {
            if (enableplugin(plg))
                m_plugins.push_back(plg);
        }
        else
        {
            plg.state = false;
            plg.handle = nullptr;
            plg.iplug = nullptr;
            m_plugins.push_back(plg);
        }
    }

bool AppPluginManager::enableplugin(AppPluginManager::Plugin_s & plg)
    {
        do
        {
            if (plg.handle)
                disableplugin(plg);

            if ((plg.path.empty()) || (plg.name.empty()))
                break;

            std::lock_guard<std::mutex> l_lock(m_lock);
            {
                if (!(plg.handle = m_loadfunc->PluginOpen(plg.path.c_str())))
                    break;

                f_Fn_construct func = (f_Fn_construct)
                    m_loadfunc->PluginInstance(plg.handle, l_nameCreatePlugin);
                if (!func)
                    break;

                if (!(plg.iplug = func(plg.name.c_str(), AppConfig::instance().GetAdbCb())))
                    break;

                plg.state = true;
            }

            return plg.state;
        }
        while (0);

        disableplugin(plg);
        return plg.state;
    }

void AppPluginManager::disableplugin(AppPluginManager::Plugin_s & plg)
    {
        std::lock_guard<std::mutex> l_lock(m_lock);
        plg.state = false;

        if (plg.iplug)
        {
            f_Fn_destruct func = (f_Fn_destruct)
                m_loadfunc->PluginInstance(plg.handle, l_nameDestroyPlugin);
            if (func)
                func();
        }
        if (plg.handle)
            m_loadfunc->PluginClose(plg.handle);

        plg.iplug = nullptr;
        plg.handle = nullptr;
    }

bool AppPluginManager::disableplugin(std::string const & s)
    {
        AppPluginManager::Plugin_s *plg = findplugin(s);

        if (!plg)
            return false;
        plg->state = false;
        return true;
    }

bool AppPluginManager::enableplugin(std::string const & s)
    {
        AppPluginManager::Plugin_s *plg = findplugin(s);

        if (!plg)
            return false;
        return enableplugin(*plg);
    }

std::vector<AppPluginManager::Plugin_s> AppPluginManager::listplugin() const
    {
        return m_plugins;
    }

void AppPluginManager::freeplugins() noexcept
    {
        if (!m_plugins.size())
            return;

        for (auto &plg : m_plugins)
            disableplugin(plg);
    }
}
