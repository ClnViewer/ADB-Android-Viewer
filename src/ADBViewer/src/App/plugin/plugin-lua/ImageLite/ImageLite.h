
#pragma once

#if defined(__WIN32)
//
#  if !defined(_WIN32_IE)
#    define _WIN32_IE 0x0600
#  endif
#  if !defined(_WIN32_WINNT)
#    define _WIN32_WINNT 0x0502
#  endif
//
#  include <windows.h>
//
#  if __has_include(<gdiplus.h>)
#    include <gdiplus.h>
#    define _BUILD_GDIPLUS_ENABLE 1
#  endif
//
#endif

namespace ImageLite
{
    /// DATA:
    /// all image structures and enumerator
    ///
            enum BufferType
            {
                IBT_RGB,
                IBT_BGR,
                IBT_RGB_PAD,
                IBT_BGR_PAD,
                IBT_BGRA,
                IBT_RGBA,
                IBT_PAD,
                IBT_NOPAD,
                IBT_NONE
            };
            //
            enum ImageType
            {
                IT_BMP,
                IT_PNG,
                IT_RAW,
                IT_ILUA,
                IT_NONE
            };
            //
            enum SizeType
            {
                IST_BUFFER_BYTES,
                IST_CALC_BYTES,
                IST_IMAGE_WH_POINT,
                IST_NONE
            };
            //
            struct IPIX3
            {
                uint8_t c1, c2, c3;
            };
            struct IPIX4
            {
                uint8_t c1, c2, c3, c4;
            };
            //
            template<typename Tval>
            struct IPOINT
            {
                Tval x, y;

                bool empty()
                {
                    return ((!x) && (!y));
                }
                //
                template<typename Tobj, typename Tin>
                Tobj get()
                {
                    return {
                        static_cast<Tin>(x),
                        static_cast<Tin>(y)
                    };
                }
                //
                template<typename Tin>
                void set(Tin const _x, Tin const _y)
                {
                    x = static_cast<Tval>(_x);
                    y = static_cast<Tval>(_y);
                }
                //
                template<typename Tobj>
                void set(Tobj const & obj)
                {
                    x = static_cast<Tval>(obj.x);
                    y = static_cast<Tval>(obj.y);
                }
            };
            //
            template<typename Tval>
            struct IRECT
            {
                Tval x, y, w, h;

                bool empty()
                {
                    return ((!w) && (!h));
                }
                //
                template<typename Tobj, typename Tin>
                Tobj get()
                {
                    return {
                        static_cast<Tin>(x),
                        static_cast<Tin>(y),
                        static_cast<Tin>(w),
                        static_cast<Tin>(h)
                    };
                }
                //
                template<typename Tin>
                void set(Tin const _x, Tin const _y, Tin const _w, Tin const _h)
                {
                    x = static_cast<Tval>(_x);
                    y = static_cast<Tval>(_y);
                    w = static_cast<Tval>(_w);
                    h = static_cast<Tval>(_h);
                }
                //
                template<typename Tin>
                void set(Tin const _w, Tin const _h)
                {
                    x = 0;
                    y = 0;
                    w = static_cast<Tval>(_w);
                    h = static_cast<Tval>(_h);
                }
                //
                template<typename T>
                void set(T const & obj)
                {
#                   if defined(__WIN32)
                    if constexpr (
                        (std::is_same<T, RECT>::value) ||
                        (std::is_same<T, RECTL>::value))
                    {
                        x = static_cast<Tval>(obj.left);
                        y = static_cast<Tval>(obj.top);
                        w = static_cast<Tval>(obj.right);
                        h = static_cast<Tval>(obj.bottom);
                    }
#                   if defined(_BUILD_GDIPLUS_ENABLE)
                    else if constexpr (std::is_same<T, Gdiplus::Rect>::value)
                    {
                        x = static_cast<Tval>(obj.x);
                        y = static_cast<Tval>(obj.y);
                        w = static_cast<Tval>(obj.width);
                        h = static_cast<Tval>(obj.height);
                    }
                    else
#                   endif
#                   endif
                    {
                        x = static_cast<Tval>(obj.x);
                        y = static_cast<Tval>(obj.y);
                        w = static_cast<Tval>(obj.w);
                        h = static_cast<Tval>(obj.h);
                    }
                }
            };
            //
            typedef struct __attribute__((__packed__))
            {
                uint16_t  bfType;
                uint32_t  bfSize;
                uint32_t  bfReserved; // 1,2
                uint32_t  bfOffBits;
            } IBITMAPFILEHEADER;
            //
            typedef struct __attribute__((__packed__))
            {
                uint32_t biSize;
                long     biWidth;
                long     biHeight;
                uint16_t biPlanes;
                uint16_t biBitCount;
                uint32_t biCompression;
                uint32_t biSizeImage;
                long     biXPelsPerMeter;
                long     biYPelsPerMeter;
                uint32_t biClrUsed;
                uint32_t biClrImportant;
            } IBITMAPINFOHEADER;
            //
            typedef struct
            {
                ImageLite::IBITMAPFILEHEADER fh;
                ImageLite::IBITMAPINFOHEADER ih;
            } IBMPHEADER;
            //
            using ImgBuffer = std::vector<uint8_t>;
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
            using  icovert_cb = void (
                    ImageLite::ImgBuffer const&,
                    ImageLite::ImgBuffer&,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    uint32_t,
                    void*
                );
#           define icovert_BIND(F)                 \
                std::bind(&F,                      \
                          this,                    \
                          std::placeholders::_1,   \
                          std::placeholders::_2,   \
                          std::placeholders::_3,   \
                          std::placeholders::_4,   \
                          std::placeholders::_5,   \
                          std::placeholders::_6,   \
                          std::placeholders::_7,   \
                          std::placeholders::_8,   \
                          std::placeholders::_9)
            //
            struct ImageData
            {
                ImageLite::IBITMAPINFOHEADER header;
                ImageLite::ImgBuffer         buffer;
                ImageLite::IPOINT<int32_t>   point;
                int32_t                      stride;
            };
            //
            struct ImageDataP
            {
                ImageLite::IBITMAPINFOHEADER header;
                ImageLite::ImgBuffer const & buffer;
                ImageLite::IPOINT<int32_t>   point;
                int32_t                      stride;
            };
            //
};

#include "ImageLiteFormater.h"
#include "ImageLiteRGBbuffer.h"
#if defined(_BUILD_IMAGELITE_DRAW)
#  include "ImageLiteDraw.h"
#endif
