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

#include <SCEditInternal.h>
#include <lua.hpp>
#include <ctime>

#include "LuaLint.h"
#include "LuaObject.h"
#include "LuaEngine.Editor.String.h"

namespace Editor
{
    enum LuaDebugDataOut
    {
        DDT_ANOTATION,
        DDT_DBGVIEW,
        DDT_LISTBOX,
        DDT_TRACELINE,
        DDT_TRACEPRN
    };

#   include "LuaEngine.Editor.DebugData.cxx"

    //
    // Public
    //
    LuaEngine::LuaEngine()
    {
        initbase_();
        m_imgdraw.init(
            MDIWin::Config::instance().gethinstance(),
            CONF_MAIN_HANDLE()
        );
        m_imgdefault_name = "scedit-default.png";
        LuaLint::settrace(true);
    }

    LuaEngine::~LuaEngine()
    {
        m_isdbgbreak = true;
        if (m_task.joinable())
            m_task.join();
        close_();
    }

    lua_State * LuaEngine::instance() const
    {
        return m_lua;
    }

    LuaEngine * LuaEngine::instance(lua_State *L)
    {
        return LuaObject::getclass<Editor::LuaEngine>(L, "LuaObject");
    }

    bool LuaEngine::getrunbreak() const
    {
        return m_isdbgbreak.load();
    }

    void LuaEngine::setrunbreak(bool b)
    {
        m_isdbgbreak = b;
        if (b)
            LuaLint::print(g_debug_str_0, LuaLint::ColorPrint::DebugOk);
    }

    bool LuaEngine::getdbgstep() const
    {
        return m_isdbgstep.load();
    }

    void LuaEngine::setdbgstep(bool b)
    {
        if (!getrunbreak())
        {
            LuaLint::print(g_debug_str_22, LuaLint::ColorPrint::DebugError);
            return;
        }
        m_isdbgstep = b;
        if (b)
            LuaLint::print(g_debug_str_1, LuaLint::ColorPrint::DebugOk);
        else
            LuaLint::print(g_debug_str_2, LuaLint::ColorPrint::DebugOk);
    }

    bool LuaEngine::getdbgcontinue() const
    {
        return m_isdbgcontinue.load();
    }

    void LuaEngine::setdbgcontinue(bool b)
    {
        m_isdbgcontinue = b;
    }

    bool LuaEngine::getdbgdump() const
    {
        return m_isdbgdump.load();
    }

    void LuaEngine::setdbgdump(bool b)
    {
        m_isdbgdump = b;
    }

    bool LuaEngine::getdbgview() const
    {
        return m_isdbgview.load();
    }

    void LuaEngine::setdbgview(bool b)
    {
        m_isdbgview = b;
    }

    bool LuaEngine::stepnextline()
    {
        if (getdbgstep())
        {
            setdbgcontinue(true);
            return true;
        }
        return false;
    }


    bool LuaEngine::ready()
    {
        return (m_lua);
    }

    bool LuaEngine::check(std::string const & s)
    {
        CLEARBox();
        return runer_(s,
                      [=](std::string const & str)
                      {
                          return check_(str);
                      }
            );
    }

    bool LuaEngine::start(std::string const & s)
    {
        CLEARBox();
        return runer_(s,
                      [=](std::string const & str)
                      {
                          return start_(str);
                      }
            );
    }

    bool LuaEngine::stop()
    {
        if (m_imgdraw.isactive())
            m_imgdraw.close();

        return runchek_(false);
    }

    bool  LuaEngine::loadimagedefault()
    {
        std::string s;
        return loadimagedefault(s);
    }

    bool  LuaEngine::loadimagedefault(std::string & s)
    {
        try
        {
            if (!m_imgdefault.empty())
                return true;

            m_imgdefault.load(
                    ((s.empty()) ? m_imgdefault_name : s),
                    ImageLite::ImageType::IT_PNG
                );
            if (!s.empty())
                m_imgdefault_name = s;
            s = "";
            return true;
        }
        catch (std::exception const & ex_)
        {
            s = ex_.what();
            return false;
        }
        catch (...)
        {
            s.assign(__FUNCTION__);
            return false;
        }
    }

    //
    // Private
    //
    static std::atomic<bool> l_ispanic = false;
    static int32_t f_atpanic_(lua_State *L)
    {
        LuaLint::print(g_debug_str_23, LuaLint::ColorPrint::DebugError);
        LuaLint::print(lua_tostring(L, -1), LuaLint::ColorPrint::DebugError);
        l_ispanic = true;
        return 1;
    }

    void LuaEngine::pcall_error_(int32_t err, int32_t cnt)
    {
        LuaLint::print_pcall_error(err);
        LuaLint::print(lua_tostring(m_lua, -1), LuaLint::ColorPrint::DebugError);
        //
        std::string stb = LuaObject::trace_(m_lua);
        if (!stb.empty())
        {
            if (cnt)
                countprint_(cnt, stb);
            else
                LuaLint::print(stb, LuaLint::ColorPrint::DebugTraceStack);
        }
    }

