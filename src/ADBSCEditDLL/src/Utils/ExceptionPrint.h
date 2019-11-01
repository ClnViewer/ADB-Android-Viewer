
#pragma once

    /*
    using:
        GameDev::ExceptionPrn::init(&MDIWin::Config::instance().colorBox); // once
        GameDev::ExceptionPrn::parse(std::current_exception());
    */

namespace GameDev
{
	class ExceptionPrn
	{
        public:
            //
            using except_prn_cb = std::function<void(std::string const&, COLORREF const&)>;

        private:
            //
            static inline const char m_msg[]   = " ! Caught exception: ";
            static inline const char m_empty[] = " ! Exception is empty identifier";
            static inline const char m_unk[]   = "Unknown type exception";
            static except_prn_cb     m_prn;
            static COLORREF const    m_color;

        public:
            //
            static void init(except_prn_cb);
            static void parse(std::exception_ptr, std::string const& = std::string());
	};
}
