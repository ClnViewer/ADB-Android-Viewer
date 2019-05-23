
#include "../../../ADBViewer/src/loader.h"
#include "stdStringUtils.h"

namespace stdext
{
	static const char base64_table[65] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string DLL_EXPORT base64_encode(const unsigned char *src, size_t len)
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
	std::string DLL_EXPORT string_sprintf<std::string>::go(const std::string fmt, ...)
	{
        va_list ap;
        va_start(ap, fmt);
        {
            m_sout.clear();
            m_slen = static_cast<size_t>(_vscprintf(fmt.c_str(), ap));
            std::vector<char> buf(++m_slen);
            _vsnprintf_s(&buf[0], buf.size(), _TRUNCATE, fmt.c_str(), ap);
            m_sout.assign(&buf[0], &buf[0] + m_slen);
        }
        va_end(ap);
        return m_sout;
	}

	template<>
	std::wstring DLL_EXPORT string_sprintf<std::wstring>::go(const std::wstring fmt, ...)
	{
        va_list ap;
        va_start(ap, fmt);
        {
            m_sout.clear();
            m_slen = static_cast<size_t>(_vscwprintf(fmt.c_str(), ap));
            std::vector<wchar_t> buf(++m_slen * sizeof(wchar_t));
            _vsnwprintf_s(&buf[0], buf.size(), m_slen, fmt.c_str(), ap);
            m_sout.assign(&buf[0], &buf[0] + m_slen);
        }
        va_end(ap);
        return m_sout;
	}

	template<>
	std::wstring DLL_EXPORT string_from_res<std::wstring>::go(HINSTANCE hinst, uint32_t id)
	{
        const wchar_t *buf = NULL;
        init(hinst);
        m_sout.clear();
        m_slen = ::LoadStringW(m_hinst, id, (LPWSTR)&buf, 0);
        if ((m_slen) && (buf))
            m_sout.assign(buf, m_slen);
        return m_sout;
	}
	template<>
	std::string DLL_EXPORT string_from_res<std::string>::go(HINSTANCE hinst, uint32_t id)
	{
        const char *buf = NULL;
        init(hinst);
        m_sout.clear();
        m_slen = ::LoadStringA(m_hinst, id, (LPSTR)&buf, 0);
        if ((m_slen) && (buf))
            m_sout.assign(buf, m_slen);
        return m_sout;
	}
}
