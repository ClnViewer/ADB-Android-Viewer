#pragma once

namespace stdext
{

class DLL_EXPORT MultiString
{
private:
    //
    std::wstring m_str;

public:
    //
    bool Is() const;
    void Clear();

    template<typename T>
        void Set(T const&);

    template<typename T>
        T Get();

    const  std::wstring & operator =(const MultiString&) const;
           std::ostream & operator <<(std::ostream&);
};

extern template void            MultiString::Set<std::wstring>(std::wstring const&);
extern template void            MultiString::Set<std::string> (std::string const&);

extern template const wchar_t * MultiString::Get<const wchar_t*>();
extern template const char    * MultiString::Get<const char*>();
extern template std::wstring    MultiString::Get<std::wstring>();
extern template std::string     MultiString::Get<std::string>();

}

using stdext::MultiString;
