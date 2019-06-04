#pragma once

namespace Plugins
{
	class IPluginLoader
	{
	public:
		IPluginLoader() {};
		virtual ~IPluginLoader() = default;
		virtual const void * PluginOpen(std::string const&) = 0;
		virtual void PluginClose(const void*) = 0;
		virtual void * PluginInstance(const void*, std::string const&) = 0;
		virtual bool PluginFind(std::function<void(std::string const&, std::string const&, bool)>) = 0;
	};
}
