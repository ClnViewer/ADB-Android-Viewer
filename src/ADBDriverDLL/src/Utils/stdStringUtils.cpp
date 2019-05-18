
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <windows.h>
#include "stdStringUtils.h"

namespace stdext
{
	static const char base64_table[65] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


	std::string base64_encode(const unsigned char *src, size_t len)
	{
		unsigned char *out, *pos;
		const unsigned char *end, *in;
		size_t olen;
		olen = 4 * ((len + 2) / 3);
		if (olen < len)
			return std::string();

		std::string ostr;
		ostr.resize(olen);
		out = (unsigned char*)&ostr[0];
		end = src + len;
		in = src;
		pos = out;

		while (end - in >= 3) {
			*pos++ = base64_table[in[0] >> 2];
			*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
			*pos++ = base64_table[in[2] & 0x3f];
			in += 3;
		}
		if (end - in)
		{
			*pos++ = base64_table[in[0] >> 2];
			if ((end - in) == 1)
			{
				*pos++ = base64_table[(in[0] & 0x03) << 4];
				*pos++ = '=';
			}
			else
			{
				*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
				*pos++ = base64_table[(in[1] & 0x0f) << 2];
			}
			*pos++ = '=';
		}
		return ostr;
	}

	template<>
	std::string string_sprintf<std::string>::go(const std::string fmt, ...)
	{
        va_list ap;
        va_start(ap, fmt);
        {
            _sout.clear();
            _slen = static_cast<size_t>(_vscprintf(fmt.c_str(), ap));
            std::vector<char> buf(++_slen);
            _vsnprintf_s(&buf[0], buf.size(), _TRUNCATE, fmt.c_str(), ap);
            _sout.assign(&buf[0], &buf[0] + _slen);
        }
        va_end(ap);
        return _sout;
	}

	template<>
	std::wstring string_sprintf<std::wstring>::go(const std::wstring fmt, ...)
	{
        va_list ap;
        va_start(ap, fmt);
        {
            _sout.clear();
            _slen = static_cast<size_t>(_vscwprintf(fmt.c_str(), ap));
            std::vector<wchar_t> buf(++_slen * sizeof(wchar_t));
            _vsnwprintf_s(&buf[0], buf.size(), _slen, fmt.c_str(), ap);
            _sout.assign(&buf[0], &buf[0] + _slen);
        }
        va_end(ap);
        return _sout;
	}

	template<>
	std::wstring string_from_res<std::wstring>::go(HINSTANCE hinst, uint32_t id)
	{
        const wchar_t *buf = NULL;
        init(hinst);
        _sout.clear();
        _slen = ::LoadStringW(_hinst, id, (LPWSTR)&buf, 0);
        if ((_slen) && (buf))
            _sout.assign(buf, _slen);
        return _sout;
	}
	template<>
	std::string string_from_res<std::string>::go(HINSTANCE hinst, uint32_t id)
	{
        const char *buf = NULL;
        init(hinst);
        _sout.clear();
        _slen = ::LoadStringA(GetModuleHandle(NULL), id, (LPSTR)&buf, 0);
        if ((_slen) && (buf))
            _sout.assign(buf, _slen);
        return _sout;
	}
}
