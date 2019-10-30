
#pragma once

#include <lua.hpp>
#include "LuaMacro.h"

namespace Plugins
{
    class LuaEngine
    {
        public:
            //
            LuaEngine();
            ~LuaEngine();
            //
            bool                       ready();
            bool                       iserror();
            bool                       getrunbreak();
            bool                       load(std::string const& = "");
            void                       close();
            //
            bool                       getfunction(std::string const&);
            //
            std::atomic<bool>          m_islog     = false;
            std::atomic<uint32_t>      m_sleep     = 1;
            std::atomic<int32_t>       m_laststate = 0;
            std::atomic<int32_t>       m_savecount = 0;
            std::string                m_imgdefault_name = "Android frame buffer";
            ImageLite::ImageRGBbuffer  m_imgdefault;
            //
            lua_State                 *instance() const;
            static LuaEngine          *instance(lua_State*);
            std::atomic<const Plugins::PluginCallBack_s*> m_adbcmd;
            //
            void setadb(Plugins::PluginCallBack_s const*);
            bool setfb(std::vector<uint8_t> const &, uint32_t, uint32_t);
            //
            __LUA_FUNC_CLASS(print)
            __LUA_FUNC_CLASS(dprint)
            //
#           define LFUN__(S,A,...) __LUA_FUNC_CLASS(A)
#           include "LuaPluginFunction.h"

        private:
            //
            lua_State                  *m_lua             = nullptr;
            struct luaL_Reg            *m_fun_object      = nullptr;
            struct luaL_Reg            *m_fun_redefine    = nullptr;
            int32_t                     m_fun_object_sz   = 0;
            int32_t                     m_fun_redefine_sz = 0;
            int32_t                     m_watch;
            std::string                 m_path;
            std::atomic<bool>           m_isrun           = false;
            //
            bool                        init_();
            void                        close_();
            void                        initbase_();
            //
            static void                 hook_cb(lua_State*, lua_Debug*);
    };
};
