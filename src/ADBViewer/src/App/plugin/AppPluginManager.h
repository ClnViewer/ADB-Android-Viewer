#pragma once
#include "AppIPlugin.h"
#include "IPluginLoader.h"

namespace Plugins
{

class AppPluginManager
{
private:
    //
    typedef struct
    {
        std::string  path;
        const void  *handle;
        Plugins::AppIPlugin *iplug;
    } Plugin_s;

    std::thread             m_thr;
    std::atomic<bool>       m_isrun;
    std::vector<Plugin_s>   m_plugins;
    std::shared_ptr<Plugins::IPluginLoader> m_loadfunc;

    AppPluginManager();
    AppPluginManager(const AppConfig& apc) = delete;
    AppPluginManager& operator=(const AppConfig&) = delete;

    bool isplugin(std::string const &);
    void freeplugins() noexcept;
    void addplugin(std::string const&, std::string const&);

public:
    //

    ~AppPluginManager();

    void init();
    void run(std::vector<uint8_t>&, uint32_t, uint32_t);

    static AppPluginManager& instance();
};

}
