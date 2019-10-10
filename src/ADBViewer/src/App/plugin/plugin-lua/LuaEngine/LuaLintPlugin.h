
#pragma once

namespace LuaLint
{

    enum ColorPrint
    {
        LocalError = 0,
        DebugError,
        DebugOk,
        DebugHelp,
        DebugTraceC,
        DebugTraceLua,
        DebugTraceStack
    };

    void print(std::string const&) {}
    void print(std::stringstream&) {}
    void print(std::string const&, COLORREF const &) {}
    void print(std::stringstream&, COLORREF const &) {}
    void print(std::string const&, LuaLint::ColorPrint) {}
    void print(std::stringstream&, LuaLint::ColorPrint) {}
    //
    void print_lua(lua_State*, COLORREF const&) {}
    void print_ipix3(ImageLite::IPIX3 const&) {}
    void print_ipix3(ImageLite::IPIX3 const&, ImageLite::IPIX3 const&) {}
    void print_LuaImage(ImageLite::ImgBuffer&, ImageLite::IPOINT<int32_t> const&) {}
    void print_LuaImage(ImageLite::ImageData&) {}
    void print_LuaImage_data(ImageLite::ImgBuffer&) {}
    void print_LuaImage_data(ImageLite::ImageData&) {}
    void print_pcall_error(int32_t) {}
    //
    void stream_print_error(std::stringstream&, std::string const&, int32_t) {}
    void stream_print_caller(std::stringstream&, std::string const&, std::string const&, int32_t) {}
    void stream_print_called(std::stringstream&, std::string const&, int32_t) {}
    void stream_print_return(std::stringstream&, std::string const&, int32_t) {}
    void stream_print_arg_warn(std::stringstream&, std::string const&, int32_t, int32_t) {}
    void stream_print_ipix3(std::stringstream&, ImageLite::IPIX3 const&) {}

    ///

    template <typename T>
    void stream_print_ipointT(std::stringstream &, ImageLite::IPOINT<T> const &) {}
    template <typename T>
    void stream_print_irectT(std::stringstream &, ImageLite::IRECT<T> const &) {}
    template <typename T>
    void stream_print_argT(std::stringstream &, T, int32_t) {}
    //
    template <typename T>
    void print_traceT(std::string const &, T) {}
    template <typename T>
    void print_ipointT(ImageLite::IPOINT<T> const &) {}
    template <typename T>
    void print_irectT(ImageLite::IRECT<T> const &) {}
    ///
};

