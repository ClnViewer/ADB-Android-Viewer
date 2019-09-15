
#pragma once

#include <lua.hpp>
#include <LuaPluginMacro.h>

namespace Editor
{
    class LuaEngine
    {
        public:
            //
            LuaEngine();
            ~LuaEngine();
            //
            bool                    ready();
            bool                    stop();
            bool                    check(std::string const&);
            bool                    start(std::string const&);
            //
            bool                    getdbgbreak() const;
            void                    setdbgbreak(bool);
            bool                    getdbgstep() const;
            void                    setdbgstep(bool);
            bool                    getdbgcontinue() const;
            void                    setdbgcontinue(bool);
            bool                    getdbgdump() const;
            void                    setdbgdump(bool);
            bool                    getdbgview() const;
            void                    setdbgview(bool);
            bool                    stepnextline();
            //
            lua_State              *instance() const;
            static LuaEngine       *instance(lua_State*);
            //
            __LUA_FUNC_CLASS(print)
            __LUA_FUNC_CLASS(dprint)
            //
#           define LFUN__(S,A,...) __LUA_FUNC_CLASS(A)
#           include <LuaPluginFunction.h>

            void                    close_();

        private:
            //
            lua_State              *m_lua             = nullptr;
            struct luaL_Reg        *m_fun_object      = nullptr;
            struct luaL_Reg        *m_fun_redefine    = nullptr;
            int32_t                 m_fun_object_sz   = 0;
            int32_t                 m_fun_redefine_sz = 0;
            std::atomic<bool>       m_isdbgbreak      = true;
            std::atomic<bool>       m_isdbgstep       = false;
            std::atomic<bool>       m_isdbgcontinue   = false;
            std::atomic<bool>       m_isdbgdump       = false;
            std::atomic<bool>       m_isdbgview       = false;
            std::atomic<int32_t>    m_laststate       = 0;
            std::thread             m_task;
            //
            void                    initbase_();
            bool                    init_();
            void                    clean_();
            //void                    close_();
            bool                    open_(std::string const&);
            //
            bool                    getfun_(std::string const&);
            //
            static void             hook_cb(lua_State*, lua_Debug*);
            //
            void                    countprint_(int32_t, std::string const&);
            void                    runscriptend_();
            //
            bool                    runer_(std::string const&, std::function<bool(std::string const&)>);
            bool                    runchek_(bool);
            bool                    check_(std::string const&);
            bool                    start_(std::string const&);
    };
};

#define LocalPrintError(A)    TRACEBoxEx(A, RGB(186,0,0))
#define DebugPrintError(A)    TRACEBox(A, RGB(192,0,0))
#define DebugPrintOk(A)       TRACEBox(A, RGB(0,134,0))
#define DebugPrintHelp(A)     COLORBox(A,   0,  64,   0)

//#define DebugPrintTraceC(A)   COLORBox(A,  35,  35, 230)
//#define DebugPrintTraceLua(A) COLORBox(A,  32,  32,  32)

#define DebugPrintTraceC(A)   TRACEBox(A, RGB(35,35,230))
#define DebugPrintTraceLua(A) TRACEBox(A, RGB(32,32,32))
