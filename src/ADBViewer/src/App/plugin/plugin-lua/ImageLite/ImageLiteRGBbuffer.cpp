
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <windows.h>

#include "ImageLite.h"
#include <lodepng.h>

# define error_name()     l_err ## __FUNCTION__
# define error_decl()     char const * error_name() = nullptr
# define error_break(A)   { error_name() = A; break; }
# define throw_if_error() if (error_name()) throw std::runtime_error(error_name())

namespace ImageLite
{
    static inline const char l_err_01[] = "image buffer empty";
    static inline const char l_err_02[] = "image size mismatch";
    static inline const char l_err_03[] = "image pointer size mismatch";
    static inline const char l_err_04[] = "image pointer is null";
    static inline const char l_err_05[] = "image file name empty";
    static inline const char l_err_06[] = "image buffer bad index";
    static inline const char l_err_07[] = "image buffer bad size";
    static inline const char l_err_08[] = "image file not access";
    static inline const char l_err_09[] = "image file empty";
    static inline const char l_err_10[] = "image format not support";
    static inline const char l_err_11[] = "not support, use load_raw() function";
    static inline const char l_err_12[] = "image rectangle size is large";
    static inline const char l_err_13[] = "not crop image rectangle";
    static inline const char l_err_14[] = "image buffer not support type";
    // BMP errors
    static inline const char l_err_21[] = "image not bitmap format";
    static inline const char l_err_22[] = "bitmap image header not correct size";
    static inline const char l_err_23[] = "bitmap image color not 24";
    static inline const char l_err_24[] = "bitmap image format not RGB (BGR)";
    static inline const char l_err_25[] = "bitmap image header size mismatch";
    static inline const char l_err_26[] = "bitmap image format not convertible";
    static inline const char l_err_27[] = "bitmap image incorrect data size";

    static inline const char *l_ifmt[] =
    {
        "bmp",
        "png",
        "raw",
        "lua",
        nullptr
    };

    ImageRGBbuffer::ImageRGBbuffer() {}

    ImageRGBbuffer::ImageRGBbuffer(ImgBuffer const & b, uint32_t w, uint32_t h, uint32_t s, BufferType t)
    {
        if (!b.size())
            throw std::runtime_error(l_err_01);
        if ((!w) || (!h))
            throw std::runtime_error(l_err_02);

        ImageLite::ImageDataP idtmp
        {
            {},
            b,
            {
                static_cast<int32_t>(w),
                static_cast<int32_t>(h)
            },
            static_cast<int32_t>(s)
        };

        _init(idtmp, m_idata, t);

        if (getsize(SizeType::IST_CALC_BYTES).x != static_cast<int32_t>(m_idata.buffer.size()))
            throw std::runtime_error(l_err_07);

        if (!s)
            m_pad = _set_stride();
        else
            m_pad = getpad();
    }

    ImageRGBbuffer::ImageRGBbuffer(uint8_t const *b, uint32_t sz, uint32_t w, uint32_t h, uint32_t s, BufferType t)
    {
        if (!sz)
            throw std::runtime_error(l_err_03);
        if (!b)
            throw std::runtime_error(l_err_04);
        if ((!w) || (!h))
            throw std::runtime_error(l_err_02);

        ImageLite::ImageData idtmp
        {
            {},
            {},
            {
                static_cast<int32_t>(w),
                static_cast<int32_t>(h)
            },
            static_cast<int32_t>(s)
        };
        idtmp.buffer.resize(sz);
        idtmp.buffer.assign(&b[0], &b[sz]);

        _init(idtmp, m_idata, t);

        if (getsize(SizeType::IST_CALC_BYTES).x != static_cast<int32_t>(m_idata.buffer.size()))
            throw std::runtime_error(l_err_07);

        if (!s)
            m_pad = _set_stride();
        else
            m_pad = getpad();
    }

    ImageRGBbuffer::ImageRGBbuffer(std::string const & s, ImageType t)
    {
        if (s.empty())
            throw std::runtime_error(l_err_05);

        load(s, t);
    }

