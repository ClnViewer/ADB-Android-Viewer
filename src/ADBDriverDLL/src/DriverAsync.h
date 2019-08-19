
#pragma once

namespace GameDev
{
    class DLL_EXPORT DriverAsync
    {
        private:
            std::thread                   m_thwatch;
            std::atomic<bool>             m_iswatch;
            std::queue<std::future<bool>> m_queue;

        public:
            DriverAsync();
            ~DriverAsync();
            template <typename T> void add(T, T const&, std::function<bool(T, T const&, T&, DriverConst::ClearType)>);
    };
    extern template void DriverAsync::add<std::wstring>(std::wstring, std::wstring const&, std::function<bool(std::wstring, std::wstring const&, std::wstring&, DriverConst::ClearType)>);
    extern template void DriverAsync::add<std::string> (std::string,  std::string const&,  std::function<bool(std::string,  std::string  const&, std::string&,  DriverConst::ClearType)>);

}
