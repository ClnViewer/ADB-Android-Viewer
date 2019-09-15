
#include "SCEditInternal.h"
#include "LuaEngine.String.h"
#include <lua.hpp>


namespace Editor
{
    __LUA_FUNC_TEMPLATE(g_debug_str_20)
    //
    __LUA_FUNC_CODE_WRAP(print)
    __LUA_FUNC_CODE_WRAP(dprint)
    //
#   define LFUN__(S,A,...) __LUA_FUNC_CODE_WRAP(A)
#   include <LuaPluginFunction.h>

    static struct luaL_Reg l_func_LuaObject[] =
    {
#       define LFUN__(S,A,...) __LUA_FUNC_TABLE(A),
#       include <LuaPluginFunction.h>
        { nullptr, nullptr }
    };

    static struct luaL_Reg l_func_GlobRedefine[] =
    {
        __LUA_FUNC_TABLE(print),
        __LUA_FUNC_TABLE(dprint),
        { nullptr, nullptr }
    };

    void LuaEngine::initbase_()
    {
        m_fun_object      = l_func_LuaObject;
        m_fun_redefine    = l_func_GlobRedefine;
        m_fun_object_sz   = (__NELE(l_func_LuaObject) - 1);
        m_fun_redefine_sz = (__NELE(l_func_GlobRedefine) - 1);
    }

    static void f_print_color(lua_State *lua_, COLORREF const & color)
    {
        std::stringstream ss;
        int32_t i, n = lua_gettop(lua_);
        for (i = 1; i < n; i++)
        {
            ss << luaL_tolstring(lua_, i, nullptr);
            lua_pop(lua_, 1);
        }
        COLORREFBox(ss.str(), color);
    }

    __LUA_FUNC_CODE_HEADER(print)
    {
        f_print_color(m_lua, RGB(32,32,32));
        return 0;
    }
    __LUA_FUNC_CODE_HEADER(dprint)
    {
        f_print_color(m_lua, RGB(128,0,128));
        return 0;
    }

    template <typename T>
    static void f_fun_print_ok(std::stringstream & ss, T val, int32_t n)
    {
        ss << "\t\targument: " << n << " = '" << (T)val << "'\n";
    }
    static void f_fun_print_error(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\t\terror, argument: " << n << " is not a " << s.c_str() << "!\n";
    }
    static void f_fun_print_caller(std::stringstream & ss, std::string const & s, std::string const & f, int32_t n)
    {
        size_t pos = f.find_last_of("/\\");
        ss << "\n\tcaller -> " << s.c_str() << " -> [";
        ss << ((pos == std::string::npos) ? f.c_str() : f.substr(pos + 1, (f.length() - pos - 1)).c_str());
        ss << ":" << n << "]\n";
    }
    static void f_fun_print_called(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\tcalled -> LuaObject:" << s.c_str() << "(" << n << " arguments)\n";
    }
    static void f_fun_print_return(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\treturn -> LuaObject:" << s.c_str() << "() -> push arguments number: " << n << "\n";
    }
    static void f_fun_print_argwarn(std::stringstream & ss, std::string const & s, int32_t n, int32_t nn)
    {
        ss << "\t\terror, function LuaObject:" << s.c_str() << "() arguments found: " << n << ", needed: " << nn << "!\n";
    }

#   define LFUN__(S,A,B,I,O) __LUA_FUNC_CODE_BODY(A,I,O,B)
#   include <LuaPluginFunction.h>

};
