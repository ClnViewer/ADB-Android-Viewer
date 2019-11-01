
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
                Tval *byindex[2] = { &x, &y };

                IPOINT()
                 : x(0), y(0) {}
                IPOINT(Tval x_, Tval y_)
                 : x(x_), y(y_) {}
                //
                bool empty() const
                {
                    if constexpr (std::is_unsigned<Tval>::value)
                        return ((!x) && (!y));
                    else
                        return ((x <= 0) && (y <= 0));
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
                template<uint32_t Tn>
                Tval get()
                {
                    static_assert((Tn < 2), "index wrong, >= 2");
                    return *byindex[Tn];
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
                    set<decltype(reinterpret_cast<Tobj*>(0)->x)>(obj.x , obj.y);
                }
            };
            //
            template<typename Tval>
            struct IRECT
            {
                Tval x, y, w, h;
                Tval *byindex[4] = { &x, &y, &w, &h };

                IRECT()
                 : x(0), y(0), w(0), h(0) {}
                IRECT(Tval x_, Tval y_, Tval w_, Tval h_)
                 : x(x_), y(y_), w(w_), h(h_) {}
                //
                bool empty() const
                {
                    if constexpr (std::is_unsigned<Tval>::value)
                        return ((!w) || (!h));
                    else
                        return ((w <= 0) || (h <= 0));
                }
                bool contains(IPOINT<Tval> const & ip, Tval pad = 0) const
                {
                    return (
                        (ip.x >= x)       && // w
                        (ip.x <= (x + w)) &&
                        (ip.y >= y)       && // h
                        (ip.y <= (y + ((pad) ? pad : h)))
                            );
                }
#               if defined(__WIN32)
                void convert(RECT const & r)
                {
                    x = static_cast<Tval>(r.left);
                    y = static_cast<Tval>(r.top);
                    w = static_cast<Tval>(r.right  - r.left);
                    h = static_cast<Tval>(r.bottom - r.top);
                }
#               endif
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
                template<uint32_t Tn>
                Tval get()
                {
                    static_assert((Tn < 4), "index wrong, >= 4");
                    return *byindex[Tn];
                }
                //
                template<typename Ttype>
                IPOINT<Ttype> get()
                {
                    return {
                        static_cast<Ttype>(w),
                        static_cast<Ttype>(h)
                    };
                }
                //
                void set(IRECT<int32_t> const & ir)
                {
                    set<int32_t>(ir.x, ir.y, ir.w, ir.h);
                }
                //
                void set(IPOINT<int32_t> const & ip)
                {
                    set<int32_t>(ip.x, ip.y);
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
                    set<Tin>(0, 0, _w, _h);
                }
                //
                template<typename Ttype, typename Tin>
                void set(Tin const & obj)
                {
                    static_assert((sizeof(Tin) == (sizeof(Ttype) * 4)), "type different size");
                    uint8_t *xobj = reinterpret_cast<uint8_t*>(
                                    const_cast<Tin*>(&obj)
                                );
                    for (uint32_t i = 0U; i < 4; i++)
                        *byindex[i] =  static_cast<Tval>(
                                        *reinterpret_cast<Ttype*>(
                                            xobj + (sizeof(Ttype) * i)
                                        )
                                    );
                }
                //
                template<typename Tin>
                bool compare(Tin const _w, Tin const _h) const
                {
                    return ((w == static_cast<Tval>(_w)) &&
                            (h == static_cast<Tval>(_h)));
                }
                template<typename Ttype>
                bool compare(IRECT<Ttype> const & ir) const
                {
                    return ((x == static_cast<Tval>(ir.x)) &&
                            (y == static_cast<Tval>(ir.y)) &&
                            (w == static_cast<Tval>(ir.w)) &&
                            (h == static_cast<Tval>(ir.h)));
                }
                template<typename Ttype>
                bool compare(IPOINT<Ttype> const & ip) const
                {
                    return compare<Ttype>(ip.x, ip.y);
                }
                //
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
