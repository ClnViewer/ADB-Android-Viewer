
#pragma once

class LuaObject
{
    private:
        lua_State  *m_lua;
        int32_t     m_watch;
        double      m_laststate;
        std::string m_path;

        std::atomic<const Plugins::PluginCallBack_s*> m_adbcmd;
        std::vector<uint8_t> const *m_vfb;

        bool init();
        void close();
        void clean();
        void dump();

        bool getstack_(std::string const&);

    public:
        std::atomic<uint32_t> sleep;
        GameDev::ADBDriver::Tap_s point;


        LuaObject();
        LuaObject(std::string const&);
        ~LuaObject() noexcept;

        void adbset(Plugins::PluginCallBack_s const*);
        void fbset(std::vector<uint8_t> const*);

        bool    getfun(std::string const&);
        int32_t getstate() const;
        bool    open(std::string const&);
        bool    check();
        bool    load();
        lua_State * instance() const;
        static LuaObject * instance(lua_State*);

        /// Export to Lua script function

        int32_t checkPixelsByPos();   /// return boolean,
                                      /// format: { buffer position, color R, color G, color B }
        int32_t checkPixelsByCord();  /// return boolean,
                                      /// format: { vertical X, horizontal Y, color R, color G, color B }
        int32_t stateSet();
        int32_t stateGet();
        int32_t stateSleep();

        int32_t adbClick();
        int32_t adbSwipe();
        int32_t adbKey();
        int32_t adbText();

        int32_t screenGet();          /// return (w, h, vector FB)
        int32_t screenGetCord();      /// return (w, h)
        int32_t screenGetFb();        /// return (vector FB)

        int32_t checkTime();          /// return (0/1/-1), input (time string start, time string end)

        /// Get methods

        template<typename T>
        T get(std::string const& vname)
        {
            T result;
            if (getstack_(vname))
                result = get_<T>();
            else
                result = getdefault_<T>();

            clean();
            return result;
        }

        template <typename T>
        void setT(T items, std::string const & vname)
        {
            lua_getglobal(m_lua, vname.c_str());
            setT<T>(items);
        }

        template <typename T>
        void setT(T items)
        {
            lua_newtable(m_lua);
            int32_t i = 1;
            for (auto & item : items)
            {
                lua_pushinteger(m_lua, i++);
                lua_pushnumber(m_lua, item);
                lua_settable(m_lua, -3);
            }
        }

        template <typename T>
        std::vector<T> getT(std::string const & vname)
        {
            lua_getglobal(m_lua, vname.c_str());
            return getT<T>();
        }

        template <typename T>
        std::vector<T> getT()
        {
            std::vector<T> items;
            if (!lua_istable(m_lua, -1))
                return items;

            size_t sz = lua_rawlen(m_lua, -1);
            if (!sz)
                return items;

            items.reserve(sz);
            for (size_t i = 0U; i < sz; i++)
            {
                lua_pushinteger(m_lua, (i + 1));
                lua_gettable(m_lua, -2);
                if constexpr (std::is_same<T, std::string>::value)
                {
                    if (lua_isstring(m_lua, -1))
                        items.push_back(lua_tostring(m_lua, -1));
                }
                else if constexpr (std::is_same<T, bool>::value)
                {
                    if (lua_isboolean(m_lua, -1))
                        items.push_back(lua_toboolean(m_lua, -1));
                }
                else
                {
                    if (lua_isnumber(m_lua, -1))
                        items.push_back((T)lua_tonumber(m_lua, -1));
                }
                lua_pop(m_lua, 1);
            }
            return items;
        }

        template <typename T, size_t N>
        std::vector<std::array<T,N>> getT(std::string const & vname)
        {
            lua_getglobal(m_lua, vname.c_str());
            return getT<T,N>();
        }

        template <typename T, size_t N>
        std::vector<std::array<T,N>> getT()
        {
            std::vector<std::array<T,N>> items;
            if (!lua_istable(m_lua, -1))
                return items;

            size_t sz = lua_rawlen(m_lua, -1);
            if (!sz)
                return items;

            items.reserve(sz);
            for (size_t i = 0U; i < sz; i++)
            {
                lua_pushinteger(m_lua, (i + 1));
                lua_gettable(m_lua, -2);

                if (!lua_istable(m_lua, -1))
                {
                    items.clear();
                    return items;
                }

                std::array<T, N> a;

                for (size_t n = 1U; n <= N; n++)
                {
                    lua_pushinteger(m_lua, n);
                    lua_gettable(m_lua, -2);

                    if constexpr (std::is_same<T, std::string>::value)
                    {
                        if (lua_isstring(m_lua, -1))
                            a[(n - 1)] = lua_tostring(m_lua, -1);
                    }
                    else if constexpr (std::is_same<T, bool>::value)
                    {
                        if (lua_isboolean(m_lua, -1))
                            a[(n - 1)] = lua_toboolean(m_lua, -1);
                    }
                    else
                    {
                        if (lua_isnumber(m_lua, -1))
                            a[(n - 1)] = (T)lua_tonumber(m_lua, -1);
                    }
                    lua_pop(m_lua, 1);
                }
                items.push_back(a);
                lua_pop(m_lua, 1);
            }
            return items;
        }

    private:
        /// Internal generic LUA get

        template<typename T>
        T get_()
        {
            if constexpr
                (std::is_same<T, std::string>::value)
            {
                if(lua_isstring(m_lua, -1))
                    return std::string(lua_tostring(m_lua, -1));
                return std::string();
            }
            else if constexpr
                ((std::is_same<T, int32_t>::value) ||
                 (std::is_same<T, int16_t>::value) ||
                 (std::is_same<T, int8_t>::value))
            {
                if (lua_isnumber(m_lua, -1))
                    return (T) lua_tointeger(m_lua, -1);
                return 0;
            }
            else if constexpr
                ((std::is_same<T, uint32_t>::value) ||
                 (std::is_same<T, uint16_t>::value) ||
                 (std::is_same<T, uint8_t>::value))
            {
                if (lua_isnumber(m_lua, -1))
                    return (T) lua_tointeger(m_lua, -1);
                return 0U;
            }
            else if constexpr
                ((std::is_same<T, float>::value) ||
                 (std::is_same<T, double>::value))
            {
                if (lua_isnumber(m_lua, -1))
                    return (T) lua_tonumber(m_lua, -1);
                return 0.0;
            }
            else if constexpr
                (std::is_same<T, bool>::value)
            {
                if (lua_isboolean(m_lua, -1))
                    return (T) lua_toboolean(m_lua, -1);
                return false;
            }
            else
                return (T)0;
        }

        template<typename T>
        T getdefault_()
        {
            if constexpr
                (std::is_same<T, std::string>::value)
                return std::string();
            else if constexpr
                ((std::is_same<T, int32_t>::value) ||
                 (std::is_same<T, int16_t>::value) ||
                 (std::is_same<T, int8_t>::value))
                return 0;
            else if constexpr
                ((std::is_same<T, uint32_t>::value) ||
                 (std::is_same<T, uint16_t>::value) ||
                 (std::is_same<T, uint8_t>::value))
                return 0U;
            else if constexpr
                ((std::is_same<T, float>::value) ||
                 (std::is_same<T, double>::value))
                return 0.0;
            else if constexpr
                (std::is_same<T, bool>::value)
                return false;
            else
                return (T)0;
        }
};

