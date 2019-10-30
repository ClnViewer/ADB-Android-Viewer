#pragma once

namespace Plugins
{
    struct PluginCallBack_s
    {
        std::function<void(GameDev::ADBDriver::Tap_s*)> click;
        std::function<void(GameDev::ADBDriver::Swipe_s*)> swipe;
        std::function<void(GameDev::DriverConst::KeysType, int32_t)> key;
        std::function<void(std::string const&)> text;
    };
}
