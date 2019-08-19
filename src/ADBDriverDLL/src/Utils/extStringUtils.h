
#pragma once

#if !defined(__STDSTRINGUTILS_H)
#define __STDSTRINGUTILS_H 1

namespace stdext
{

static inline bool endcheck(char c)
    {
        return ((c == '\r') || (c == '\n'));
    }
static inline bool endcheck(wchar_t c)
    {
        return ((c == L'\r') || (c == L'\n'));
    }

template<typename T>
static inline void string_clear_end(T & s)
    {
        s.erase(
            std::remove_if(
                s.begin(),
                s.end(),
                [](auto c)
                    {
                        return endcheck(c);
                    }
            ),
            s.end()
        );
    }

template<typename T>
static inline T string_filename(T const & s)
    {
        size_t pos;
        T fname;

        if constexpr (std::is_same<T, std::string>::value)
            pos = s.find_last_of("/\\");
        else if constexpr (std::is_same<T, std::wstring>::value)
            pos = s.find_last_of(L"/\\");

        if (pos == std::string::npos)
            fname = s.c_str();
        else
            fname = s.substr((pos + 1), (s.length() - 1)).c_str();
        return fname;
    }

template<typename T>
static inline void impl_trim__(T & s, T & c)
    {
        s.erase(0, s.find_first_not_of(c));
        s.erase(s.find_last_not_of(c)+1);
    }

template<typename T>
static inline void string_trim_endline(T & s)
    {
        T d0, d1, d2;

        if constexpr (std::is_same<T, std::wstring>::value)
        {
            d0 = L"\n"; d1 = L"\r"; d2 = L" ";
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            d0 = "\n"; d1 = "\r"; d2 = " ";
        }
        impl_trim__<T>(s, d0);
        impl_trim__<T>(s, d1);
        impl_trim__<T>(s, d2);
    }

template<typename T>
static inline void string_trim_space(T & s)
    {
        T d;

        if constexpr (std::is_same<T, std::wstring>::value)
            d = L" ";
        else if constexpr (std::is_same<T, std::string>::value)
            d = " ";

        impl_trim__<T>(s, d);
    }

template<typename T>
static inline T string_trim_space_r(T s)
    {
        string_trim_space<T>(s);
        return s;
    }

template<typename T>
static inline void string_trim_endline_r(T & s)
    {
        string_trim_endline<T>(s);
        return s;
    }

template<class T>
static inline bool string_is_end(T const & s, T const & d)
    {
        if (d.size() > s.size()) return false;
        return std::equal(d.rbegin(), d.rend(), s.rbegin());
    }

template<class T>
static inline bool string_is_begin(T const & s, T const & d)
    {
        if (d.size() > s.size()) return false;
        return std::equal(d.begin(), d.end(), s.begin());
    }

template<class T>
static inline typename T::size_type string_replace(T & s, const T & f, const T & r)
	{
	    typename T::size_type pos = 0U,
                              cnt = 0U;
	    for (
              ;
              ((pos = s.find(f, pos)) != T::npos);
              pos += r.length()
            )
        {
            s.replace(pos, f.length(), r);
            cnt++;
        }
        return cnt;
	}

static inline std::string string_to_utf8(std::wstring const & s)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cnv;
        return cnv.to_bytes(s.c_str());
    }

static inline std::string string_to_utf8_sub(std::wstring const & s, std::size_t b, std::size_t e)
    {
        return string_to_utf8(s.substr(b, e));
    }

static inline std::string string_to_utf8_sub(std::wstring const & s, std::size_t e)
    {
        return string_to_utf8(s.substr(0U, e));
    }

};

using stdext::string_filename;
using stdext::string_clear_end;
using stdext::string_replace;
using stdext::string_to_utf8;
using stdext::string_to_utf8_sub;

using stdext::string_trim_endline;
using stdext::string_trim_endline_r;
using stdext::string_trim_space;
using stdext::string_trim_space_r;

using stdext::string_is_end;
using stdext::string_is_begin;

#endif
