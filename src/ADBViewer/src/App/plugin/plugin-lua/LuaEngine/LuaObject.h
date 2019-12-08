
#pragma once

namespace LuaObject
{
        bool        getstack_(lua_State*, std::string const&);
        bool        getfunction_(std::optional<lua_State*>&, std::string const&);
        bool        getLuaImage(lua_State*, ImageLite::ImageData&);
        void        setLuaImage(lua_State*, ImageLite::ImageData const &);  // using setformatLuaImage instead
        bool        setformatLuaImage(lua_State*, ImageLite::ImageRGBbuffer&);
        bool        setformatLuaImage(lua_State*, ImageLite::ImageData&, ImageLite::BufferType, int32_t);
        int32_t     tablecount_(lua_State*);
        void        stackdump_(std::optional<lua_State*>&, std::function<void(std::string const&, int32_t)>);
        std::string trace_(lua_State*);
        std::string trace_(std::optional<lua_State*>&);
        void        clean_(std::optional<lua_State*>&);
        void        close_(std::optional<lua_State*>&);
        bool        init_(
                        std::optional<lua_State*>&,
                        std::string const& = "",
                        struct luaL_Reg* = nullptr,
                        struct luaL_Reg* = nullptr,
                        int32_t sz = 0,
                        void* = nullptr);
        void        pcall_error_print_(std::optional<lua_State*> const&, int32_t);
        //

        /// Internal generic LUA get/set/callback
        typedef void (*lobjprint_cb)(std::string const&);

        template <lobjprint_cb fun>
        int32_t trace(lua_State *L)
        {
            fun(LuaObject::trace_(L));
            return 0;
        }

