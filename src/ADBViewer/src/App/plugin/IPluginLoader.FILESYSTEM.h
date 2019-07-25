#pragma once

#include "IPluginLoader.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Plugins
{
	class PluginLoader : public IPluginLoader
	{
	public:
		const void * PluginOpen(std::string const & s) override
		{
#           if defined(OS_WIN)
		    return ::LoadLibraryA(s.c_str());
#           else
		    return ::dlopen(s.c_str(), RTLD_NOW | RTLD_LAZY);
#           endif
		}
		void PluginClose(const void *instance) override
		{
#           if defined(OS_WIN)
		    (void) ::FreeLibrary((HMODULE)instance);
#           else
		    (void) ::dlclose((void*)instance);
#           endif
		}
		void * PluginInstance(const void *instance, std::string const & s) override
		{
#           if defined(OS_WIN)
		    return (void*) ::GetProcAddress((HMODULE)instance, s.c_str());
#           else
		    return (void*) ::dlsym((void*)instance, s.c_str());
#           endif
		}
		bool PluginFind(
                std::function<void(std::string const&, std::string const&, bool)> fun) override
		{
		    try
		    {
                fs::path l_root = AppConfig::instance().cnf_root_path.c_str();
                std::string rs = l_root.generic_string();
                l_root /= "plugins";

                do
                {
                    if ((!fs::exists(l_root)) || (!fs::is_directory(l_root)))
                        break;

                    uint32_t cnt = 0U;
                    fs::directory_iterator sfiles(l_root);
                    std::vector<fs::path> vfiles;

                    std::copy_if(begin(sfiles), end(sfiles), std::back_inserter(vfiles),
                        [](const fs::path & p)
                            {
                                return ((fs::is_regular_file(p)) && (p.extension() == ".dll"));
                            }
                    );
                    if (!vfiles.size())
                        break;

                    std::vector<std::string> & l_cnf = AppConfig::instance().GetFileConfig(
                                                AppConfig::instance().GetFileConfigId(
                                                    AppConfig::ConfigIdType::CNF_PLUGINS_ENABLE
                                                )
                                            );

                    for (auto & p : vfiles)
                    {
                        bool plug_enable = false;

                        if (l_cnf.size())
                        {
                            auto sname = find_if(
                                    l_cnf.begin(),
                                    l_cnf.end(),
                                    [=](std::string const & s)
                                    {
                                        return (s.compare(
                                                    0,
                                                    s.length(),
                                                    p.generic_string().c_str(),
                                                    0,
                                                    s.length()
                                                ) == 0);
                                    }
                                );
                            plug_enable = (sname != l_cnf.end());
                        }
                        fun(
                            l_root.generic_string(),
                            p.filename().generic_string(),
                            plug_enable
                        );
                        cnt++;
                    }
                    return static_cast<bool>(cnt);
                }
                while (0);

                return false;
		    }
		    catch (const fs::filesystem_error &)
		    {
                return false;
            }
            catch (const std::exception &)
            {
                return false;
            }
            catch (...)
            {
                return false;
            }
		}
	};
}
