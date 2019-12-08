
#include "../../plugin-base.h"
#include <sys/stat.h>
#include <lua.hpp>

#include "../ImageLite/ImageLite.h"
#include "LuaLint.h"
#include "LuaEngine.Plugin.h"
#include "LuaObject.h"

#define __TRACE_PRINT(A)                                                                                        \
    if (LuaLint::istrace()) LuaLint::print(A)

namespace Plugins
{
    //
    // Static
    //
    static inline const char g_luae_str_00[] = " -- LuaEngine() -> unknown exception!";
    static inline const char g_luae_str_01[] = " -- LuaEngine() -> load()";
    static inline const char g_luae_str_02[] = " -- LuaEngine() -> close()";
    static inline const char g_luae_str_10[] = " -- Lua hook, break detected..";
    static inline const char g_luae_str_11[] = " -- Lua panic!";
    //
    static std::atomic<bool> l_ispanic = false;
    static int32_t f_atpanic_(lua_State *L)
    {
        l_ispanic = true;
        __TRACE_PRINT(g_luae_str_11);
        __TRACE_PRINT(lua_tostring(L, -1));
        return 1;
    }
    //
    // Public
    //
    LuaEngine::LuaEngine()
    {
        // extern in LuaBind.cpp
        initbase_();
#       if defined(_DEBUG)
        LuaLint::settrace(true);
#       endif
    }

    LuaEngine::~LuaEngine()
    {
        close_();
    }

    lua_State * LuaEngine::instance() const
    {
        if (m_lua.has_value())
            return m_lua.value();
        return nullptr;
    }

    LuaEngine * LuaEngine::instance(lua_State *L)
    {
        return LuaObject::getclass<Plugins::LuaEngine>(L, "LuaObject");
    }

    void LuaEngine::setadb(Plugins::PluginCallBack_s const *adbcmd)
    {
        m_adbcmd = adbcmd;
    }

    bool LuaEngine::setfb(ImageLite::ImgBuffer const & vdata, uint32_t w, uint32_t h)
    {
        try
        {
            ImageLite::ImageRGBbuffer img(vdata, w, h, 0, ImageLite::BufferType::IBT_RGB_PAD);
            if (img.empty())
                return false;

            m_imgdefault = std::move(img);
            return true;
        }
        catch (...)
        {
            m_imgdefault = ImageLite::ImageRGBbuffer();
            return false;
        }
    }

    void LuaEngine::close()
    {
        close_();
    }

    bool LuaEngine::ready()
    {
        return m_lua.has_value();
    }

    bool LuaEngine::iserror()
    {
        return l_ispanic.load();
    }

    bool LuaEngine::getrunbreak()
    {
        if (l_ispanic.load())
            close_();

        return ((l_ispanic.load()) ? l_ispanic.load() : !(m_isrun.load())
            );
    }

    bool LuaEngine::getfunction(std::string const & s)
    {
        return LuaObject::getfunction_(m_lua, s);
    }

    bool LuaEngine::load(std::string const & s)
    {
        try
        {
            __TRACE_PRINT(g_luae_str_01);

            if (!s.empty())
            {
                m_watch = 0;
                m_path.assign(s);
            }

            m_isrun   = false;
            l_ispanic = false;

            do
            {
                struct _stat st{};

                if (m_path.empty())
                    break;
                if (::_stat(m_path.c_str(), &st) < 0)
                    break;
                if (m_watch == st.st_mtime)
                {
                    m_isrun = true;
                    return true;
                }

                m_watch = st.st_mtime;

                if (!init_())
                    break;

                int32_t err;
                lua_atpanic(m_lua.value(), &f_atpanic_);

                __TRACE_PRINT(m_path);

                if ((err = luaL_loadfile(m_lua.value(), m_path.c_str())) != LUA_OK)
                {
                    if (!LuaLint::istrace())
                        break;
                    LuaLint::print_pcall_error(err);
                    break;
                }

                ::lua_sethook(m_lua.value(), &LuaEngine::hook_cb, LUA_MASKCALL, 0);

                if ((err = ::lua_pcall(m_lua.value(), 0, 0, 0)) != LUA_OK)
                {
                    if (!LuaLint::istrace())
                        break;
                    LuaLint::print_pcall_error(err);
                    break;
                }
                m_isrun = true;
                return true;
            }
            while (0);

            if (m_lua.has_value())
            {
                __TRACE_PRINT(::lua_tostring(m_lua.value(), -1));
                close_();
            }
            m_path.assign("");
            return false;
        }
        catch(std::exception const & ex_) { __TRACE_PRINT(ex_.what()); }
        catch (...)                       { __TRACE_PRINT(g_luae_str_00); }
        return false;
    }

    bool LuaEngine::init_()
    {
        close_();

        if (!LuaObject::init_(
                m_lua,
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
        try
        {
            __TRACE_PRINT(g_luae_str_02);

            m_isrun   = false;
            l_ispanic = false;

            LuaObject::close_(m_lua);
            if (!m_imgdefault.empty())
                m_imgdefault = ImageLite::ImageRGBbuffer();
        }
        catch(std::exception const & ex_) { __TRACE_PRINT(ex_.what()); }
        catch (...)                       { __TRACE_PRINT(g_luae_str_00); }
    }

    void LuaEngine::hook_cb(lua_State *lua_, lua_Debug *ldb_)
    {
        try
        {
            LuaEngine *le = LuaEngine::instance(lua_);
            if (!le)
                return;

            switch (ldb_->event)
            {
                case LUA_HOOKLINE:
                {
                    if (le->getrunbreak())
                    {
                        luaL_error(lua_, g_luae_str_10);
                        break;
                    }
                }
                default:
                    break;
            }
            return;
        }
        catch(std::exception const & ex_) { __TRACE_PRINT(ex_.what()); }
        catch (...)                       { __TRACE_PRINT(g_luae_str_00); }
    }

};
