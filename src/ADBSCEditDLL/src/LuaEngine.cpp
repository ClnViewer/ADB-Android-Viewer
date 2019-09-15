
#include "SCEditInternal.h"
#include "LuaEngine.String.h"
#include <iomanip>
#include <ctime>
#include <lua.hpp>

namespace Editor
{
    //
    // Public
    //

    LuaEngine::LuaEngine()
    {
        initbase_();
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
        lua_getglobal(L, "LuaObject");
        if (!lua_istable(L, -1))
            return nullptr;

        lua_pushstring(L, "_self");
        lua_gettable(L, -2);

        if (!lua_isuserdata(L, -1))
            return nullptr;

        LuaEngine *p = (LuaEngine*)lua_touserdata(L, -1);
        lua_pop(L, 1);
        return p;
    }

    bool LuaEngine::getdbgbreak() const
    {
        return m_isdbgbreak.load();
    }

    void LuaEngine::setdbgbreak(bool b)
    {
        m_isdbgbreak = b;
        if (b)
            DebugPrintOk(g_debug_str_0);
    }

    bool LuaEngine::getdbgstep() const
    {
        return m_isdbgstep.load();
    }

    void LuaEngine::setdbgstep(bool b)
    {
        if (!getdbgbreak())
        {
            DebugPrintError(g_debug_str_22);
            return;
        }
        m_isdbgstep = b;
        if (b)
            DebugPrintOk(g_debug_str_1);
        else
            DebugPrintOk(g_debug_str_2);
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
        return runchek_(false);
    }

    //
    // Private
    //

    bool LuaEngine::init_()
    {
        close_();

        if (!(m_lua = luaL_newstate()))
            return false;

        luaL_checkversion(m_lua);
        luaL_openlibs(m_lua);
        {
            lua_getglobal(m_lua, "_G");
            luaL_setfuncs(m_lua, m_fun_redefine, 0);
            lua_pop(m_lua, 1);
        }
        //
        {
            lua_createtable(m_lua, 0, m_fun_object_sz + 1);
            //
            lua_pushlightuserdata(m_lua, this);
            lua_setfield(m_lua, -2, "_self");
            //
            luaL_setfuncs(m_lua, m_fun_object, 0);
            //
            lua_setglobal(m_lua, "LuaObject");
            lua_pop(m_lua, 1);
        }
        return true;
    }

    void LuaEngine::clean_()
    {
        if (m_lua)
            lua_pop(m_lua, lua_gettop(m_lua));
    }

    void LuaEngine::close_()
    {
        if (m_lua)
            lua_close(m_lua);
        m_lua = nullptr;
    }

    static int f_atpanic_(lua_State*)
    {
        DebugPrintError("Lua panic!");
        return 1;
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
            if (lua_pcall(m_lua, 0, 0, 0))
                break;

            DebugPrintOk(g_debug_str_3);
            return true;
        }
        while (0);

        if (m_lua)
        {
            DebugPrintError(lua_tostring(m_lua, -1));
            close_();
        }
        else
            DebugPrintError(g_debug_str_4);

