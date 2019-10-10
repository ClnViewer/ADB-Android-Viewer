
#include <SCEditInternal.h>
#include <lua.hpp>
#include <iomanip>
#include <ctime>

#include "LuaLintEditor.h"
#include "LuaObject.h"
#include "LuaEngineEditorString.h"

namespace Editor
{
    //
    // Public
    //

    LuaEngine::LuaEngine()
    {
        initbase_();
        m_imgdraw.init(
            Config::instance().gethinstance(),
            Config::instance().gethandle()
        );
        m_imgdefault_name = "scedit-default.png";
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
        /*
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
        */
    }

    bool LuaEngine::getdbgbreak() const
    {
        return m_isdbgbreak.load();
    }

    void LuaEngine::setdbgbreak(bool b)
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
        if (!getdbgbreak())
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

    static int32_t f_atpanic_(lua_State*)
    {
        LuaLint::print(g_debug_str_23, LuaLint::ColorPrint::DebugError);
        l_ispanic = true;
        return 1;
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
                LuaLint::print_pcall_error(err);
                std::string stb = LuaObject::trace_(m_lua);
                if (!stb.empty())
                    LuaLint::print(stb, LuaLint::ColorPrint::DebugTraceStack);
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

    void LuaEngine::countprint_(int32_t cnt, std::string const & s)
    {
        std::stringstream ss;
        ss << "[ " << cnt << " ] - " << s.c_str();
        LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceC);
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

        setdbgbreak(false);
        LuaLint::print(g_debug_help, LuaLint::ColorPrint::DebugHelp);
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
                    if (getdbgbreak())
                        break;

                    // push main() arguments
                    lua_pushinteger(m_lua, cnt);

                    int32_t err;
                    if ((err = lua_pcall(m_lua, 1, 1, 0)) != LUA_OK)
                    {
                        if ((!getdbgbreak())  ||
                            (err != LUA_ERRRUN))
                        {
                            LuaLint::print_pcall_error(err);
                            std::string stb = LuaObject::trace_(m_lua);
                            if (!stb.empty())
                                countprint_(cnt, stb);
                        }
                        break;
                    }
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
                        LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceC);
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
                setdbgbreak(true);
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
                    LuaLint::print(ss.str(), LuaLint::ColorPrint::DebugTraceStack);
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
