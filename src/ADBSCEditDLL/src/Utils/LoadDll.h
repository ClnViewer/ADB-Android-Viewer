
#pragma once

namespace GameDev
{

class LoadDll
	{
        private:
            //
            HMODULE  m_lib = nullptr;
            uint32_t m_err = 0U;

        public:
            //
            LoadDll() {}
            LoadDll(std::string const & s)
            {
                init(s);
            }
            ~LoadDll()
            {
                if (m_lib)
                    ::FreeLibrary(m_lib);
                m_lib = nullptr;
            }
            void init(std::string const & s)
            {
                try
                {
                    if (s.empty())
                    {
                        m_err = ERROR_INVALID_DATA;
                        return;
                    }
                    if (m_lib)
                    {
                        m_err = ERROR_ALREADY_ASSIGNED;
                        return;
                    }
                    m_lib = ::LoadLibraryA(s.c_str());
                    if (!m_lib)
                        m_err = ::GetLastError();
                }
                catch (...) { m_err = ::GetLastError(); }
            }
            bool isload() const
            {
                return (m_lib);
            }
            uint32_t geterror() const
            {
                return m_err;
            }
	};
}
