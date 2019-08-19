
#pragma once

#if defined(OS_WIN)

namespace GameDev
{

template<class T>
class ResStringDialog
	{
    private:
	    size_t    m_slen;
	    HINSTANCE m_hinst;
	    T         m_sout;

    public:
        ResStringDialog()
            : m_slen(0U), m_hinst(nullptr) {}
        void init(HINSTANCE h)
        {
            m_hinst = ((!h) ? GameDev::gethmodule() : h);
        }
        T get()
        {
            return m_sout;
        }
        T go(HINSTANCE h, uint32_t id)
        {
            init(h);
            if (!m_sout.empty())
                m_sout.clear();

            if constexpr (std::is_same<T, std::wstring>::value)
            {
                const wchar_t *buf = nullptr;
                m_slen = ::LoadStringW(m_hinst, id, (LPWSTR)&buf, 0);
                if ((m_slen) && (buf))
                    m_sout.assign(buf, m_slen);
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                const char *buf = nullptr;
                m_slen = ::LoadStringA(m_hinst, id, (LPSTR)&buf, 0);
                if ((m_slen) && (buf))
                    m_sout.assign(buf, m_slen);
            }
            return m_sout;
        }

	};
}

#endif

