
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <atomic>
#include <functional>
#include <cassert>
#include <windows.h>

#include "ImageLite.h"
#define _PIXNUM 3
#define _PIXSTRUCT ImageLite::IPIX3

namespace ImageLite
{
    void Formater::CONVERT_BMP(
            ImageLite::ImgBuffer const & src,
            ImageLite::ImgBuffer & dst,
            uint32_t,
            uint32_t h,
            uint32_t x,
            uint32_t y,
            uint32_t,
            uint32_t wlen,
            void*)
    {
        std::size_t spos = ((y * wlen) + (x * _PIXNUM)),
                    dpos = (((h - y - 1) * wlen) + (x * _PIXNUM));

        assert(static_cast<int32_t>(spos) >= 0);
        assert(static_cast<int32_t>(dpos) >= 0);

        _PIXSTRUCT *pix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&src[spos])
                    );
        dst[dpos + 0] = pix->c3;
        dst[dpos + 1] = pix->c2;
        dst[dpos + 2] = pix->c1;
    }

    void Formater::CONVERT_TO_NOPAD(
            ImageLite::ImgBuffer const & src,
            ImageLite::ImgBuffer & dst,
            uint32_t w,
            uint32_t,
            uint32_t x,
            uint32_t y,
            uint32_t,
            uint32_t wlen,
            void*)
    {
        std::size_t spos = ((y * wlen) + (x * _PIXNUM)),
                    dpos = ((y * (w * _PIXNUM)) + (x * _PIXNUM));

        assert(static_cast<int32_t>(spos) >= 0);
        assert(static_cast<int32_t>(dpos) >= 0);

        _PIXSTRUCT *pix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&src[spos])
                    );
        dst[dpos + 0] = pix->c1;
        dst[dpos + 1] = pix->c2;
        dst[dpos + 2] = pix->c3;
    }

    void Formater::CONVERT_FROM_NOPAD(
            ImageLite::ImgBuffer const & src,
            ImageLite::ImgBuffer & dst,
            uint32_t w,
            uint32_t,
            uint32_t x,
            uint32_t y,
            uint32_t,
            uint32_t wlen,
            void*)
    {
        std::size_t spos = ((y * (w * _PIXNUM)) + (x * _PIXNUM)),
                    dpos = ((y * wlen) + (x * _PIXNUM));

        assert(static_cast<int32_t>(spos) >= 0);
        assert(static_cast<int32_t>(dpos) >= 0);

        _PIXSTRUCT *pix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&src[spos])
                    );
        dst[dpos + 0] = pix->c1;
        dst[dpos + 1] = pix->c2;
        dst[dpos + 2] = pix->c3;
    }

    void Formater::STREAM_LUA_TABLE(
            ImageLite::ImgBuffer const & src,
            ImageLite::ImgBuffer&,
            uint32_t,
            uint32_t,
            uint32_t x,
            uint32_t y,
            uint32_t,
            uint32_t wlen,
            void    *userdata)
    {
        if (!userdata)
            return;

        std::ofstream *s = reinterpret_cast<std::ofstream*>(userdata);
        std::size_t spos = ((y * wlen) + (x * _PIXNUM));
        assert(static_cast<int32_t>(spos) >= 0);

        _PIXSTRUCT *pix = reinterpret_cast<_PIXSTRUCT*>(
                const_cast<uint8_t*>(&src[spos])
            );
        *s << "\t\t{ " << (int)pix->c1 << ", " << (int)pix->c2 << ", " << (int)pix->c3 << " },\n";
    }

    ///

    bool Formater::iconvert(
            std::function<ImageLite::icovert_cb> fun,
            ImageLite::ImgBuffer const & src,
            ImageLite::ImgBuffer & dst,
            uint32_t w,
            uint32_t h,
            uint32_t stride,
            uint32_t wpad,
            void *userdata)
    {
        if (!src.size())
            return false;

        uint32_t y,
                 wlen = (w * _PIXNUM) + wpad;

        assert((wlen % 4) == 0);
        dst.resize(h * wlen);

#       pragma omp parallel for private(y) schedule(static)
        for(y = 0; y < h; y++)
            for(uint32_t x = 0; x < w; x++)
            {
                fun (src, dst, w, h, x, y, stride, wlen, userdata);
            }
        return true;
    }

    bool Formater::igetrectangle(
            ImageLite::ImageData const & src,
            ImageLite::ImageData & dst,
            IRECT<int32_t> const & r)
    {
        if (!src.buffer.size())
            return false;
        if ((!r.w) || (!r.h))
            return false;

        IRECT<uint32_t>  drect;
        IPOINT<uint32_t> spoint;
        drect.set<IRECT<int32_t>>(r);
        spoint.set<IPOINT<int32_t>>(src.point);

        uint32_t y,
                 swpad = (spoint.x % 4),
                 swlen = ((spoint.x * _PIXNUM) + swpad),
                 dwpad = (drect.w % 4),
                 dwlen = ((drect.w * _PIXNUM) + dwpad),
                 dyend = (drect.y + drect.h),
                 dxend = (drect.x + drect.w);

        assert((swlen % 4) == 0);
        assert((dwlen % 4) == 0);

        dst.buffer.resize(drect.h * dwlen);
        dst.point.set<uint32_t>(drect.w, drect.h);
        dst.stride = ((dwpad) ? static_cast<int32_t>(dwlen) : 0);

#       pragma omp parallel for private(y) schedule(static)
        for(y = 0; y < spoint.y; y++)
        {
            if ((y >= drect.y) && (y < dyend)) {} else continue;
            for(uint32_t x = 0; x < spoint.x; x++)
            {
                if ((x >= drect.x) && (x < dxend)) {} else continue;
                std::size_t spos = ((y * swlen) + (x * _PIXNUM)),
                            dpos = (((y - drect.y) * dwlen) + ((x - drect.x) * _PIXNUM));

                _PIXSTRUCT *pix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&src.buffer[spos])
                    );
                ::memcpy(&dst.buffer[dpos], pix, sizeof(*pix));
            }
        }
        return true;
    }

    bool Formater::iresize(
            ImageLite::ImageData const & src,
            ImageLite::ImageData & dst,
            IPOINT<int32_t> const & p)
    {
        if (!src.buffer.size())
            return false;
        if ((!p.x) || (!p.y))
            return false;

        (void)dst;

        // TODO (clanc#1#17.10.2019): write latter

        return true;
    }

    static inline uint8_t pix_icompare(uint8_t spix, uint8_t dpix, uint8_t pc)
    {
        return ((spix == dpix) ?
                1U : ((pc) ? ((std::max(spix, dpix) - std::min(spix, dpix)) <= pc) : 0U)
            );
    }

    double Formater::icompare(
            ImageLite::ImageData const & src,
            ImageLite::ImageData const & dst,
            IRECT<int32_t> const & r,
            uint8_t pc)
    {
        if ((!src.buffer.size()) ||
            (!dst.buffer.size()))
            return 0.0;

        if ((!r.w)                      ||
            (!r.h)                      ||
            (!src.point.x)              ||
            (!src.point.y)              ||
            ((r.w + r.x) > src.point.x) ||
            ((r.h + r.y) > src.point.y))
            return 0.0;

        IPOINT<uint32_t> spoint;
        IRECT<uint32_t>  drect;
        drect.set<IRECT<int32_t>>(r);
        spoint.set<IPOINT<int32_t>>(src.point);

        uint32_t y,
                 cmp   = 0,
                 swpad = (spoint.x % 4),
                 swlen = ((spoint.x * _PIXNUM) + swpad),
                 dwpad = (drect.w % 4),
                 dwlen = ((drect.w * _PIXNUM) + dwpad),
                 dyend = (drect.y + drect.h),
                 dxend = (drect.x + drect.w);

        assert((swlen % 4) == 0);
        assert((dwlen % 4) == 0);

#       pragma omp parallel for private(y) schedule(static) reduction(+:cmp)
        for(y = 0; y < spoint.y; y++)
        {
            if ((y >= drect.y) && (y < dyend)) {} else continue;
            for(uint32_t x = 0; x < spoint.x; x++)
            {
                if ((x >= drect.x) && (x < dxend)) {} else continue;
                std::size_t spos = ((y * swlen) + (x * _PIXNUM)),
                            dpos = (((y - drect.y) * dwlen) + ((x - drect.x) * _PIXNUM));

                _PIXSTRUCT *spix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&src.buffer[spos])
                    );
                _PIXSTRUCT *dpix = reinterpret_cast<_PIXSTRUCT*>(
                        const_cast<uint8_t*>(&dst.buffer[dpos])
                    );
                uint8_t rpix = 0;
                {
                    rpix += pix_icompare(spix->c1, dpix->c1, pc);
                    rpix += pix_icompare(spix->c2, dpix->c2, pc);
                    rpix += pix_icompare(spix->c3, dpix->c3, pc);
                }
                cmp += rpix;
            }
        }
        if (!cmp)
            return 0.0;

        uint32_t dsz = ((drect.w * _PIXNUM) * drect.h);
        if (cmp == dsz)
            return 100.0;

        return (static_cast<double>(cmp) / (static_cast<double>(dsz) / 100.0));
    }

    bool Formater::itostream(
            std::function<ImageLite::icovert_cb> fun,
            ImageLite::ImgBuffer const & src,
            uint32_t w,
            uint32_t h,
            uint32_t wpad,
            std::ofstream & s)
    {
        if (!src.size())
            return false;

        uint32_t y,
                 wlen = (w * _PIXNUM) + wpad;

        assert((wlen % 4) == 0);
        ImageLite::ImgBuffer btmp;

        for(y = 0; y < h; y++)
            for(uint32_t x = 0; x < w; x++)
            {
                fun (src, btmp, w, h, x, y, 0, wlen, static_cast<void*>(&s));
            }
        return true;
    }

};
