
#pragma once

namespace ImageLite
{
    class Formater
    {
        public:
            //
            /*
                // iconvert()
                bool (
                      callback function [type icovert_cb]
                      buffer source,
                      buffer destination,
                      width,
                      height,
                      stride,
                      pad [align w length],
                      user data void* )
            */
            static bool iconvert(
                    std::function<ImageLite::icovert_cb>,
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void* = nullptr);
            //
            static bool igetrectangle(
                    ImageLite::ImageData const&,
                    ImageLite::ImageData&,
                    IRECT<int32_t> const&);

            static bool iresize(
                    ImageLite::ImageData const&,
                    ImageLite::ImageData&,
                    IPOINT<int32_t> const&);

            static double icompare(
                    ImageLite::ImageData const&,
                    ImageLite::ImageData const&,
                    IRECT<int32_t> const&,
                    uint8_t = 0U);

            static bool itostream(
                    std::function<ImageLite::icovert_cb>,
                    ImageLite::ImgBuffer const&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    std::ofstream&);

            //
            /*
                // Converter callback
                void (
                      buffer source,
                      buffer destination,
                      width,
                      height,
                      x [w iterate],
                      y [h iterate],
                      s [stride],
                      l [width bytes length]
                      v [user data void*] )
            */
            //
            static void CONVERT_BMP(
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void*);

            static void CONVERT_TO_NOPAD(
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void*);

            static void CONVERT_FROM_NOPAD(
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void*);

            static void STREAM_LUA_TABLE(
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void*);
    };
};

