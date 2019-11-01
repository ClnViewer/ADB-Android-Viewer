
#pragma once

namespace Editor
{
    static inline const char l_err_str_000[] = " -- Lua editor plugin ";
    static inline const char l_err_str_001[] = " -> data found";
    static inline const char l_err_str_002[] = " -> data empty";
    //
    static inline const char l_err_str_01[] = " -- Lua editor plugin: Lua init internal error";
    static inline const char l_err_str_02[] = " -- Lua editor plugin: load Lua code error";
    static inline const char l_err_str_03[] = " -- Lua editor plugin: 'plugin' not module (table), or set as local variable";
    static inline const char l_err_str_04[] = " -- Lua editor plugin: 'plugin.name' not a string variable";
    static inline const char l_err_str_05[] = " -- Lua editor plugin: 'plugin.callplugin()' not function";
    static inline const char l_err_str_06[] = " -- Lua editor plugin: Lua return undefined type";
    static inline const char l_err_str_07[] = " -- Lua editor plugin: Lua return null";
    static inline const char l_err_str_08[] = " -- Lua editor plugin: Lua panic!";
    static inline const char l_err_str_09[] = " -- Lua editor plugin: file name empty";
    static inline const char l_err_str_10[] = " -- Lua editor plugin: file not found";
    static inline const char l_err_str_11[] = " -- Lua editor plugin: status Lua is closed..";
    static inline const char l_err_str_12[] = " -- Lua editor plugin: plugin data empty..";
    static inline const char l_err_str_13[] = " -- Lua editor plugin: plugin type not support";
    static inline const char l_err_str_14[] = " -- Lua editor plugin: Lua source script is empty..";
    //
    static inline const char l_err_str_20[] = " -- Error add Lua editor plugin: ";
    static inline const char l_uexception[] = " -- Lua editor plugin unknown exception";
    //
    enum PlugIntError
    {
        ERR_NONE = 0,
        ERR_INIT,
        ERR_CODE,
        ERR_GETMODULE,
        ERR_GETVALUE,
        ERR_GETFUNCTION,
        ERR_LUATYPE,
        ERR_LUANIL,
        ERR_LUAHANDLE,
        ERR_PLUGINDATA,
        ERR_LUAPANIC,
        ERR_LUASRCEMPTY,
        ERR_PLUGTYPE
    };
    //
};
