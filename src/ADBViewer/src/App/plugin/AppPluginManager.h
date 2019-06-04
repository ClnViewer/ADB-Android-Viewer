#pragma once
#include "AppIPlugin.h"
#include "IPluginLoader.h"

namespace Plugins
{

class AppPluginManager
{
public:
    //
    typedef struct Plugin_s
    {
        bool         state;
        std::string  path;
        std::string  name;
        const void  *handle;
        Plugins::AppIPlugin *iplug;
    } Plugin_s;

private:
    //
    std::thread             m_thr;
    std::atomic<bool>       m_isrun;
    std::mutex              m_lock;
    std::vector<AppPluginManager::Plugin_s> m_plugins;
    std::shared_ptr<Plugins::IPluginLoader> m_loadfunc;

    AppPluginManager();
    AppPluginManager(const AppPluginManager& apc) = delete;
    AppPluginManager& operator=(const AppPluginManager&) = delete;

    AppPluginManager::Plugin_s * findplugin(std::string const&);
    bool isplugin(std::string const &);
    void addplugin(std::string const&, std::string const&, bool);
    void freeplugins() noexcept;

public:
    //

    ~AppPluginManager();

    void init();
    void run(std::vector<uint8_t>&, uint32_t, uint32_t);
    bool enableplugin(AppPluginManager::Plugin_s&);
    bool enableplugin(std::string const&);
    void disableplugin(AppPluginManager::Plugin_s&);
    bool disableplugin(std::string const&);
    std::vector<AppPluginManager::Plugin_s> listplugin() const;

    static AppPluginManager& instance();
};

}