    void LuaEngine::countprint_(int32_t cnt, std::string const & s)
    {
        std::stringstream ss;
        ss << "[ " << cnt << " ] - " << s.c_str();
        LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceC);
    }

    bool LuaEngine::init_()
    {
        close_();

        if (!LuaObject::init_(
                &m_lua,
                "LuaObject",
                m_fun_redefine,
                m_fun_object,
                (m_fun_object_sz + 1),
                static_cast<void*>(this)
            ))
            return false;
        return true;
    }

    void LuaEngine::close_()
    {
        LuaObject::close_(&m_lua);
    }

    bool LuaEngine::open_(std::string const & s)
    {
        do
        {
            if (!init_())
                break;

            lua_atpanic(m_lua, &f_atpanic_);

            if (luaL_loadstring(m_lua, s.c_str()))
                break;

            lua_sethook(m_lua, &LuaEngine::hook_cb, LUA_MASKCALL, 0);

            int32_t err;
            if ((err = lua_pcall(m_lua, 0, 0, 0)) != LUA_OK)
            {
                pcall_error_(err);
                break;
            }
            LuaLint::print(g_debug_str_3, LuaLint::ColorPrint::DebugOk);
            return true;
        }
        while (0);

        if (m_lua)
        {
            LuaLint::print(lua_tostring(m_lua, -1), LuaLint::ColorPrint::DebugError);
            close_();
        }
        else
            LuaLint::print(g_debug_str_4, LuaLint::ColorPrint::DebugError);

        return false;
    }

    void LuaEngine::runscriptend_()
    {
        SendMessageA(
            CONF_MAIN_HANDLE(),
            WM_COMMAND,
            (WPARAM)IDM_EVENT_SCRUN_STOP,
            0
        );

        if (!getdbgstep())
            return;

        EditBox::AnnotateData ad{};
        SendMessageA(
            CONF_MAIN_HANDLE(),
            WM_COMMAND,
            (WPARAM)IDM_EVENT_EDIT_ANNOTATION,
            reinterpret_cast<LPARAM>(&ad)
        );
        SendMessageA(
            CONF_MAIN_HANDLE(),
            WM_COMMAND,
            (WPARAM)IDM_EVENT_EDIT_DBGLINE,
            (LPARAM)-1
        );
    }

    bool LuaEngine::runer_(std::string const & s, std::function<bool(std::string const&)> fun)
    {
        try
        {
            return fun(s);
        }
        catch (std::exception const & ex_)
        {
            LocalPrintError(ex_.what());
            return false;
        }
        catch (...)
        {
            LocalPrintError(g_scedit_error);
            return false;
        }
    }

    bool LuaEngine::runchek_(bool b)
    {
        if (b)
        {
            if (!m_isdbgbreak)
            {
                if (l_ispanic)
                {
                    runchek_(false);
                    LuaLint::print(g_debug_str_24, LuaLint::ColorPrint::LocalError);
                }
                else
                {
                    LuaLint::print(g_debug_str_5, LuaLint::ColorPrint::LocalError);
                    return false;
                }
            }
            if (m_task.joinable())
            {
                m_task.join();
                LuaLint::print(g_debug_str_6, LuaLint::ColorPrint::DebugOk);
            }
            if (m_lua)
            {
                LuaLint::print(g_debug_str_7, LuaLint::ColorPrint::DebugOk);
                close_();
            }
            l_ispanic = false;
            return true;
        }
        else
        {
            if (m_isdbgbreak)
            {
                LuaLint::print(g_debug_str_8, LuaLint::ColorPrint::DebugError);
                return false;
            }
            if (m_task.joinable())
            {
                runscriptend_();
                l_ispanic    = false;
                m_isdbgbreak = true;
                m_isdbgstep  = false;
                m_isdbgdump  = false;
                m_savecount  = 0;
                LuaLint::print(g_debug_str_9, LuaLint::ColorPrint::DebugOk);
            }
            return true;
        }
    }

    bool LuaEngine::start_(std::string const & s)
    {
        if (!runchek_(true))
            return false;

        if (!m_lua)
            if (!check_(s))
                return false;

        setrunbreak(false);
        LuaLint::print(g_debug_help, LuaLint::ColorPrint::DebugHelp);
        SendMessageA(
            CONF_MAIN_HANDLE(),
            WM_COMMAND,
            (WPARAM)IDM_EVENT_SCRUN_START,
            0
        );

        std::thread thr
        {
            [=]()
            {
                int32_t cnt = 0,
                        ret = 0;
                m_laststate = 0;

                LuaLint::print(g_debug_str_10, LuaLint::ColorPrint::DebugOk);
                lua_sethook(m_lua, &LuaEngine::hook_cb, LUA_MASKCALL | LUA_MASKLINE, 0);
                lua_pushcfunction(m_lua, &LuaObject::trace<&LuaLint::print>);

                do
                {
                    if (getdbgstep())
                        countprint_(++cnt, g_debug_str_11);
                    else
                        countprint_(++cnt, g_debug_str_12);
                    //
                    if (!LuaObject::getfunction_(m_lua, "main"))
                    {
                        countprint_(cnt, g_debug_str_13);
                        break;
                    }
                    if (getrunbreak())
                        break;

                    // push main() arguments
                    ::lua_pushinteger(m_lua, ret);

                    int32_t err;
                    if ((err = lua_pcall(m_lua, 1, 1, 0)) != LUA_OK)
                    {
                        if ((!getrunbreak())  ||
                            (err != LUA_ERRRUN))
                            pcall_error_(err, cnt);
                        break;
                    }
                    //
                    if (getrunbreak())
                        break;
                    // pull state from main()
                    if (!lua_isnil(m_lua, -1))
                    {
                        if (lua_isnumber(m_lua, -1))
                            ret = ::lua_tointeger(m_lua, -1);
                        ::lua_pop(m_lua, 1);

                        std::stringstream ss;
                        ss << g_debug_str_16 << m_laststate.load();
                        LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceC);
                    }
                    else
                        ret = 0;
                    //
                    if (getrunbreak())
                        break;
                    if (getdbgstep())
                    {
                        countprint_(cnt, g_debug_str_14);
                        continue;
                    }
                    // thread sleep 5 sec
                    {
                        auto s_start = std::chrono::high_resolution_clock::now();
                        auto s_end = (s_start + std::chrono::seconds(5));

                        countprint_(cnt, g_debug_str_15);

                        while (std::chrono::high_resolution_clock::now() < s_end)
                        {
                            if (getrunbreak())
                            {
                                countprint_(cnt, g_debug_str_17);
                                break;
                            }
                            std::this_thread::yield();
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                    }
                }
                while (!getrunbreak());

                countprint_(cnt, g_debug_str_18);
                runscriptend_();
                setrunbreak(true);
                return;
            }
        };
        m_task = move(thr);
        return true;
    }

    bool LuaEngine::check_(std::string const & s)
    {
        if (!runchek_(true))
            return false;
        do
        {
            if (s.empty())
            {
                LocalPrintError(g_debug_str_19);
                break;
            }

            if (!open_(s))
                break;

            if (!LuaObject::getfunction_(m_lua, "main"))
            {
                LocalPrintError(g_debug_str_13);
                break;
            }
            return true;
        }
        while (0);

        close_();
        return false;
    }

    void LuaEngine::hook_cb(lua_State *lua_, lua_Debug *ldb_)
    {
        LuaEngine *le = LuaEngine::instance(lua_);
        if (!le)
            return;

        try
        {
            switch (ldb_->event)
            {
                case LUA_HOOKLINE:
                {
                    if (le->getrunbreak())
                    {
                        luaL_error(lua_, g_debug_str_17);
                        break;
                    }
                    if (!le->getdbgstep())
                        break;
                    if (!lua_getinfo(lua_, "l", ldb_))
                        break;
                    if (ldb_->currentline < 0)
                        break;

                    DebugData dd{};
                    dd.nline = (ldb_->currentline - 1);
                    //
                    f_DebugData_set_(lua_, dd);
                    f_DebugData_send_(dd, LuaDebugDataOut::DDT_TRACELINE);
                    f_DebugData_send_(dd, LuaDebugDataOut::DDT_LISTBOX);
                    //
                    if (le->getdbgview())
                        f_DebugData_send_(dd, LuaDebugDataOut::DDT_DBGVIEW);

                    while (!le->getdbgcontinue())
                    {
                        if (le->getrunbreak())
                            break;
                        if (le->getdbgdump())
                        {
                            f_DebugData_send_(dd, LuaDebugDataOut::DDT_ANOTATION);
                            le->setdbgdump(false);
                        }
                        std::this_thread::yield();
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    le->setdbgcontinue(false);
                    break;
                }
                case LUA_HOOKCALL:
                {
                    if (!lua_getinfo(lua_, "Snl", ldb_))
                        break;
                    if (!ldb_->name)
                        break;

                    DebugData dd{};
                    dd.nline = (ldb_->linedefined + 1);
                    //
                    if ((le->getdbgview()) && (ldb_->linedefined > 0))
                    {
                        f_DebugData_set_(lua_, dd);
                        f_DebugData_send_(dd, LuaDebugDataOut::DDT_DBGVIEW);
                    }
                    f_DebugData_send_(dd, LuaDebugDataOut::DDT_TRACEPRN, ldb_);
                    break;
                }
                default:
                    break;
            }
            return;
        }
        catch(std::exception const & ex_)
        {
            LuaLint::print(ex_.what(), LuaLint::ColorPrint::DebugError);
        }
        catch (...)
        {
            LuaLint::print(g_debug_str_21, LuaLint::ColorPrint::DebugError);
        }
    }
};
