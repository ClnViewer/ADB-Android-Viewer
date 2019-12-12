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

 /*
    LUA PLUGIN REDEFINE DEBUG PRINT:
    --
    --  DON'T MODIFY!
    local ENV = {}
    setmetatable(ENV, {
        __index = _ENV
    })

    LUA PLUGIN INTERFACE:
    --
    --
    local plugin = {}
    --
    plugin.name = "TestPlugin"
    plugin.title = "Test SCEdit plugin"
    plugin.type = 1 -- parse running you code, argument 'str' is empty
    plugin.type = 2 -- parse input string, argument 'str'  is you code
    plugin.savename = nil
    --
    function plugin.init(p,d,s)
        ENV._G.print  = p
        ENV._G.dprint = d
        plugin.savename = s
    end
    --
    function plugin.callplugin(str)
        local o = { { string }, { string }, ...}
        return o
    end
    --
    function plugin.debugplugin(str)
        local o = { { string }, { string }, ...}
        for k in pairs(o) do
            print(o[k])
        end
        return o
    end
    --
    return plugin

    ///

    Editor callback in plugin:

    --
    local function editor(str)
        if str == nil then
           return
        end
        ... other code ...
        LuaObject:TextInsert("-- This should work TextInsert\n")
        LuaObject:TextReplace("-- This should work TextReplace\n")
    end
    --

 */

#include <SCEditInternal.h>
#include <lua.hpp>
#include <ctime>
#include <filesystem>

#include <ImageLite.h>
#include "LuaLint.h"
#include "LuaObject.h"
#include "LuaPlugin.Editor.String.h"

#define break_error(A) { ret = A; break; }
// Debug plugins interface:
//#define _BUILD_DEBUG_LUAPLUGIN 1

namespace fs = std::filesystem;

namespace Editor
{
#   if defined(_BUILD_DEBUG_LUAPLUGIN)
    static const char *l_plug_call_function = "debugplugin";
    static bool f_out_mute_(bool)
    {
        return false;
    }
#   else
    static const char *l_plug_call_function = "callplugin";
    static bool f_out_mute_(bool ismute)
    {
        ::SendMessageA(
            CONF_MAIN_HANDLE(),
            WM_COMMAND,
            (WPARAM)IDM_BTN_DBGMUTE,
            (LPARAM)0
        );
        return !(ismute);
    }
#   endif
    //
    static std::atomic<bool> l_ispanic = false;
    extern "C" int32_t f_atpanic_(lua_State *L)
    {
        LuaLint::print(l_err_str_08, LuaLint::ColorPrint::DebugError);
        if (L)
        {
            lua_pop(L, 1);
            std::string s = lua_tostring(L, -1);
            if (!s.empty())
                LuaLint::print(s, LuaLint::ColorPrint::DebugError);
        }
        l_ispanic = true;
        return 1;
    }
    extern "C" int32_t f_bind_emptyd(lua_State*)
    {
        return 0;
    }
    extern "C" int32_t f_bind_printd(lua_State *L)
    {
        LuaLint::print_lua(L, LuaLint::ColorPrint::LuaFunctionPrint);
        ::lua_pushnil(L);
        return 0;
    }

    ///

    template <uint32_t ID>
    int32_t f_bind_EditorService(lua_State *L)
    {
        do
        {
            if (!lua_isstring(L, -1))
                break;

            std::string s = lua_tostring(L, -1);
            if (s.empty())
                break;

            ::SendMessageA(
                    MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
                    WM_COMMAND,
                    (WPARAM)ID,
                    reinterpret_cast<LPARAM>(s.data())
                );
            ::lua_pop(L, 1);
        }
        while (0);
        ::lua_pushnil(L);
        return 0;
    }
    //
    static struct luaL_Reg l_func_empty_GlobRedefine[] =
    {
        { "print",  &f_bind_emptyd },
        { "dprint", &f_bind_emptyd },
        { nullptr, nullptr }
    };
    //
    static struct luaL_Reg l_func_print_GlobRedefine[] =
    {
        { "print",  &f_bind_printd },
        { "dprint", &f_bind_printd },
        { nullptr, nullptr }
    };
    //
    static struct luaL_Reg l_func_LuaObject[] =
    {
#       define LFUN__(S,A,...) { #A, f_bind_emptyd },
#       include "LuaPluginFunction.h"
        { "TextInsert",  &f_bind_EditorService<IDM_EVENT_TXT_INSERT> },
        { "TextReplace", &f_bind_EditorService<IDM_EVENT_TXT_NEW> },
        { nullptr, nullptr }
    };
    //
    // Public
    //
    LuaPluginEditor::LuaPluginEditor()
    {
#       if defined(_BUILD_DEBUG_LUAPLUGIN)
        LuaLint::settrace(true);
#       endif
    }