        return false;
    }

    void LuaEngine::countprint_(int32_t cnt, std::string const & s)
    {
        std::stringstream ss;
        ss << "[ " << cnt << " ] - " << s.c_str();
        DebugPrintTraceC(ss.str());
    }

    void LuaEngine::runscriptend_()
    {
        SendMessageA(
            Config::instance().gethandle(),
            WM_COMMAND,
            (WPARAM)IDM_BTN_SCRUN_STOP,
            0
        );

        if (!getdbgstep())
            return;

        EditBox::AnnotateData ad{};
        SendMessageA(
            Config::instance().gethandle(),
            WM_COMMAND,
            (WPARAM)IDM_EDIT_SHOW_ANNOTATION,
            reinterpret_cast<LPARAM>(&ad)
        );
        SendMessageA(
            Config::instance().gethandle(),
            WM_COMMAND,
            (WPARAM)IDM_EDIT_SHOW_DBGLINE,
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
                LocalPrintError(g_debug_str_5);
                return false;
            }
            if (m_task.joinable())
            {
                m_task.join();
                DebugPrintOk(g_debug_str_6);
            }
            if (m_lua)
            {
                DebugPrintOk(g_debug_str_7);
                close_();
            }
            return true;
        }
        else
        {
            if (m_isdbgbreak)
            {
                DebugPrintError(g_debug_str_8);
                return false;
            }
            if (m_task.joinable())
            {
                runscriptend_();
                m_isdbgbreak = true;
                m_isdbgstep  = false;
                m_isdbgdump  = false;
                DebugPrintOk(g_debug_str_9);
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

        setdbgbreak(false);
        DebugPrintHelp(g_debug_help);
        SendMessageA(
            Config::instance().gethandle(),
            WM_COMMAND,
            (WPARAM)IDM_BTN_SCRUN_START,
            0
        );

        std::thread thr
        {
            [=]()
            {
                int32_t cnt = 0;
                m_laststate = 0;

                DebugPrintOk(g_debug_str_10);
                lua_sethook(m_lua, &LuaEngine::hook_cb, LUA_MASKCALL | LUA_MASKLINE, 0);

                do
                {
                    if (getdbgstep())
                        countprint_(++cnt, g_debug_str_11);
                    else
                        countprint_(++cnt, g_debug_str_12);
                    //
                    if (!getfun_("main"))
                    {
                        countprint_(cnt, g_debug_str_13);
                        break;
                    }
                    if (getdbgbreak())
                        break;
                    // push main() arguments
                    lua_pushinteger(m_lua, cnt);
                    lua_pcall(m_lua, 1, 1, 0);
                    //
                    if (getdbgbreak())
                        break;
                    // pull state from main()
                    if (!lua_isnil(m_lua, -1))
                    {
                        if (lua_isnumber(m_lua, -1))
                            m_laststate = lua_tonumber(m_lua, -1);
                        lua_pop(m_lua, 1);

                        std::stringstream ss;
                        ss << g_debug_str_16 << m_laststate.load();
                        DebugPrintTraceC(ss.str());
                    }
                    //
                    if (getdbgbreak())
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
                            if (getdbgbreak())
                            {
                                countprint_(cnt, g_debug_str_17);
                                break;
                            }
                            std::this_thread::yield();
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                    }
                }
                while (!getdbgbreak());

                countprint_(cnt, g_debug_str_18);
                runscriptend_();
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

            if (!getfun_("main"))
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

    //
    // LUA utilities method
    //

    bool LuaEngine::getfun_(std::string const & s)
    {
        if ((s.empty()) || (!ready()))
            return false;

        lua_getglobal(m_lua, s.c_str());
        return lua_isfunction(m_lua, -1);
    }

    //
    // static callback
    //

    static void f_dump_(lua_State *lua_, EditBox::AnnotateData *ad_, bool isdbgview)
    {
        lua_Debug ldb{};
        int32_t i, n, l = 0;
        std::stringstream ss;

        while (lua_getstack(lua_, l++, &ldb) == 1)
        {
            if (!lua_getinfo(lua_, "nlSf", &ldb))
                continue;

            n = lua_gettop(lua_);
            lua_pop(lua_, 1);
            if (isdbgview)
                ss << "line: " << ad_->line << ", " << "stack: " << n << ", ";
            else
                ss << "* stack available: " << n << "\n";

            for (i = 0; i < n; i++)
            {
                int32_t cnt = 0;

                do
                {
                    const char *str, *val;

                    if (!(str = lua_getlocal(lua_, &ldb, i)))
                        break;

                    cnt++;

                    if (::memcmp(str, static_cast<const void*>(&"(*temporary)"), 12) == 0)
                        break;

                    if (!isdbgview)
                        ss << "   " << i << "] - ";
                    ss << "(" << lua_typename(lua_, lua_type(lua_, i)) << ") ";
                    ss << str << " = [";

                    if (!(val = luaL_tolstring(lua_, -1, nullptr)))
                        ss << "?";
                    else
                    {
                        ss << val;
                        cnt++;
                    }
                    if (isdbgview)
                        ss << "], ";
                    else
                        ss << "]\n";
                }
                while (0);

                if (cnt)
                    lua_pop(lua_, cnt);
            }
        }

        ad_->text = ss.str();

        if ((isdbgview) && (!ad_->text.empty()))
        {
            std::size_t pos = ad_->text.find_last_of(",");
            if (pos != std::string::npos)
                ad_->text.erase(pos, (ad_->text.length() - 1));

            ::OutputDebugStringA(ad_->text.c_str());
        }
        else
            SendMessageA(
                Config::instance().gethandle(),
                WM_COMMAND,
                (WPARAM)IDM_EDIT_SHOW_ANNOTATION,
                reinterpret_cast<LPARAM>(ad_)
            );
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
                    if (le->getdbgbreak())
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

                    EditBox::AnnotateData ad{};
                    ad.line = (ldb_->currentline - 1);

                    SendMessageA(
                        Config::instance().gethandle(),
                        WM_COMMAND,
                        (WPARAM)IDM_EDIT_SHOW_DBGLINE,
                        (LPARAM)ad.line
                    );
                    SendMessageA(
                        Config::instance().gethandle(),
                        WM_COMMAND,
                        (WPARAM)IDM_EDIT_SHOW_ANNOTATION,
                        reinterpret_cast<LPARAM>(&ad)
                    );

                    if (le->getdbgview())
                        f_dump_(lua_, &ad, true);

                    while (!le->getdbgcontinue())
                    {
                        if (le->getdbgbreak())
                            break;
                        if (le->getdbgdump())
                        {
                            f_dump_(lua_, &ad, false);
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

                    if ((le->getdbgview()) && (ldb_->linedefined > 0))
                    {
                        EditBox::AnnotateData ad{};
                        ad.line = (ldb_->linedefined + 1);
                        f_dump_(lua_, &ad, true);
                    }

                    std::stringstream ss;
                    ss << "\tcall ";
                    if (ldb_->what)
                        ss << ldb_->what << ":";
                    if (ldb_->namewhat)
                        ss << ldb_->namewhat << ":";
                    ss << " -> " << ldb_->name;
                    if (ldb_->linedefined > 0)
                        ss << ":" << (ldb_->linedefined + 1);
                    COLORREFBox(ss.str(), RGB(128,0,128));
                    break;
                }
                default:
                    break;
            }
            return;
        }
        catch(std::exception const & ex_)
        {
            DebugPrintError(ex_.what());
        }
        catch (...)
        {
            DebugPrintError(g_debug_str_21);
        }
    }
};
