#pragma once

#if !defined(__STDSTRINGUTILS_H)
#define __STDSTRINGUTILS_H 1

#include <sstream>
#include <iomanip>

namespace stdext
{
    /// in line
    template<typename T>
    inline void string_trim_impl(T & str, T & delim)
    {
        str.erase(0, str.find_first_not_of(delim));
        str.erase(str.find_last_not_of(delim)+1);
    }
    template<typename T>
    inline void string_trimn(T & str)
    {
        T delim0;
        T delim1;
        T delim2;

        if constexpr (std::is_same<T, std::wstring>::value)
        {
            delim0 = L"\n";
            delim1 = L"\r";
            delim2 = L" ";
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            delim0 = "\n";
            delim1 = "\r";
            delim2 = " ";
        }
        string_trim_impl<T>(str, delim0);
        string_trim_impl<T>(str, delim1);
        string_trim_impl<T>(str, delim2);
    }
    template<typename T>
    inline void string_trim(T & str)
    {
        T delim;

        if constexpr (std::is_same<T, std::wstring>::value)
            delim = L" ";
        else if constexpr (std::is_same<T, std::string>::value)
            delim = " ";

        string_trim_impl<T>(str, delim);
    }
    template<typename T>
    inline T string_trimr(T str)
    {
        string_trim<T>(str);
        return str;
    }
    inline HMODULE hmodule_get()
    {
        HMODULE hmd;
#       if defined(_BUILD_DLL)
        if ((!GetModuleHandleEx(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCTSTR>(stdext::hmodule_get),
                &hmd)
        ) || (hmd == INVALID_HANDLE_VALUE))
#       else
        if ((hmd = GetModuleHandle(NULL)) == INVALID_HANDLE_VALUE)
#       endif
        return NULL;
    return hmd;
    }

    /// compare
    template<class T>
    inline bool string_end(T const & val, T const & ending)
    {
        if (ending.size() > val.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), val.rbegin());
    }
    template<class T>
    inline bool string_begin(T const & val, T const & start)
    {
        if (start.size() > val.size()) return false;
        return std::equal(start.begin(), start.end(), val.begin());
    }

	template<class T>
	typename T::size_type inline string_replace(T & src, const T & fstr, const T & rstr)
	{
	    typename T::size_type pos = 0U, cnt = 0U;

	    for (
              ;
              ((pos = src.find(fstr, pos)) != T::npos);
              pos += rstr.length()
            )
        {
            src.replace(pos, fstr.length(), rstr);
            cnt++;
        }
        return cnt;
	}

    /// class
	class DLL_EXPORT wstring_fmt {
	private:
		std::wstringstream _ws;
	public:
		template<class T>
		wstring_fmt& operator<< (const T& arg) {
			_ws << arg;
			return *this;
		}
		template<class T>
		wstring_fmt& operator>> (const T& arg) {
			std::ios_base::fmtflags _frm(_ws.flags());
			_ws << L"0x" << std::uppercase << std::setfill(L'0') << std::setw(sizeof(T) * 2) << std::hex << arg;
			_ws.flags(_frm);
			return *this;
		}
		operator std::wstring() const {
			return _ws.str();
		}
		const wchar_t * str() const {
			return _ws.str().c_str();
		}
		size_t length() {
			return _ws.str().length();
		}
		void clear() {
			_ws.clear();
		}
	};

	template<class T>
	class DLL_EXPORT string_sprintf
	{
	    size_t m_slen;
	    T m_sout;
    public:
        string_sprintf() : m_slen(0U), m_sout{} {}
        ~string_sprintf() { m_sout.clear(); }
        T get() { return m_sout; }
        T go(const T, ...);
	};
	template<>
	std::wstring string_sprintf<std::wstring>::go(const std::wstring, ...);
	template<>
	std::string string_sprintf<std::string>::go(const std::string, ...);

    /// class, using this utilities
	template<class T>
	class DLL_EXPORT string_from_res
	{
	    size_t m_slen;
	    HINSTANCE m_hinst;
	    T m_sout;
    public:
        string_from_res() : m_slen(0U), m_hinst(NULL), m_sout{} {}
        ~string_from_res()  { m_sout.clear(); }
        void init(HINSTANCE hinst)  { m_hinst = ((!hinst) ? hmodule_get() : hinst); }
        T get() { return m_sout; }
        T go(HINSTANCE hinst, uint32_t id);
	};
	template<>
	std::wstring string_from_res<std::wstring>::go(HINSTANCE, uint32_t);
	template<>
	std::string string_from_res<std::string>::go(HINSTANCE, uint32_t);

    /// function
	std::string base64_encode(const unsigned char*, size_t);
}

using stdext::wstring_fmt;
using stdext::string_trim;
using stdext::string_trimn;
using stdext::string_trimr;
using stdext::string_trim_impl;
using stdext::string_replace;
using stdext::string_sprintf;
using stdext::string_from_res;
using stdext::string_begin;
using stdext::string_end;
using stdext::base64_encode;
using stdext::hmodule_get;

#endif
