#pragma once

namespace Plugins
{
    typedef struct
    {
        std::function<void(GameDev::ADBDriver::Tap_s*)> click;
        std::function<void(GameDev::ADBDriver::Swipe_s*)> swipe;
        std::function<void(GameDev::ADBDriver::KeysType,int32_t)> key;
        std::function<void(std::string const&)> text;
    } PluginCallBack_s;
}
