#pragma once
#include "AppIPluginCallBack.h"

namespace Plugins
{

class AppIPlugin
{
protected:
    //
    std::atomic<bool> m_isready;
    std::atomic<int32_t> m_priority;
    Plugins::PluginCallBack_s const *m_adbcmd;
    std::string m_name;

public:
    //

    AppIPlugin()
        : m_isready(false), m_priority(0), m_adbcmd(nullptr)
    {
    }

    virtual ~AppIPlugin() = default;

    virtual bool    ready() { return m_isready; }
    virtual int32_t priority() { return m_priority; }
    virtual void    go(std::vector<uint8_t> const &, uint32_t, uint32_t) {};
    std::string     getname() const { return m_name; }
};

}