    ImageRGBbuffer::ImageRGBbuffer(std::string const & s, uint32_t w, uint32_t h, uint32_t s_, BufferType)
    {
        if (s.empty())
            throw std::runtime_error(l_err_05);

        // TODO (clanc#1#18.10.2019): check BufferType
        load_raw(s, w, h, s_);
    }

    ImageRGBbuffer::ImageRGBbuffer(
            ImageData const &idata, IRECT<int32_t> const & ir, BufferType t)
    {
        if (!idata.buffer.size())
            throw std::runtime_error(l_err_01);
        if ((!idata.point.x) || (!idata.point.y))
            throw std::runtime_error(l_err_02);
        if ((idata.point.x < (ir.w + ir.x)) || (idata.point.y < (ir.h + ir.y)))
            throw std::runtime_error(l_err_12);

        ImageLite::ImageData idtmp{};
        _init(idata, idtmp, t);

        if ((idtmp.point.x == ir.w) &&
            (idtmp.point.y == ir.h))
        {
            m_idata = idtmp;
            m_pad = _set_stride();
        }
        else
        {
            if(!ImageLite::Formater::igetrectangle(
                    idtmp,
                    m_idata,
                    ir
                ))
                throw std::runtime_error(l_err_13);
            m_pad = getpad();
        }
    }

    ImageRGBbuffer::ImageRGBbuffer(
            ImageData const &idata, IPOINT<int32_t> const & ip, BufferType t)
    {
        if (!idata.buffer.size())
            throw std::runtime_error(l_err_01);
        if ((!idata.point.x) || (!idata.point.y))
            throw std::runtime_error(l_err_02);
        if ((idata.point.x < ip.x) || (idata.point.y < ip.y))
            throw std::runtime_error(l_err_12);

        ImageLite::ImageData idtmp{};
        _init(idata, idtmp, t);

        if ((idtmp.point.x == ip.x) &&
            (idtmp.point.y == ip.y))
        {
            m_idata = idtmp;
            m_pad = _set_stride();
        }
        else
        {
            // TODO (clanc#1#18.10.2019): write resize
            throw std::runtime_error(l_err_13);
            m_pad = getpad();
        }
    }

    ImageRGBbuffer::ImageRGBbuffer(
            ImageData const &idata, BufferType t)
    {
        if (!idata.buffer.size())
            throw std::runtime_error(l_err_01);
        if ((!idata.point.x) || (!idata.point.y))
            throw std::runtime_error(l_err_02);

        ImageLite::ImageData idtmp{};
        _init(idata, idtmp, t);

        m_idata = idtmp;
        m_pad = _set_stride();
    }

    ImageRGBbuffer::~ImageRGBbuffer()
    {
    }

    ///

    template <typename T>
    void ImageRGBbuffer::_init(
            T const &src, ImageData & dst, BufferType t)
    {
        dst.point = src.point;
        BufferType tt = ispad(dst.point.x);
        // check pad
        switch (t)
        {
            case BufferType::IBT_BGR: { t = ((tt == BufferType::IBT_NOPAD) ? BufferType::IBT_BGR_PAD : t); break; }
            case BufferType::IBT_RGB: { t = ((tt == BufferType::IBT_NOPAD) ? BufferType::IBT_RGB_PAD : t); break; }
            default: break;
        }
        // convert is buffer not pad
        switch (t)
        {
            case BufferType::IBT_BGR:
            case BufferType::IBT_RGB:
                {
                    if(!ImageLite::Formater::iconvert(
                            &ImageLite::Formater::CONVERT_FROM_NOPAD,
                            src.buffer,
                            dst.buffer,
                            src.point.x, src.point.y, src.stride, (src.point.x % 4),
                            nullptr
                        ))
                        throw std::runtime_error(l_err_01);

                    if (!dst.buffer.size())
                        throw std::runtime_error(l_err_07);
                    break;
                }
            case BufferType::IBT_BGR_PAD:
            case BufferType::IBT_RGB_PAD:
                {
                    dst.buffer = src.buffer;
                    break;
                }
            default:
                throw std::runtime_error(l_err_14);
        }
        // convert buffer is bitmap reverse pixels
        switch (t)
        {
            case BufferType::IBT_RGB:
            case BufferType::IBT_RGB_PAD:
                {
                    break;
                }
            case BufferType::IBT_BGR:
            case BufferType::IBT_BGR_PAD:
                {
                    ImageLite::ImgBuffer ibtmp{};

                    if(!ImageLite::Formater::iconvert(
                                &ImageLite::Formater::CONVERT_BMP,
                                dst.buffer,
                                ibtmp,
                                dst.point.x, dst.point.y, dst.stride, (dst.point.x % 4),
                                nullptr
                            ))
                            throw std::runtime_error(l_err_01);

                    if (!ibtmp.size())
                        throw std::runtime_error(l_err_07);

                    dst.buffer = ibtmp;
                    break;
                }
            default:
                break;
        }
    }

