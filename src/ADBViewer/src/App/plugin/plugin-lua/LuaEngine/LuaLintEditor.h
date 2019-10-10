
#pragma once

namespace LuaLint
{
    static inline const char g_lint_str_20[] = " -- Help: ";
    static inline const char g_lint_str_23[] = " -- User script call time sleep action: ";
    static inline const char g_lint_str_24[] = " -- User script set state to: ";
    static inline const char g_lint_str_25[] = " -- User script get state: ";
    static inline const char g_lint_str_26[] = " -- Invalid table object, count: ";
    static inline const char g_lint_str_27[] = " -- Engine exception: ";
    static inline const char g_lint_str_28[] = " -- Unknown exception in function: ";
    static inline const char g_lint_str_29[] = " -- Function return: ";
    static inline const char g_lint_str_30[] = " -- Function bad argument: ";
    static inline const char g_lint_str_31[] = " -- Function argument: ";
    static inline const char g_lint_str_32[] = " -- Function start: ";
    static inline const char g_lint_str_33[] = " -- Image details: ";
    static inline const char g_lint_str_34[] = " -- Image data is empty..";
    static inline const char g_lint_str_35[] = " -- Image broking size, offset: ";
    static inline const char g_lint_str_36[] = " -- Image arguments: ";
    static inline const char g_lint_str_37[] = " -- Image set error, to emulate Android frame buffer";
    static inline const char g_lint_str_38[] = " -- Image position: ";
    static inline const char g_lint_str_39[] = " -- Emulate Android frame buffer image is not set: ";
    static inline const char g_lint_str_40[] = "time string incorrect format";
    static inline const char g_lint_str_41[] = "image file name is empty";
    static inline const char g_lint_str_42[] = "not negative value!";
    static inline const char g_lint_str_43[] = "argument is not LuaImage table";
    static inline const char g_lint_str_44[] = "linter -> unknown error";
    static inline const char g_lint_str_45[] = "using ' LuaObject:imageDefault(filename.png) ' to set it..";
    static inline const char g_lint_str_46[] = "unknown image type return";
    static inline const char g_lint_str_47[] = "ImageLite return empty object";
    static inline const char g_lint_str_48[] = "bad image data";
    static inline const char g_lint_str_49[] = "save image limit exceed: ";
    static inline const char g_lint_str_50[] = "bad rectangle data";

    enum ColorPrint
    {
        LocalError = 0,
        DebugError,
        DebugOk,
        DebugHelp,
        DebugTraceC,
        DebugTraceLua,
        DebugTraceStack,
        LuaFunctionPrint,
        LuaFunctionDprint
    };

   /*
    void __attribute__((weak)) print(std::string const&);
    void __attribute__((weak)) print(std::stringstream&);
    void __attribute__((weak)) print(std::string const&, COLORREF const &);
    void __attribute__((weak)) print(std::stringstream&, COLORREF const &);
    */

    void print(std::string const&);
    void print(std::stringstream&);
    void print(std::string const&, COLORREF const &);
    void print(std::stringstream&, COLORREF const &);
    void print(std::string const&, LuaLint::ColorPrint);
    void print(std::stringstream&, LuaLint::ColorPrint);
    //
    void print_lua(lua_State*, COLORREF const&);
    void print_ipix3(ImageLite::IPIX3 const&);
    void print_ipix3(ImageLite::IPIX3 const&, ImageLite::IPIX3 const&);
    void print_LuaImage(ImageLite::ImgBuffer&, ImageLite::IPOINT<int32_t> const&);
    void print_LuaImage(ImageLite::ImageData&);
    void print_LuaImage_data(ImageLite::ImgBuffer&);
    void print_LuaImage_data(ImageLite::ImageData&);
    void print_pcall_error(int32_t);
    //
    void stream_print_error(std::stringstream&, std::string const&, int32_t);
    void stream_print_caller(std::stringstream&, std::string const&, std::string const&, int32_t);
    void stream_print_called(std::stringstream&, std::string const&, int32_t);
    void stream_print_return(std::stringstream&, std::string const&, int32_t);
    void stream_print_arg_warn(std::stringstream&, std::string const&, int32_t, int32_t);
    void stream_print_ipix3(std::stringstream&, ImageLite::IPIX3 const&);

    ///

    template <typename T>
    void stream_print_ipointT(std::stringstream & ss, ImageLite::IPOINT<T> const & ip)
    {
        ss << "{ x:" << ip.x << ", y:" << ip.y << " }";
    }
    template <typename T>
    void stream_print_irectT(std::stringstream & ss, ImageLite::IRECT<T> const & ir)
    {
        ss << "{ x:" << ir.x << ", y:" << ir.y;
        ss << ", w:" << ir.w << ", h:" << ir.h << " }";
    }
    template <typename T>
    void stream_print_argT(std::stringstream & ss, T val, int32_t n)
    {
        ss << "\t\targument: " << " " << n << " = '" << (T)val << "'\n";
    }
    //
    template <typename T>
    void print_traceT(std::string const & s, T val)
    {
        std::stringstream ss;
        ss << s.c_str() << " " << val;
        LuaLint::print(ss);
    }
    template <typename T>
    void print_ipointT(ImageLite::IPOINT<T> const & ip)
    {
        std::stringstream ss;
        LuaLint::stream_print_ipointT<T>(ss, ip);
        LuaLint::print_traceT(" -- IPOINT: ", ss.str());
    }
    template <typename T>
    void print_irectT(ImageLite::IRECT<T> const & ir)
    {
        std::stringstream ss;
        LuaLint::stream_print_irectT<T>(ss, ir);
        LuaLint::print_traceT(" -- IRECT: ", ss.str());
    }
    ///
};

