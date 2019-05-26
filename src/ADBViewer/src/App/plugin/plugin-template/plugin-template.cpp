
#include "plugin-template.h"
#include <chrono>
#include <thread>

namespace Plugins
{
    PluginTemplate::PluginTemplate(const void *vcb)
    {
        m_adbcmd = static_cast<Plugins::PluginCallBack_s const*>(vcb);
        m_priority = 1;
        m_isready = true;

        /// test part
        m_test_swap_click = false;
    }

    void PluginTemplate::go(std::vector<uint8_t> const & v, uint32_t w, uint32_t h)
    {
        m_isready = false;

        //...
        std::this_thread::sleep_for(std::chrono::seconds(10));

        GameDev::ADBDriver::Tap_s t{};

        if (!m_test_swap_click)
            t = { 14, 416 };
        else
            t = { 412, 416 };

        m_adbcmd->click(&t);
        m_test_swap_click = !m_test_swap_click;

        m_isready = true;
    }
}



