#ifndef __MAIN_PLUGIN_H__
#define __MAIN_PLUGIN_H__

#include "../plugin-base.h"
#include <lua.hpp>
#include "LuaObject/LuaObject.h"

#ifdef __cplusplus

namespace Plugins
{
	class PluginLua : public AppIPlugin
	{
    private:
        LuaObject   m_luaobj;
        std::string m_script;
        std::atomic<int32_t> m_wcount;

        void pathLuaScript();

	public:
		PluginLua(const char *, const void*);
		~PluginLua() noexcept;
		void go(std::vector<uint8_t> const &, uint32_t, uint32_t) override;
	};
}

#endif

#endif