    bool ImageRGBbuffer::empty()
    {
        return ((!m_idata.point.x) || (!m_idata.point.y) || (!m_idata.buffer.size()));
    }

    IPOINT<int32_t> ImageRGBbuffer::getsize(SizeType t)
    {
        IPOINT<int32_t> p{};

        switch (t)
        {
            case SizeType::IST_BUFFER_BYTES:
                {
                    p.set<std::size_t>(m_idata.buffer.size(), 0);
                    break;
                }
            case SizeType::IST_CALC_BYTES:
                {
                    p.set<int32_t>((m_idata.point.y * ((m_idata.point.x * 3) + m_pad)), 0);
                    break;
                }
            case SizeType::IST_IMAGE_WH_POINT:
                {
                    p.set<uint32_t>(m_idata.point.x, m_idata.point.y);
                    break;
                }
            default:
                break;
        }
        return p;
    }

    int32_t ImageRGBbuffer::getpos(IPOINT<int32_t> const & p)
    {
        return getpos(p.x, p.y);
    }

    int32_t ImageRGBbuffer::getpos(int32_t x, int32_t y)
    {
        return ((y * ((m_idata.point.x * 3) + m_pad)) + (x * 3));
    }

    int32_t ImageRGBbuffer::getpad()
    {
        return (m_idata.point.x % 4);
    }

    BufferType ImageRGBbuffer::ispad(int32_t w)
    {
        return (((
                ((w) ? w : m_idata.point.x) % 4) == 0) ?
                    BufferType::IBT_NOPAD : BufferType::IBT_PAD
            );
    }

    ImageLite::IPIX3 ImageRGBbuffer::getpixel(IPOINT<int32_t> const & p)
    {
        return getpixel(p.x, p.y);
    }

    ImageLite::IPIX3 ImageRGBbuffer::getpixel(int32_t x, int32_t y)
    {
        ImageLite::IPIX3 pix{};
        uint32_t pos = getpos(x, y);
        _check_buffer_pos(pos);
        ::memcpy(&pix, &m_idata.buffer[pos], sizeof(pix));
        return pix;
    }

    ImageLite::IPIX3 ImageRGBbuffer::getpixel(int32_t pos_)
    {
        ImageLite::IPIX3 pix{};
        uint32_t pos = static_cast<uint32_t>(pos_);
        _check_buffer_pos(pos);
        ::memcpy(&pix, &m_idata.buffer[pos], sizeof(pix));
        return pix;
    }

    ImageLite::ImageData & ImageRGBbuffer::getbuffer()
    {
        return m_idata;
    }

    void ImageRGBbuffer::_check_buffer_pos(uint32_t pos)
    {
        if (pos >= m_idata.buffer.size())
            throw std::runtime_error(l_err_06);
    }

    int32_t ImageRGBbuffer::_set_stride()
    {
        int32_t pad = getpad();
        m_idata.stride = ((pad) ? ((m_idata.point.x * 3) + pad) : 0);
        return pad;
    }

