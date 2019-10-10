
#pragma once

#define __LUA_FUNC_TEMPLATE(ERRSTR_)                                                                            \
    static int32_t f_safeBindRuner_(lua_State *L, lua_CFunction fun)                                            \
    {                                                                                                           \
        int32_t ret = 0;                                                                                        \
        try { ret = fun(L); }                                                                                   \
        catch(std::exception const & ex_) { luaL_error(L, ex_.what()); }                                        \
        catch(...)                        { luaL_error(L, ERRSTR_); }                                           \
        return ret;                                                                                             \
    }                                                                                                           \
    template <lua_CFunction fun>                                                                                \
    int32_t f_callSafeBind_(lua_State *L) { return f_safeBindRuner_(L, fun); }

#define __LUA_FUNC_CLASS(funname)                                                                               \
    int32_t funname ();

#define __LUA_FUNC_TABLE(funname)                                                                               \
    { #funname, f_callSafeBind_<&f_bind_ ## funname> }

#define __LUA_FUNC_CODE_WRAP(funname)                                                                           \
    extern "C" int32_t f_bind_ ## funname (lua_State *L)                                                        \
    {                                                                                                           \
        LuaEngine *le = LuaEngine::instance(L);                                                                 \
        if (!le) { lua_pushnil(L); return 1; }                                                                  \
        return le->funname();                                                                                   \
    }

#define __LUA_FUNC_CODE_HEADER(funname)                                                                         \
    int32_t LuaEngine::funname ()

#if defined(_BUILD_LUA_EDITOR)
#  include "LuaMacroEditor.h"
#else
#  include "LuaMacroPlugin.h"
#endif
