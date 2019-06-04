#pragma once

 #include "IPluginLoader.h"

namespace Plugins
{
	class PluginLoader : public IPluginLoader
	{
	public:
		const void * PluginOpen(std::string const & s) override
		{
		    return dlopen(s.c_str(), RTLD_NOW | RTLD_LAZY);
		}
		void PluginClose(const void *instance) override
		{
		    (void) dlclose((void*)instance);
		}
		void * PluginInstance(const void *instance, std::string const & s) override
		{
		    return (void*) dlsym((void*)instance, s.c_str());
		}
		bool PluginFind(
                std::function<void(std::string const&, std::string const&)> fun) override
		{
		    static const char *l_pluginPath = "/plugins/";
#           ifdef __APPLE__
		    static const char *l_pluginMask = ".dylib";
#           else
		    static const char *l_pluginMask = ".so";
#           endif

		    DIR *dir;
		    struct dirent *ent;
		    uint32_t cnt = 0U;

		    std::string fpath_d;
		    fpath_d.resize(1024);

		    if (!::getcwd(fpath_d.data(), 1024))
                return false;

            size_t pos = fpath_d.find_last_of("/");
            if (pos != std::string::npos)
                fpath_d =  fpath_d.substr(0, pos);
            else
                fpath_d = ".";

            fpath_d += l_pluginPath;

		    if (::access(fpath_d.c_str(), F_OK) < 0)
                return false;

            if (!(dir = ::opendir(fpath_d.c_str())))
                return false;

            std::vector<std::string> & l_cnf = AppConfig::instance().GetFileConfig("plugins-enable");

            while ((ent = ::readdir(dir)))
            {
                bool plug_enable = false;
                std::string file_l(ent->d_name);
                pos = file_l.find_last_of(".");

                if (pos != std::string::npos)
                    if (file_l.substr(0, pos).compare(l_pluginMask) != 0)
                        continue;

                if (l_cnf.size())
                {
                    auto sname = find_if(
                            l_cnf.begin(),
                            l_cnf.end(),
                            [=](std::string const & s)
                            {
                                return (s.compare(0, s.length(), ent->d_name, 0, s.length()) == 0);
                            }
                        );
                     plug_enable = (sname != l_cnf.end());
                }

                fun(fpath_d.c_str(), ent->d_name, plug_enable);
                cnt++;
            }
            ::closedir(dir);
            return static_cast<bool>(cnt);
		}
	};
}
