
static Plugins::PluginCallBack_s const *adbcmd = nullptr;
static std::vector<uint8_t> const *adbbuffer = nullptr;

extern "C" int lua_Screen(lua_State* L)
{
    if (!adbbuffer)
    {
        lua_pushnil(L);
        return 1;
    }

#   if defined(_LUA_NEW_TABLE)
    lua_newtable(L);
#   else
    lua_createtable(L, adbbuffer->size(), 0);
    int32_t topidx = lua_gettop(L);
#   endif

    for (size_t i = 0; i < adbbuffer->size(); i++)
    {
        char t[] = { 0x0, '\0' };
        t[0] = (char)adbbuffer->begin()[i];

#       if defined(_LUA_NEW_TABLE)
        lua_pushnumber(L, i + 1);
        lua_pushstring(L, t);
        lua_settable(L, -3);
#       else
        lua_pushstring(L, t);
        lua_rawseti(L, topidx, i + 1);
#       endif
    }
    return 1;
}

extern "C" int lua_Click(lua_State* L)
{
    if (!adbcmd)
        return 0;

    GameDev::ADBDriver::Tap_s t;
    t.x = lua_tointeger(L, 1);
    t.y = lua_tointeger(L, 2);

    adbcmd->click(&t);
    return 0;
}

extern "C" int lua_Swipe(lua_State* L)
{
    if (!adbcmd)
        return 0;

    GameDev::ADBDriver::Swipe_s t{};
    t.x0 = lua_tointeger(L, 1);
    t.y0 = lua_tointeger(L, 2);
    t.x1 = lua_tointeger(L, 3);
    t.y1 = lua_tointeger(L, 4);
    t.t  = lua_tointeger(L, 5);

    adbcmd->swipe(&t);
    return 0;
}

extern "C" int lua_Key(lua_State* L)
{
    if (!adbcmd)
        return 0;

    int32_t k = lua_tointeger(L, 1);
    if (k > 0)
        adbcmd->key(GameDev::ADBDriver::KEYS_ANDROID, k);

    return 0;
}

extern "C" int lua_Text(lua_State* L)
{
    if (!adbcmd)
        return 0;

    if (lua_type(L, -1) == LUA_TSTRING)
    {
        const char *txt = lua_tostring(L, 1);
        if (txt)
            adbcmd->text(txt);
    }
    return 0;
}

