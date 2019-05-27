
#include "../../ADBViewer.h"

#if defined(OS_WIN)
#  include "IPluginLoader.WINAPI.h"
#else
#  include "IPluginLoader.LINUX.h"
#endif

using namespace std::placeholders;

namespace Plugins
{

typedef Plugins::AppIPlugin * (*f_Fn_construct)(const char*, const void*);
typedef void (*f_Fn_destruct)(void);

static inline const char *l_nameCreatePlugin = "CreatePlugin";
static inline const char *l_nameDestroyPlugin = "DestroyPlugin";

AppPluginManager::AppPluginManager()
        : m_isrun(false)
    {
        m_loadfunc = std::make_unique<Plugins::PluginLoader>();
    }

AppPluginManager::~AppPluginManager()
    {
        m_isrun = false;

        if (m_thr.joinable())
            m_thr.join();

        m_isrun = true;
        freeplugins();
    }

AppPluginManager& AppPluginManager::instance()
    {
        static AppPluginManager m_instance{};
        return m_instance;
    }

void AppPluginManager::init()
    {
        if (m_plugins.size())
            return;

        m_isrun = true;

        std::thread thr
        {
            [=]()
            {
                m_loadfunc->PluginFind(
                        std::bind(&Plugins::AppPluginManager::addplugin, this, _1, _2)
                    );

                if (!m_isrun.load())
                    return;

                if (m_plugins.size() > 1U)
                {
                    std::sort(
                            m_plugins.begin(), m_plugins.end(),
                            [=](const auto& lplg, const auto& rplg)
                            {
                                return (lplg.iplug->priority() < rplg.iplug->priority());
                            }
                        );
                }
                m_isrun = false;
            }
        };
        m_thr = move(thr);
    }

void AppPluginManager::run(std::vector<uint8_t> &v, uint32_t w, uint32_t h)
    {
        if ((m_isrun.load()) || (!m_plugins.size()))
            return;

        if (m_thr.joinable())
            m_thr.join();

        bool isready = false;
        for (auto &plg : m_plugins)
        {
            if (!plg.iplug)
                continue;
            if ((isready = plg.iplug->ready()))
                break;
        }
        if (!isready)
            return;

        m_isrun = true;

        std::vector<uint8_t> vt(v);
        std::thread thr
        {
            [=]()
            {
                for (auto &plg : m_plugins)
                {
                    if (!m_isrun.load())
                        break;
                    if (!plg.iplug)
                        continue;
                    if (plg.iplug->ready())
                        plg.iplug->go(vt, w, h);
                }
                m_isrun = false;
            }
        };
        m_thr = move(thr);
    }

bool AppPluginManager::isplugin(std::string const & s)
    {
        auto plg = find_if(
                    m_plugins.begin(),
                    m_plugins.end(),
                    [=](Plugin_s pl)
                    {
                        return (pl.path.compare(s) == 0);
                    }
            );
        return (plg != m_plugins.end());
    }

void AppPluginManager::addplugin(std::string const & sp, std::string const & sn)
    {
        if (isplugin(sn))
            return;

        AppPluginManager::Plugin_s plg{};

        do
        {
            std::stringstream ss;
            ss << sp.c_str() << "\\" << sn.c_str();
            plg.path = sn.substr(0, sn.length() - 4);

            if (!(plg.handle = m_loadfunc->PluginOpen(ss.str().c_str())))
                break;

            f_Fn_construct func = (f_Fn_construct)
                m_loadfunc->PluginInstance(plg.handle, l_nameCreatePlugin);
            if (!func)
                break;

            if (!(plg.iplug = func(plg.path.c_str(), AppConfig::instance().GetAdbCb())))
                break;

            m_plugins.push_back(plg);
            return;
        }
        while (0);

        if (plg.iplug)
        {
            f_Fn_destruct func = (f_Fn_destruct)
                m_loadfunc->PluginInstance(plg.handle, l_nameDestroyPlugin);
            if (func)
                func();
        }
        if (plg.handle)
            m_loadfunc->PluginClose(plg.handle);
    }

void AppPluginManager::freeplugins() noexcept
    {
        if (!m_plugins.size())
            return;

        for (auto &plg : m_plugins)
        {
            if (plg.handle)
            {
                f_Fn_destruct func = (f_Fn_destruct)
                    m_loadfunc->PluginInstance(plg.handle, l_nameDestroyPlugin);
                if (func)
                    func();

                m_loadfunc->PluginClose(plg.handle);
                plg.handle = nullptr;
            }
            plg.iplug = nullptr;
        }
        m_plugins.clear();
    }


}
