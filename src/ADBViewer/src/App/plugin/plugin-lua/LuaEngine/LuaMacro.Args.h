
/// INPUT ARGUMENTS NUMBER

#define __LUA_FUNC_ARGS_INPUT_NONE()     0
#define __LUA_FUNC_ARGS_INPUT_STRING()   1
#define __LUA_FUNC_ARGS_INPUT_STRING_1() 1
#define __LUA_FUNC_ARGS_INPUT_STRING_2() 2
#define __LUA_FUNC_ARGS_INPUT_STRING_3() 3
#define __LUA_FUNC_ARGS_INPUT_STRING_4() 4
#define __LUA_FUNC_ARGS_INPUT_STRING_5() 5
#define __LUA_FUNC_ARGS_INPUT_NUMBER()   1
#define __LUA_FUNC_ARGS_INPUT_NUMBER_1() 1
#define __LUA_FUNC_ARGS_INPUT_NUMBER_2() 2
#define __LUA_FUNC_ARGS_INPUT_NUMBER_3() 3
#define __LUA_FUNC_ARGS_INPUT_NUMBER_4() 4
#define __LUA_FUNC_ARGS_INPUT_NUMBER_5() 5
#define __LUA_FUNC_ARGS_INPUT_STRING_1_NUMBER_1() 2
#define __LUA_FUNC_ARGS_INPUT_STRING_1_NUMBER_4() 5
#define __LUA_FUNC_ARGS_INPUT_TABLE() 1
#define __LUA_FUNC_ARGS_INPUT_NUMBER_4_OR_NUMBER_1_TABLE()                                                      \
    ((lua_istable(m_lua.value(), -1)) ? 2 : ((lua_isnumber(m_lua.value(), -1)) ? 4 : 0))
#define __LUA_FUNC_ARGS_INPUT_NUMBER_4_OR_TABLE()                                                               \
    ((lua_istable(m_lua.value(), -1)) ? 1 : ((lua_isnumber(m_lua.value(), -1)) ? 4 : 0))
#define __LUA_FUNC_ARGS_INPUT_NUMBER_5_OR_NUMBER_2_TABLE()                                                      \
    ((lua_istable(m_lua.value(), -1)) ? 3 : ((lua_isnumber(m_lua.value(), -1)) ? 5 : 0))