    ImageLite::ImageData ImageRGBbuffer::getbmp()
    {
        ImageLite::ImageData icnvbmp{};
        icnvbmp.point       = m_idata.point;
        icnvbmp.stride      = m_idata.stride;
        IPOINT<uint32_t> ip = m_idata.point.get<IPOINT<uint32_t>, uint32_t>();

        if(!ImageLite::Formater::iconvert(
                &ImageLite::Formater::CONVERT_BMP,
                m_idata.buffer,
                icnvbmp.buffer,
                ip.x, ip.y, m_idata.stride, m_pad,
                nullptr
                ))
                throw std::runtime_error(l_err_01);

        icnvbmp.header = getbmpheader();
        return icnvbmp;
    }

    ImageLite::IBMPHEADER ImageRGBbuffer::getbmpfileheader(uint32_t sz)
    {
        ImageLite::IBMPHEADER bmph{};
        bmph.fh.bfType = 0x4D42; // "BM"
        bmph.fh.bfSize = ((sz) ? sz : m_idata.buffer.size()) + sizeof(bmph);
        bmph.fh.bfOffBits = sizeof(bmph);
        bmph.ih = getbmpheader();
        return bmph;
    }

    ImageLite::IBITMAPINFOHEADER ImageRGBbuffer::getbmpheader()
    {
        ImageLite::IBITMAPINFOHEADER bmi{};
        bmi.biSize   = sizeof(bmi);
        bmi.biWidth  = m_idata.point.x;
        bmi.biHeight = m_idata.point.y;
        bmi.biPlanes = 1U;
        bmi.biBitCount = 24; //(b * 8);
        bmi.biCompression = 0x0000; // BI_RGB
        return bmi;
    }

    double ImageRGBbuffer::icompare(ImageLite::ImageRGBbuffer & dst, uint8_t pc)
    {
        ImageData & data = dst.getbuffer();
        IRECT<int32_t> r = { 0, 0, data.point.x, data.point.y };
        return ImageLite::Formater::icompare(
                m_idata,
                data,
                r,
                pc);
    }

    double ImageRGBbuffer::icompare(ImageLite::ImageRGBbuffer & dst, IRECT<int32_t> const & r, uint8_t pc)
    {
        ImageData & data = dst.getbuffer();
        return ImageLite::Formater::icompare(
                m_idata,
                data,
                r,
                pc);
    }

    double ImageRGBbuffer::icompare(ImageLite::ImageData const & dst, uint8_t pc)
    {
        IRECT<int32_t> r = { 0, 0, dst.point.x, dst.point.y };
        return ImageLite::Formater::icompare(
                m_idata,
                dst,
                r,
                pc);
    }

    double ImageRGBbuffer::icompare(ImageLite::ImageData const & dst, IRECT<int32_t> const & r, uint8_t pc)
    {
        return ImageLite::Formater::icompare(
                m_idata,
                dst,
                r,
                pc);
    }

    ImageLite::ImageType ImageRGBbuffer::getimgtype(std::string const & s)
    {
        std::size_t pos = s.find_last_of(".");
        if (pos == std::string::npos)
            return ImageType::IT_NONE;

        if (s.compare((pos + 1), 3, l_ifmt[0]) == 0)
            return ImageType::IT_BMP;
        if (s.compare((pos + 1), 3, l_ifmt[1]) == 0)
            return ImageType::IT_PNG;
        if (s.compare((pos + 1), 3, l_ifmt[2]) == 0)
            return ImageType::IT_RAW;
        if (s.compare((pos + 1), 3, l_ifmt[3]) == 0)
            return ImageType::IT_ILUA;
        else
            return ImageType::IT_NONE;
    }

