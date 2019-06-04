
#pragma once

#if !defined(__NELE)
#  define __NELE(a) (sizeof(a) / sizeof(a[0]))
#endif

#define __LUA_FUNC_CLASS(funname)                \
    int32_t funname ();

#define __LUA_FUNC_BODY(funname)                 \
    int32_t LuaObject::funname ()

#define __LUA_FUNC_TABLE(funname)                \
    { .name = #funname, .fun = & l_ ## funname }

#define __LUA_FUNC_STATIC(funname)               \
    extern "C" int l_ ## funname (lua_State *L)  \
    {                                            \
        LuaObject *lo = LuaObject::instance(L);  \
        if (!lo) { lua_pushnil(L); return 1; }   \
        return lo->funname();                    \
    }

#define __RGB_CHECK(A,B) \
    ((A != B) && ((std::max(A,B) - 1) != std::min(A,B)))

#include "../plugin-lua-utils.h"

struct _RGB {
    uint8_t r, g, b;
};

typedef struct
{
    const char   *name;
    lua_CFunction fun;

} LuaFunReg_s;

__LUA_FUNC_STATIC(checkPixelsByPos)
__LUA_FUNC_STATIC(checkPixelsByCord)
__LUA_FUNC_STATIC(adbClick)
__LUA_FUNC_STATIC(adbSwipe)
__LUA_FUNC_STATIC(adbKey)
__LUA_FUNC_STATIC(adbText)
__LUA_FUNC_STATIC(stateSet)
__LUA_FUNC_STATIC(stateGet)
__LUA_FUNC_STATIC(stateSleep)
__LUA_FUNC_STATIC(screenGetCord)
__LUA_FUNC_STATIC(screenGetFb)
__LUA_FUNC_STATIC(screenGet)

static LuaFunReg_s l_func[] =
{
    __LUA_FUNC_TABLE(checkPixelsByPos),
    __LUA_FUNC_TABLE(checkPixelsByCord),
    __LUA_FUNC_TABLE(adbClick),
    __LUA_FUNC_TABLE(adbSwipe),
    __LUA_FUNC_TABLE(adbKey),
    __LUA_FUNC_TABLE(adbText),
    __LUA_FUNC_TABLE(stateSet),
    __LUA_FUNC_TABLE(stateGet),
    __LUA_FUNC_TABLE(stateSleep),
    __LUA_FUNC_TABLE(screenGetCord),
    __LUA_FUNC_TABLE(screenGetFb),
    __LUA_FUNC_TABLE(screenGet)
};


__LUA_FUNC_BODY(checkPixelsByPos)
    {
        bool ret = false;

        do
        {
            lua_pop(m_lua, 1);

            if ((!m_vfb) || (!m_vfb->size()))
                break;

            auto tbl = getT<int32_t, 4>();
            if (!tbl.size())
                break;

            ret = true;

            for (auto & arr : tbl)
            {
                if (
                    (std::get<0>(arr) < 0) ||
                    (static_cast<uint32_t>(std::get<0>(arr)) >= m_vfb->size())
                    )
                    continue;

                /*
                uint32_t p = static_cast<uint32_t>(std::get<0>(arr));
                uint8_t  r = static_cast<uint8_t>(std::get<1>(arr));
                uint8_t  g = static_cast<uint8_t>(std::get<2>(arr));
                uint8_t  b = static_cast<uint8_t>(std::get<3>(arr));
                */

                _RGB const *rgb = (_RGB*)&(*m_vfb)[arr[0]];
                if (
                    __RGB_CHECK(static_cast<uint8_t>(std::get<1>(arr)), rgb->r) ||
                    __RGB_CHECK(static_cast<uint8_t>(std::get<2>(arr)), rgb->g) ||
                    __RGB_CHECK(static_cast<uint8_t>(std::get<3>(arr)), rgb->b)
                   )
                {
                    ret = false;
                    break;
                }
            }
        }
        while (0);

        lua_pushboolean(m_lua, ret);
        return 1;
    }



__LUA_FUNC_BODY(checkPixelsByCord)
    {
        bool ret = false;

        do
        {
            lua_pop(m_lua, 1);

            if ((!m_vfb) || (!m_vfb->size()))
                break;

            auto tbl = getT<int32_t, 5>();
            if (!tbl.size())
                break;

            ret = true;

            for (auto & arr : tbl)
            {
                int32_t x = static_cast<int32_t>(std::get<0>(arr)),
                        y = static_cast<int32_t>(std::get<1>(arr)),
                        p = utils_getpos(x, y, point.x, point.y);

                if ((uint32_t)p >= m_vfb->size())
                    continue;

                _RGB const *rgb = (_RGB*)&(*m_vfb)[p];
                if (
                    __RGB_CHECK(static_cast<uint8_t>(std::get<2>(arr)), rgb->r) ||
                    __RGB_CHECK(static_cast<uint8_t>(std::get<3>(arr)), rgb->g) ||
                    __RGB_CHECK(static_cast<uint8_t>(std::get<4>(arr)), rgb->b)
                   )
                {
                    ret = false;
                    break;
                }
            }
        }
        while (0);

        lua_pushboolean(m_lua, ret);
        return 1;
    }

__LUA_FUNC_BODY(adbClick)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_adbcmd.load())
                break;

            if (
                (!lua_isnumber(m_lua, -2)) ||
                (!lua_isnumber(m_lua, -1))
                )
                break;

            GameDev::ADBDriver::Tap_s t = {
                static_cast<int32_t>(lua_tointeger(m_lua, -2)),
                static_cast<int32_t>(lua_tointeger(m_lua, -1))
                };

            m_adbcmd.load()->click(&t);
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(adbSwipe)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_adbcmd.load())
                break;

            if (
                (!lua_isnumber(m_lua, -5)) ||
                (!lua_isnumber(m_lua, -4)) ||
                (!lua_isnumber(m_lua, -3)) ||
                (!lua_isnumber(m_lua, -2)) ||
                (!lua_isnumber(m_lua, -1))
                )
                break;

            GameDev::ADBDriver::Swipe_s s = {
                static_cast<int32_t>(lua_tointeger(m_lua, -5)),
                static_cast<int32_t>(lua_tointeger(m_lua, -4)),
                static_cast<int32_t>(lua_tointeger(m_lua, -3)),
                static_cast<int32_t>(lua_tointeger(m_lua, -2)),
                static_cast<int32_t>(lua_tointeger(m_lua, -1))
                };

            m_adbcmd.load()->swipe(&s);
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(adbKey)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_adbcmd.load())
                break;

            if (!lua_isnumber(m_lua, -1))
                break;

            m_adbcmd.load()->key(
                GameDev::ADBDriver::KeysType::KEYS_ANDROID,
                static_cast<int32_t>(lua_tointeger(m_lua, -1))
            );
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(adbText)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_adbcmd.load())
                break;

            if (!lua_isstring (m_lua, -1))
                break;

            std::string txt;
            txt.assign(lua_tostring(m_lua, -1));
            if (txt.empty())
                break;

            m_adbcmd.load()->text(txt);
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(stateSet)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!lua_isnumber(m_lua, -1))
                break;

            m_laststate = static_cast<double>(lua_tonumber(m_lua, -1));
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(stateGet)
    {
        lua_pop(m_lua, 1);
        lua_pushinteger(m_lua, m_laststate);
        return 1;
    }

__LUA_FUNC_BODY(stateSleep)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!lua_isnumber(m_lua, -1))
                break;

            /// seconds value
            sleep = static_cast<uint32_t>(lua_tointeger(m_lua, -1));
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(screenGetCord)
    {
        lua_pop(m_lua, 1);
        lua_pushinteger(m_lua, point.x);
        lua_pushinteger(m_lua, point.y);
        return 2;
    }

__LUA_FUNC_BODY(screenGetFb)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_vfb)
                break;

            setT(*m_vfb);
            return 1;
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

__LUA_FUNC_BODY(screenGet)
    {
        do
        {
            lua_pop(m_lua, 1);

            if (!m_vfb)
                break;

            lua_pushinteger(m_lua, point.x);
            lua_pushinteger(m_lua, point.y);
            setT(*m_vfb);
            return 3;
        }
        while (0);

        lua_pushnil(m_lua);
        return 0;
    }

