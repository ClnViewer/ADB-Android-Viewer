#pragma once

 #include "IPluginLoader.h"

namespace Plugins
{
	class PluginLoader : public IPluginLoader
	{
	public:
		const void * PluginOpen(std::string const & s) override
		{
		    return LoadLibraryA(s.c_str());
		}
		void PluginClose(const void *instance) override
		{
		    (void) FreeLibrary((HMODULE)instance);
		}
		void * PluginInstance(const void *instance, std::string const & s) override
		{
		    return (void*) GetProcAddress((HMODULE)instance, s.c_str());
		}
		bool PluginFind(
                std::function<void(std::string const&, std::string const&, bool)> fun) override
		{
		    static const char *l_pluginPath = "\\plugins";
		    static const char *l_pluginMask = "\\plugins\\*.dll";

		    HANDLE handle;
		    WIN32_FIND_DATA data{};
		    uint32_t cnt = 0U;

		    std::string fpath_d;
		    fpath_d.resize(MAX_PATH);

		    std::string fpath_a;
		    std::string fpath_s;

		    if (!::GetModuleFileName(NULL, fpath_d.data(), MAX_PATH))
                return false;

            size_t pos = fpath_d.find_last_of("/\\");
            if (pos != std::string::npos)
                fpath_d = fpath_d.substr(0, pos);
            else
                fpath_d = ".";

            fpath_s = fpath_d;
            fpath_s.append(l_pluginMask);
            fpath_a = fpath_d;
            fpath_a.append(l_pluginPath);

		    if (::_access(fpath_a.c_str(), F_OK) < 0)
                return false;

            if ((handle = ::FindFirstFile(fpath_s.c_str(), &data)) == INVALID_HANDLE_VALUE)
                return false;

            std::vector<std::string> & l_cnf = AppConfig::instance().GetFileConfig(
                                                AppConfig::instance().GetFileConfigId(
                                                    AppConfig::ConfigIdType::CNF_PLUGINS_ENABLE
                                                )
                                            );
            do
            {
                bool plug_enable = false;

                if (
                    (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
                    (data.cFileName[0] == '_')
                   )
                   continue;

                if (l_cnf.size())
                {
                    auto sname = find_if(
                            l_cnf.begin(),
                            l_cnf.end(),
                            [=](std::string const & s)
                            {
                                return (s.compare(0, s.length(), data.cFileName, 0, s.length()) == 0);
                            }
                        );
                     plug_enable = (sname != l_cnf.end());
                }

                fun(fpath_a, data.cFileName, plug_enable);
                cnt++;
            }
            while (::FindNextFile(handle, &data));
            return static_cast<bool>(cnt);
		}
	};
}