        template<typename T>
        T get_(lua_State *L)
        {
            if constexpr
                (std::is_same<T, std::string>::value)
                 {
                     if (::lua_isstring(L, -1))
                        return std::string(lua_tostring(L, -1));
                     return std::string();
                 }
            else if constexpr
                (std::is_same<T, std::wstring>::value)
                 {
                     if (::lua_isstring(L, -1))
                     {
                         std::string s = lua_tostring(L, -1);
                         if (!s.empty())
                             return std::wstring(s.begin(), s.end());
                     }
                     return std::wstring();
                 }
            else if constexpr
                ((std::is_same<T, int32_t>::value)  ||
                 (std::is_same<T, int16_t>::value)  ||
                 (std::is_same<T, int8_t>::value)   ||
                 (std::is_same<T, uint32_t>::value) ||
                 (std::is_same<T, uint16_t>::value) ||
                 (std::is_same<T, uint8_t>::value))
                 {
                     if (::lua_isnumber(L, -1))
                        return (T) ::lua_tointeger(L, -1);
                     return (T)0;
                 }
            else if constexpr
                ((std::is_same<T, float>::value) ||
                 (std::is_same<T, double>::value))
                 {
                     if (::lua_isnumber(L, -1))
                        return (T) ::lua_tonumber(L, -1);
                     return (T)0.0;
                 }
            else if constexpr
                (std::is_same<T, bool>::value)
                 {
                     if (lua_isboolean(L, -1))
                        return (T) ::lua_toboolean(L, -1);
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
                (std::is_same<T, std::wstring>::value)
                return std::wstring();
            else if constexpr
                ((std::is_same<T, int32_t>::value)  ||
                 (std::is_same<T, int16_t>::value)  ||
                 (std::is_same<T, int8_t>::value)   ||
                 (std::is_same<T, uint32_t>::value) ||
                 (std::is_same<T, uint16_t>::value) ||
                 (std::is_same<T, uint8_t>::value))
                return 0;
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

        template<typename T>
        int32_t gettype_()
        {
            if constexpr
                ((std::is_same<T, std::string>::value) ||
                 (std::is_same<T, std::wstring>::value))
                return LUA_TSTRING;
            else if constexpr
                ((std::is_same<T, int32_t>::value)  ||
                 (std::is_same<T, int16_t>::value)  ||
                 (std::is_same<T, int8_t>::value)   ||
                 (std::is_same<T, uint32_t>::value) ||
                 (std::is_same<T, uint16_t>::value) ||
                 (std::is_same<T, uint8_t>::value)  ||
                 (std::is_same<T, float>::value)    ||
                 (std::is_same<T, double>::value))
                return LUA_TNUMBER;
            else if constexpr
                (std::is_same<T, bool>::value)
                return LUA_TBOOLEAN;
            else if constexpr
                (std::is_same<T, void*>::value)
                return LUA_TTABLE;
            else
                return LUA_TNONE;
        }

        /// Get/Set methods

        template <typename T>
        void setT(lua_State *L, T items, std::string const & vname)
        {
            ::lua_getglobal(L, vname.c_str());
            setT<T>(L, items);
        }

        template <typename T>
        void setT(lua_State *L, T items)
        {
            ::lua_newtable(L);
            int32_t i = 1;
            for (auto & item : items)
            {
                ::lua_pushinteger(L, i++);
                ::lua_pushnumber(L, item);
                ::lua_settable(L, -3);
            }
        }

        template <typename T>
        void setimagedataT(lua_State *L, T items)
        {
            ::lua_newtable(L);
            int32_t n = 3, i = 0;
            for (auto & item : items)
            {
                if (n == 3)
                {
                    if (i)
                        ::lua_settable(L, -3);
                    ::lua_pushinteger(L, ++i);
                    ::lua_newtable(L);
                    n = 0;
                }
                ::lua_pushinteger(L, ++n);
                ::lua_pushnumber(L, item);
                ::lua_settable(L, -3);
            }
            ::lua_settable(L, -3);
        }

        template <typename T>
        void setfieldT(lua_State *L, std::string const & s, T val = (T)INT_MIN)
        {
            ::lua_pushstring(L, s.c_str());
            if (val == (T)INT_MIN)
                return;
            ::lua_pushnumber(L, val);
            ::lua_settable(L, -3);
        }

        template<typename T>
        T get(lua_State *L, std::string const& vname)
        {
            T result;
            if (getstack_(L, vname))
                result = get_<T>();
            else
                result = getdefault_<T>();

            ::lua_pop(L, ::lua_gettop(L));
            return result;
        }

        template <typename T>
        std::vector<T> getT(lua_State *L, std::string const & vname)
        {
            ::lua_getglobal(L, vname.c_str());
            return getT<T>(L);
        }

        template <typename T>
        std::vector<T> getT(lua_State *L)
        {
            std::vector<T> items;
            if (!lua_istable(L, -1))
                return items;

            size_t sz = ::lua_rawlen(L, -1);
            if (!sz)
                return items;

            items.reserve(sz);
            for (size_t i = 0U; i < sz; i++)
            {
                ::lua_pushinteger(L, (i + 1));
                ::lua_gettable(L, -2);
                items.push_back((T)get_<T>(L));
                ::lua_pop(L, 1);
            }
            return items;
        }

        template <typename T, size_t N>
        std::vector<T> getT(lua_State *L, std::string const & vname)
        {
            ::lua_getglobal(L, vname.c_str());
            return getT<T,N>(L);
        }

        template <typename T, size_t N>
        std::vector<T> getT(lua_State *L)
        {
            std::vector<T> items;
            if (!lua_istable(L, -1))
                return items;

            size_t sz = ::lua_rawlen(L, -1);
            if (!sz)
                return items;

            items.reserve(sz * N);
            for (size_t i = 0U; i < sz; i++)
            {
                ::lua_pushinteger(L, (i + 1));
                ::lua_gettable(L, -2);

                if (!lua_istable(L, -1))
                {
                    items.clear();
                    return items;
                }

                for (size_t n = 1U; n <= N; n++)
                {
                    ::lua_pushinteger(L, n);
                    ::lua_gettable(L, -2);
                    items.push_back((T)get_<T>(L));
                    ::lua_pop(L, 1);
                }
                ::lua_pop(L, 1);
            }
            return items;
        }

        template <size_t N>
        bool getpixelsarray(lua_State *L, std::vector<int32_t> & vtbl)
        {
            try
            {
                vtbl = LuaObject::getT<int32_t, N>(L);
                if ((!vtbl.size()) ||
                    ((vtbl.size() % N) != 0))
                    return false;
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        template<typename T>
        bool getfield(lua_State *L, std::string const & s, T & obj, int32_t t = LUA_TNONE)
        {
            bool ret = false;
            do
            {
                t = ((t == LUA_TNONE) ? gettype_<T>() : t);
                if (::lua_getfield(L, 2, s.c_str()) != t)
                    break;

                switch (t)
                {
                    case LUA_TSTRING:
                    case LUA_TNUMBER:
                    case LUA_TBOOLEAN:
                        {
                            obj = get_<T>(L);
                            ret = true;
                            break;
                        }
                    case LUA_TTABLE:
                        return true; // using ::lua_pop(L, 1) in next code
                    default:
                        break;
                }
            }
            while (0);
            ::lua_pop(L, 1);
            return ret;
        }

        template<class T>
        T * getclass(lua_State *L, std::string const & s)
        {
            do
            {
                ::lua_getglobal(L, s.c_str());
                if (!lua_istable(L, -1))
                    break;

                ::lua_pushstring(L, "_self");
                ::lua_gettable(L, -2);

                if (!lua_isuserdata(L, -1))
                    break;

                T *t = reinterpret_cast<T*>(::lua_touserdata(L, -1));
                ::lua_pop(L, 1);
                return t;
            }
            while (0);
            return nullptr;
        }

        template<class T>
        void setclass(lua_State *L, std::string const & s, T *val)
        {
            ::lua_newtable(L);
            ::lua_pushlightuserdata(L, reinterpret_cast<void*>(val));
            ::lua_setfield(L, -2, "_self");
            ::lua_setglobal(L, s.c_str());
            ::lua_pop(L, 1);
        }
};
