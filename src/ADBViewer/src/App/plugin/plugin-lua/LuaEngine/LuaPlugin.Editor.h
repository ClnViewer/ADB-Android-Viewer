
#pragma once

#include <lua.hpp>

namespace Editor
{
    class LuaPluginEditor
    {
        public:
            //
            enum PluginType
            {
                PLUGT_NONE = 0,
                PLUGT_RUNCODE,
                PLUGT_PARSECODE
            };
            //
            LuaPluginEditor();
            ~LuaPluginEditor();
            //
            bool                       init();
            bool                       open(uint32_t, std::string const&);
            void                       close();
            bool                       run();
            std::optional<uint32_t>    getid();
            std::optional<std::string> getname();
            std::optional<std::string> gettitle();
            std::optional<PluginType>  gettype();
            std::vector<MDIWin::BaseData::dataMap> getlist();
            //

        private:
            //
            std::optional<lua_State*>                     m_lua;
            std::optional<std::string>                    m_lua_source;
            std::optional<MDIWin::BaseData::dataMap>      m_plug;
            std::map<uint32_t, MDIWin::BaseData::dataMap> m_map;
            //
            bool                       luainit_(luaL_Reg*);
            bool                       luacode_(std::string const&, std::string const&);
            void                       local_error_print_(uint32_t);
            bool                       load_(std::string const&, std::string const&, luaL_Reg* = nullptr);
            //
            std::string                getptitle_();
            PluginType                 getptype_();
    };
};