    LuaPluginEditor::~LuaPluginEditor()
    {
        LuaObject::close_(m_lua);
    }

    bool LuaPluginEditor::init()
    {
        try
        {
            do
            {
                //
                {
                    m_plug.reset();
                    m_lua_source.reset();
                    LuaObject::close_(m_lua);
                }
                //
                if (!m_map.empty())
                    m_map.clear();
                //
                fs::path l_root{ "plugins" };
                if (!fs::exists(l_root))
                    break;
                if (!fs::is_directory(l_root))
                    break;

                fs::directory_iterator sfiles(l_root);
                std::vector<fs::path> vfiles;
                std::copy_if(
                    begin(sfiles),
                    end(sfiles),
                    std::back_inserter(vfiles),
                    [](const fs::path & p)
                    {
                        if ((fs::is_regular_file(p)) &&
                            (p.filename().generic_string().length() > 11) &&
                            (p.extension() == ".lua"))
                             //
                            if (p.filename().generic_string().compare(
                                        0,
                                        11,
                                        "sce-plugin-",
                                        11
                                ) == 0)
                                return true;
                        return false;
                    }
                );
                if (!vfiles.size())
                    break;

                uint32_t uid = IDM_BTN_PLUGIN_FIRST;
                for (auto & p : vfiles)
                {
                    if (++uid > IDM_BTN_PLUGIN_END)
                        break;
                    //
                    {
                        do
                        {
                            std::string pname = p.stem().generic_string();
                            if (pname.empty())
                                break;
                            if (!load_("", pname, l_func_empty_GlobRedefine))
                                break;
                            std::string ptitle = getptitle_();
                            if (ptitle.empty())
                                break;
                            PluginType  ptype = getptype_();
                            //
                            if ((ptype != PluginType::PLUGT_RUNCODE) &&
                                (ptype != PluginType::PLUGT_PARSECODE))
                            {
                                local_error_print_(PlugIntError::ERR_PLUGTYPE);
                                break;
                            }
                            //
                            auto [it, result] = m_map.try_emplace(
                                    uid,
                                    std::make_tuple(uid, ptype, pname, ptitle)
                            );
                            if (!result)
                            {
                                std::stringstream ss;
                                ss << l_err_str_20 << pname.c_str();
                                LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugError);
                            }
                        }
                        while (0);
                        LuaObject::close_(m_lua);
                        m_plug.reset();
                    }
                    //
                }
                return true;
            }
            while (0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), l_uexception); }
        return false;
    }

    bool LuaPluginEditor::open(uint32_t uid, std::string const & s)
    {
        try
        {
            do
            {
                LuaObject::close_(m_lua);
                m_plug.reset();
                //
                if ((uid < IDM_BTN_PLUGIN_FIRST) ||
                    (uid > IDM_BTN_PLUGIN_END))
                    break;

                auto obj = m_map.find(uid);
                if (obj == m_map.end())
                    break;

                auto it = obj->second;

                fs::path l_plug{ "plugins" };
                l_plug /= std::get<2>(it);
                l_plug += ".lua";
                if (!fs::exists(l_plug))
                {
                    LuaLint::print(l_err_str_10, LuaLint::ColorPrint::DebugError);
                    break;
                }
                LuaPluginEditor::PluginType plgt =
                        static_cast<PluginType>(std::get<1>(it));
                //
                if (plgt == PluginType::PLUGT_RUNCODE)
                {
                    if (!load_(s, std::get<2>(it), l_func_print_GlobRedefine))
                        break;
                }
                else if (plgt == PluginType::PLUGT_PARSECODE)
                {
                    if (!load_("", std::get<2>(it), l_func_print_GlobRedefine))
                        break;
                    m_lua_source.emplace(s);
                }
                else if (plgt == PluginType::PLUGT_NONE)
                    break;
                //
                m_plug.emplace(it);
                return true;
            }
            while (0);
            m_plug.reset();
            LuaObject::close_(m_lua);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), l_uexception); }
        return false;
    }

    void LuaPluginEditor::close()
    {
        if (m_lua.has_value())
            LuaObject::close_(m_lua);
        m_plug.reset();
    }

    std::optional<uint32_t> LuaPluginEditor::getid()
    {
        if (!m_plug.has_value())
            return {};
        return { std::get<0>(m_plug.value()) };
    }

    std::optional<std::string> LuaPluginEditor::getname()
    {
        if (!m_plug.has_value())
            return {};
        return { std::get<2>(m_plug.value()) };
    }

    std::optional<std::string> LuaPluginEditor::gettitle()
    {
        if (!m_plug.has_value())
            return {};
        return { std::get<3>(m_plug.value()) };
    }

    std::optional<LuaPluginEditor::PluginType> LuaPluginEditor::gettype()
    {
        if (!m_plug.has_value())
            return {};
        return { static_cast<LuaPluginEditor::PluginType>(std::get<1>(m_plug.value())) };
    }

    std::vector<MDIWin::BaseData::dataMap> LuaPluginEditor::getlist()
    {
        std::vector<MDIWin::BaseData::dataMap> v;
        for (const auto & [key, val] : m_map)
            v.push_back(val);
        return v;
    }

    bool LuaPluginEditor::run()
    {
        uint32_t ret    = 0U;
        int32_t  err    = LUA_OK;
        //
        do
        {
            if (!m_lua.has_value())
                break_error(PlugIntError::ERR_LUAHANDLE);
            if (!m_plug.has_value())
                break_error(PlugIntError::ERR_PLUGINDATA);
            //
            if (::lua_getfield(m_lua.value(), -1, l_plug_call_function) != LUA_TFUNCTION)
                break_error(PlugIntError::ERR_GETFUNCTION);
            if (!lua_isfunction(m_lua.value(), -1))
                break_error(PlugIntError::ERR_LUATYPE);
            //
            int32_t  nargs = 0;
            LuaPluginEditor::PluginType plgt =
                        static_cast<PluginType>(std::get<1>(m_plug.value()));
            //
            if (plgt == PluginType::PLUGT_PARSECODE)
            {
                if (!m_lua_source.has_value())
                    break_error(PlugIntError::ERR_LUASRCEMPTY);
                nargs = 1;
                ::lua_pushstring(m_lua.value(), m_lua_source.value().data());
            }
            else if (plgt == PluginType::PLUGT_NONE)
                break_error(PlugIntError::ERR_PLUGTYPE);
            //
            if ((err = lua_pcall(m_lua.value(), nargs, LUA_MULTRET, 0)) != LUA_OK)
                break;
            if (lua_isnil(m_lua.value(), -1))
                break_error(PlugIntError::ERR_LUANIL);
            //
            {
                std::string splug = std::get<3>(m_plug.value());
                std::stringstream ss;
                ss << l_err_str_000;
                if (!splug.empty())
                    ss << "' " << splug.c_str() << " ' ";

                std::vector<std::string> vtbl = LuaObject::getT<std::string>(m_lua.value());

                if (vtbl.size())
                {
                    EditBox::AnnotateData ad = { 0, std::string(), vtbl };
                    ::SendMessageA(
                        CONF_MAIN_HANDLE(),
                        WM_COMMAND,
                        (WPARAM)IDM_EVENT_SCRUN_MONITOR,
                        reinterpret_cast<LPARAM>(&ad)
                    );
                    ss << l_err_str_001;
                }
                else
                    ss << l_err_str_002;
                splug = ss.str();
                LuaLint::print(splug, LuaLint::ColorPrint::DebugHelp);
            }
            //
        }
        while (0);

        m_lua_source.reset();

        if (err != LUA_OK)
            LuaObject::pcall_error_print_(m_lua, err);
        //
        if (ret)
            local_error_print_(ret);
        //
        if ((ret) || (err != LUA_OK))
        {
            if (m_lua)
                LuaObject::close_(m_lua);

            m_plug.reset();
            return false;
        }
        return true;
    }

    //
    // Private
    //

    bool LuaPluginEditor::luainit_(luaL_Reg *tbl)
    {
        if (!LuaObject::init_(
                m_lua,
                "LuaObject",
                tbl,
                l_func_LuaObject,
                __NELE(l_func_LuaObject),
                this
            ))
            return false;

        lua_atpanic(m_lua.value(), &f_atpanic_);
        return true;
    }

    bool LuaPluginEditor::luacode_(std::string const & s, std::string const & p)
    {
        std::stringstream ss;
        ss << "\nplugin = require(\"plugins." << p.c_str() << "\")\n\n";
        //
        if (!s.empty())
        {
            std::size_t ipos = 0;
            do
            {
                std::string rs("return");
                auto is = std::find_end(
                                std::begin(s),
                                std::end(s),
                                std::begin(rs),
                                std::end(rs)
                        );
                if (is == std::end(s))
                    break;
                //
                bool isnline = false;
                     ipos    = (is - std::begin(s));
                //
                for (std::size_t i = (ipos + rs.length()); i < s.length(); i++)
                {
                    const char c = s.at(i);
                    if ((!isnline) && (c == '\n'))
                    {
                        isnline = true;
                        continue;
                    }
                    else if (!isnline)
                        continue;
                    else if ((c != '\n') &&
                             (c != '\r') &&
                             (c != '\t') &&
                             (c != ' '))
                    {
                        ipos = 0;
                        break;
                    }
                }
            }
            while (0);

            if (!ipos)
                ss << s.c_str();
            else
                ss << s.substr(0, ipos).c_str();
        }
        //
        ss << "\nplugin.init(_ENV._G.print, _ENV._G.dprint, ";
        if (MDIWin::Config::instance().savepath.empty())
            ss << "nil";
        else
            ss << "\"" << MDIWin::Config::instance().savepath.c_str() << "\"";
        ss << ")\nplugin." << l_plug_call_function << "()\n";
        std::string src = ss.str();
        if (luaL_loadstring(m_lua.value(), src.c_str()))
            return false;
        return true;
    }

    void LuaPluginEditor::local_error_print_(uint32_t err)
    {
        std::string s;
        switch (static_cast<PlugIntError>(err))
        {
            case PlugIntError::ERR_INIT:        s = l_err_str_01; break;
            case PlugIntError::ERR_CODE:        s = l_err_str_02; break;
            case PlugIntError::ERR_GETMODULE:   s = l_err_str_03; break;
            case PlugIntError::ERR_GETVALUE:    s = l_err_str_04; break;
            case PlugIntError::ERR_GETFUNCTION: s = l_err_str_05; break;
            case PlugIntError::ERR_LUATYPE:     s = l_err_str_06; break;
            case PlugIntError::ERR_LUAPANIC:    s = l_err_str_07; break;
            case PlugIntError::ERR_LUAHANDLE:   s = l_err_str_11; break;
            case PlugIntError::ERR_PLUGINDATA:  s = l_err_str_12; break;
            case PlugIntError::ERR_PLUGTYPE:    s = l_err_str_13; break;
            case PlugIntError::ERR_LUASRCEMPTY: s = l_err_str_14; break;
            default: break;
        }
        if (!s.empty())
            LuaLint::print(s, LuaLint::ColorPrint::DebugError);
    }

    bool LuaPluginEditor::load_(std::string const & s, std::string const & p, luaL_Reg *tbl)
    {
        bool        ismute = false;
        uint32_t    ret    = 0U;
        int32_t     err    = LUA_OK;

        do
        {
            if (!luainit_(tbl))
                break_error(PlugIntError::ERR_INIT);

            if (!luacode_(s, p))
                break_error(PlugIntError::ERR_CODE);

            ismute = f_out_mute_(ismute);
            //
            if ((err = lua_pcall(m_lua.value(), 0, 0, 0)) != LUA_OK)
                break;

            if (l_ispanic)
                break_error(PlugIntError::ERR_LUAPANIC);

            if (::lua_getglobal(m_lua.value(), "plugin") != LUA_TTABLE)
                break_error(PlugIntError::ERR_GETMODULE);
        }
        while (0);

        if (ismute)
            (void) f_out_mute_(ismute);
        //
        if (err != LUA_OK)
            LuaObject::pcall_error_print_(m_lua, err);
        //
        if (ret)
            local_error_print_(ret);
        //
        if ((ret) || (err != LUA_OK))
        {
            if (m_lua)
                LuaObject::close_(m_lua);

            m_plug.reset();
            return false;
        }
        return true;
    }

    std::string LuaPluginEditor::getptitle_()
    {
        uint32_t    ret = 0U;
        std::string s;

        do
        {
            if (!m_lua)
                break;

            if (::lua_getfield(m_lua.value(), -1, "title") != LUA_TSTRING)
                break_error(PlugIntError::ERR_GETVALUE);

            if (!::lua_isstring(m_lua.value(), -1))
                break_error(PlugIntError::ERR_LUATYPE);

            s = ::lua_tostring(m_lua.value(), -1);
            ::lua_pop(m_lua.value(), 1);
        }
        while (0);

        if (ret)
            local_error_print_(ret);

        return s;
    }

    LuaPluginEditor::PluginType LuaPluginEditor::getptype_()
    {
        uint32_t ret = 0U;
        //
        do
        {
            if (!m_lua.has_value())
                break;

            if (::lua_getfield(m_lua.value(), -1, "type") != LUA_TNUMBER)
                break_error(PlugIntError::ERR_GETVALUE);

            if (!::lua_isnumber(m_lua.value(), -1))
                break_error(PlugIntError::ERR_LUATYPE);

            ret = static_cast<uint32_t>(::lua_tointeger(m_lua.value(), -1));
            ::lua_pop(m_lua.value(), 1);
            return static_cast<LuaPluginEditor::PluginType>(ret);
        }
        while (0);

        if (ret)
            local_error_print_(ret);

        return LuaPluginEditor::PluginType::PLUGT_NONE;
    }
};