    void ImageRGBbuffer::load(std::string const & s, ImageType t)
    {
        if (t == ImageType::IT_NONE)
            if ((t = getimgtype(s)) == ImageType::IT_NONE)
                throw std::runtime_error(l_err_10);

        switch (t)
        {
            case ImageType::IT_PNG:
                {
                    error_decl();

                    do
                    {
                        ImageLite::ImgBuffer icnvpng;
                        uint32_t w,
                                 h,
                                 err = lodepng::decode(icnvpng, w, h, s, LCT_RGB, 8);
                        if (err)
                            error_break(lodepng_error_text(err));

                        if (!icnvpng.size())
                            error_break(l_err_01);

                        m_idata.point.set<uint32_t>(w, h);
                        m_pad = _set_stride();

                        switch (ispad(m_idata.point.x))
                        {
                            case BufferType::IBT_PAD:
                                {
                                    if(!ImageLite::Formater::iconvert(
                                            &ImageLite::Formater::CONVERT_FROM_NOPAD,
                                            icnvpng,
                                            m_idata.buffer,
                                            m_idata.point.x, m_idata.point.y, m_idata.stride, m_pad,
                                            nullptr
                                        ))
                                        error_break(l_err_01);

                                    if (!m_idata.buffer.size())
                                        error_break(l_err_07);
                                    break;
                                }
                            case BufferType::IBT_NOPAD:
                                {
                                    m_idata.buffer = icnvpng;
                                    m_pad = _set_stride();
                                    break;
                                }
                            default:
                                return;
                        }
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            case ImageType::IT_BMP:
                {
                    error_decl();

                    do
                    {
                        std::ifstream f(s.c_str (), std::ios::binary);

                        if (!f.is_open())
                            error_break(l_err_08);

                        ImageLite::IBMPHEADER bmph{};
                        f.read(reinterpret_cast<char*>(&bmph), sizeof(bmph));

                        if (bmph.fh.bfType != 0x4D42)
                            error_break(l_err_21);

                        if ((!bmph.ih.biWidth) || (!bmph.ih.biHeight))
                            error_break(l_err_22);

                        if (bmph.ih.biBitCount != 24)
                            error_break(l_err_23);

                        if (bmph.ih.biCompression != 0x0000)
                            error_break(l_err_24);

                        if ((!bmph.fh.bfOffBits) || (bmph.fh.bfSize <= bmph.fh.bfOffBits))
                            error_break(l_err_25);

                        m_idata.point.set<long>(bmph.ih.biWidth, bmph.ih.biHeight);
                        m_pad = _set_stride();

                        uint32_t sz = (bmph.fh.bfSize - bmph.fh.bfOffBits);
                        ImageLite::ImgBuffer icnvbmp(sz);
                        f.read(reinterpret_cast<char*>(&icnvbmp[0]), sz);

                        if (icnvbmp.size() != sz)
                            error_break(l_err_25);

                        if(!ImageLite::Formater::iconvert(
                                &ImageLite::Formater::CONVERT_BMP,
                                icnvbmp,
                                m_idata.buffer,
                                m_idata.point.x, m_idata.point.y, m_idata.stride, m_pad,
                                nullptr
                            ))
                            error_break(l_err_26);
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            case ImageType::IT_RAW:
                {
                    throw std::runtime_error(l_err_11);
                }
            default:
                throw std::runtime_error(l_err_10);
        }
    }

    /// RGB bytes file format (BMP buffer, not reverse bytes)
    void ImageRGBbuffer::load_raw(std::string const & s, uint32_t w, uint32_t h, uint32_t s_)
    {
        error_decl();

        do
        {
            if ((!w) || (!h))
                error_break(l_err_02);
            if (s.empty())
                error_break(l_err_01);

            std::ifstream f(s.c_str (), std::ios::binary | std::ios::ate);
            if (!f.is_open())
                error_break(l_err_08);

            std::size_t sz = f.tellg();
            if (!sz)
                error_break(l_err_09);

            f.seekg(0, std::ios::beg);
            m_idata.buffer.resize(sz);
            f.read(reinterpret_cast<char*>(&m_idata.buffer[0]), sz);

            m_idata.point.set<uint32_t>(w, h);

            if (!s_)
                m_pad = _set_stride();
            else
            {
                m_idata.stride = static_cast<int32_t>(s_);
                m_pad = getpad();
            }
            return;
        }
        while (0);

        throw_if_error();
    }

    void ImageRGBbuffer::save(std::string const & s, ImageType t)
    {
        if (!m_idata.buffer.size())
            throw std::runtime_error(l_err_01);

        if (t == ImageType::IT_NONE)
            t = getimgtype(s);

        // end file name parse
        switch (t)
        {
            case ImageType::IT_PNG:
                {
                    error_decl();

                    do
                    {
                        ImageLite::ImgBuffer dst;
                        ImageLite::ImgBuffer icnvpng;
                        IPOINT<uint32_t> ip = m_idata.point.get<IPOINT<uint32_t>, uint32_t>();
                        BufferType tt = ispad(m_idata.point.x);

                        if (tt == BufferType::IBT_PAD)
                        {
                            if(!ImageLite::Formater::iconvert(
                                    &ImageLite::Formater::CONVERT_TO_NOPAD,
                                    m_idata.buffer,
                                    icnvpng,
                                    ip.x, ip.y, m_idata.stride, m_pad,
                                    nullptr
                                ))
                                error_break(l_err_01);

                            if (!icnvpng.size())
                                error_break(l_err_07);

                            icnvpng.resize(ip.y * (ip.x * 3));
                        }
                        uint32_t err = lodepng::encode(
                                        dst,
                                        ((tt == BufferType::IBT_PAD) ? icnvpng : m_idata.buffer),
                                        ip.x,
                                        ip.y,
                                        LCT_RGB,
                                        8
                                    );
                        if (err)
                            error_break(lodepng_error_text(err));

                        if (!dst.size())
                            error_break(l_err_01);

                        lodepng::save_file(dst, s);
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            case ImageType::IT_RAW:
                {
                    error_decl();

                    do
                    {
                        std::ofstream f(s.c_str (), std::ios::binary | std::ios::ate);
                        if (!f.is_open())
                            error_break(l_err_08);
                        f.write (reinterpret_cast<const char*>(&m_idata.buffer[0]), m_idata.buffer.size());
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            case ImageType::IT_BMP:
                {
                    error_decl();

                    do
                    {
                        ImageLite::ImgBuffer icnvbmp;
                        IPOINT<uint32_t> ip = m_idata.point.get<IPOINT<uint32_t>, uint32_t>();

                        if(!ImageLite::Formater::iconvert(
                                &ImageLite::Formater::CONVERT_BMP,
                                m_idata.buffer,
                                icnvbmp,
                                ip.x, ip.y, m_idata.stride, m_pad,
                                nullptr
                            ))
                            error_break(l_err_26);

                        if (!icnvbmp.size())
                            error_break(l_err_01);

                        std::ofstream f(s.c_str (), std::ios::binary | std::ios::ate);
                        if (!f.is_open())
                            error_break(l_err_08);

                        ImageLite::IBMPHEADER bmph = getbmpfileheader(icnvbmp.size());
                        f.write (reinterpret_cast<const char*>(&bmph), sizeof(bmph));
                        f.write (reinterpret_cast<const char*>(&icnvbmp[0]), icnvbmp.size());
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            case ImageType::IT_ILUA:
                {
                    error_decl();

                    do
                    {
                        std::string valname;
                        std::ofstream f(s.c_str (), std::ios::binary | std::ios::ate);
                        if (!f.is_open())
                            error_break(l_err_08);

                        IPOINT<uint32_t> ip = m_idata.point.get<IPOINT<uint32_t>, uint32_t>();
                        {
                            std::size_t pos;
                            if ((pos = s.find_last_of("/\\")) != std::string::npos)
                                valname = s.substr(pos + 1, s.length() - 1);
                            else
                                valname = s;

                            if ((pos = valname.find_last_of(".")) != std::string::npos)
                                valname = valname.substr(0, pos);
                        }

                        f << valname << "_img = {\n";
                        f << "\twidth  = " << ip.x << ",\n";
                        f << "\theight = " << ip.y << ",\n";
                        f << "\titype  = " << (int)BufferType::IBT_RGB << ", -- IBT_RGB\n";
                        f << "\tdata   = {\n";

                        if(!ImageLite::Formater::itostream(
                                &ImageLite::Formater::STREAM_LUA_TABLE,
                                m_idata.buffer,
                                ip.x, ip.y, m_pad,
                                f
                            ))
                            error_break(l_err_26);

                        f << "\t}\n};\nreturn " << valname << "_img\n\n";
                        return;
                    }
                    while (0);

                    throw_if_error();
                    break;
                }
            default:
                throw std::runtime_error(l_err_10);
        }
    }
};
