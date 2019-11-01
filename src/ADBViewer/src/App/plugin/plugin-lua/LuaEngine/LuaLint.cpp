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

#if defined(_BUILD_LUA_EDITOR)
#  include <SCEditInternal.h>
#  define __LINT_PRINT(...)                                                                                     \
    if (l_istrace) MDIWin::Config::instance().colorBox(__VA_ARGS__)
#else
#include "../../plugin-base.h"
#  define __LINT_PRINT(A,...)                                                                                  \
    if (l_istrace) ::OutputDebugStringA(static_cast<LPCSTR>((A).c_str()));
#endif

#include <string>
#include <sstream>
#include <atomic>
#include <vector>
#include <lua.hpp>
#include "../ImageLite/ImageLite.h"
#include "LuaObject.h"
#include "LuaLint.h"

namespace LuaLint
{
    static const char l_lint_str_LUA_ERRRUN[]  = " -- Lua library runtime error";
    static const char l_lint_str_LUA_ERRMEM[]  = " -- Lua library memory allocation error";
    static const char l_lint_str_LUA_ERRERR[]  = " -- Lua library error while running the message handler";
    static const char l_lint_str_LUA_ERRGCMM[] = " -- Lua library error while running a __gc metamethod";
    static std::atomic<bool> l_istrace         = false;

#   if defined(_BUILD_LUA_EDITOR)
    static COLORREF l_color[] =
    {
        RGB(186,0,0),   // LocalError
        RGB(192,0,0),   // DebugError
        RGB(0,134,0),   // DebugOk
        RGB(0,64,0),    // DebugHelp
        RGB(35,35,230), // DebugTraceC
        RGB(32,32,32),  // DebugTraceLua
        RGB(128,0,128), // DebugTraceStack
        RGB(32,72,72),  // LuaFunctionPrint
        RGB(73,3,3)     // LuaFunctionDprint
    };
#   endif

    //
    bool istrace()
    {
        return l_istrace.load();
    }

    void settrace(bool b)
    {
        l_istrace = b;
    }

    // weak functions:
    void print(std::string const & s)
    {
        __LINT_PRINT(s, l_color[LuaLint::ColorPrint::DebugTraceLua]);
    }
    void print(std::stringstream & ss)
    {
        __LINT_PRINT(ss.str(), l_color[LuaLint::ColorPrint::DebugTraceLua]);
    }
    void print(std::string const & s, COLORREF const & color)
    {
        __LINT_PRINT(s, color);
    }
    void print(std::stringstream & ss, COLORREF const & color)
    {
        __LINT_PRINT(ss.str(), color);
    }
    void print(std::string const & s, LuaLint::ColorPrint idx)
    {
        __LINT_PRINT(s, l_color[idx]);
    }
    void print(std::stringstream & ss, LuaLint::ColorPrint idx)
    {
        __LINT_PRINT(ss.str(), l_color[idx]);
    }
    //
    void print_lua(lua_State *lua_, COLORREF const & color)
    {
        std::stringstream ss;
        int32_t i, n = ::lua_gettop(lua_);
        for (i = 1; i <= n; i++)
        {
            ss << ::luaL_tolstring(lua_, i, nullptr);
            ::lua_pop(lua_, 1);
        }
        if (!ss.str().empty())
            print(ss.str(), color);
    }
    //
    void stream_print_error(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\t\terror, argument: " << n << " is not a " << s.c_str() << "!\n";
    }
    void stream_print_caller(std::stringstream & ss, std::string const & s, std::string const & f, int32_t n)
    {
        size_t pos = f.find_last_of("/\\");
        ss << "\n\tcaller -> " << s.c_str() << " -> [";
        ss << ((pos == std::string::npos) ? f.c_str() : f.substr(pos + 1, (f.length() - pos - 1)).c_str());
        ss << ":" << n << "]\n";
    }
    void stream_print_called(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\tcalled -> LuaObject:" << s.c_str() << "(" << n << " arguments)\n";
    }
    void stream_print_return(std::stringstream & ss, std::string const & s, int32_t n)
    {
        ss << "\treturn -> LuaObject:" << s.c_str() << "() -> push arguments number: " << n << "\n";
    }
    void stream_print_arg_warn(std::stringstream & ss, std::string const & s, int32_t n, int32_t nn)
    {
        ss << "\t\terror, LuaObject function:" << s.c_str() << "() arguments found: " << n << ", needed: " << nn << "!\n";
    }
    void stream_print_ipix3(std::stringstream & ss, ImageLite::IPIX3 const & pix)
    {
        ss << "{ R:" << (int)pix.c1 << ", G:" << (int)pix.c2 << ", B:" << (int)pix.c3 << " }";
    }
    //
    void print_ipix3(ImageLite::IPIX3 const & pix)
    {
        std::stringstream ss;
        LuaLint::stream_print_ipix3(ss, pix);
        LuaLint::print_traceT(LuaLint::g_lint_str_36, ss.str());
    }
    void print_ipix3(ImageLite::IPIX3 const & pix1, ImageLite::IPIX3 const & pix2)
    {
        std::stringstream ss;
        LuaLint::stream_print_ipix3(ss, pix1);
        ss << " -> ";
        LuaLint::stream_print_ipix3(ss, pix2);
        LuaLint::print_traceT(LuaLint::g_lint_str_36, ss.str());
    }
    void print_LuaImage(ImageLite::ImageData & data)
    {
        print_LuaImage(data.buffer, data.point);
    }
    void print_LuaImage(ImageLite::ImgBuffer & ib, ImageLite::IPOINT<int32_t> const & ip)
    {
        uint32_t sz = static_cast<uint32_t>(ip.y * (ip.x * 3));
        std::stringstream ss;
        ss << "width/height: ";
        LuaLint::stream_print_ipointT(ss, ip);
        ss << ", table size: " <<  ib.size();
        ss << ", estimated size: " << sz << " bytes";
        LuaLint::print_traceT(LuaLint::g_lint_str_33, ss.str());
        if (ib.size() != sz)
            LuaLint::print_traceT(
                        LuaLint::g_lint_str_35,
                        (std::max(ib.size(), sz) - std::min(ib.size(), sz))
                    );
    }
    void print_LuaImage_data(ImageLite::ImageData & data)
    {
        print_LuaImage_data(data.buffer);
    }
    void print_LuaImage_data(ImageLite::ImgBuffer & ib)
    {
        if (!ib.size())
        {
            LuaLint::print_traceT(LuaLint::g_lint_str_34, 0);
            return;
        }
        for (uint32_t i = 0; i < ib.size(); i += 3)
        {
            std::string s("\t\t- ");
            s.append(std::to_string(i / 3));
            s.append(": ");
            uint32_t rgb = RGB(
                          (ib[(i + 0)]),
                          (ib[(i + 1)]),
                          (ib[(i + 2)])
                    );
            LuaLint::print_traceT(s, rgb);
        }
    }
    void print_pcall_error(int32_t err)
    {
        std::string s_error;
        switch (err)
        {
            case LUA_ERRRUN:  s_error = LuaLint::l_lint_str_LUA_ERRRUN;  break;
            case LUA_ERRMEM:  s_error = LuaLint::l_lint_str_LUA_ERRMEM;  break;
            case LUA_ERRERR:  s_error = LuaLint::l_lint_str_LUA_ERRERR;  break;
            case LUA_ERRGCMM: s_error = LuaLint::l_lint_str_LUA_ERRGCMM; break;
            default: return;
        }
        LuaLint::print(s_error, LuaLint::ColorPrint::LocalError);
    }

};
