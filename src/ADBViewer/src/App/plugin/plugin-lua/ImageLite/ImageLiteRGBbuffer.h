
#pragma once

namespace ImageLite
{
    /// BASE CLASS:
    /// format buffer RGB + padding (BI_RGB)
    /// all error convert to exception
    class ImageRGBbuffer
    {
        public:
            //
            ImageRGBbuffer();
            ImageRGBbuffer(ImageData const&, BufferType = BufferType::IBT_RGB_PAD);
            ImageRGBbuffer(ImageData const&, IRECT<int32_t> const&, BufferType = BufferType::IBT_RGB_PAD);  // get rectangle from source ImageData
            ImageRGBbuffer(ImageData const&, IPOINT<int32_t> const&, BufferType = BufferType::IBT_RGB_PAD); // resize from source ImageData
            ImageRGBbuffer(ImgBuffer const&, uint32_t, uint32_t, uint32_t = 0, BufferType = BufferType::IBT_RGB_PAD);
            ImageRGBbuffer(uint8_t const*, uint32_t, uint32_t, uint32_t, uint32_t = 0, BufferType = BufferType::IBT_RGB_PAD);
            ImageRGBbuffer(std::string const&, ImageType t = ImageType::IT_NONE);
            ImageRGBbuffer(std::string const&, uint32_t, uint32_t, uint32_t = 0, BufferType = BufferType::IBT_RGB_PAD);
            ~ImageRGBbuffer();
            //
            void                         load(std::string const&, ImageType t = ImageType::IT_NONE);
            void                         load_raw(std::string const&, uint32_t, uint32_t, uint32_t = 0);
            void                         save(std::string const&, ImageType t = ImageType::IT_NONE);
            bool                         empty();
            BufferType                   ispad(int32_t = 0);
            int32_t                      getpad();
            IPOINT<int32_t>              getsize(SizeType);
            ImageData &                  getbuffer();
            //
            int32_t                      getpos(IPOINT<int32_t> const&);
            int32_t                      getpos(int32_t, int32_t);
            //
            ImageLite::IPIX3             getpixel(int32_t);
            ImageLite::IPIX3             getpixel(IPOINT<int32_t> const&);
            ImageLite::IPIX3             getpixel(int32_t, int32_t);
            //
            ImageLite::ImageType         getimgtype(std::string const&);
            //
            ImageLite::ImageData         getbmp();
            ImageLite::IBITMAPINFOHEADER getbmpheader();
            ImageLite::IBMPHEADER        getbmpfileheader(uint32_t = 0);
            //
            double                       icompare(ImageLite::ImageRGBbuffer&, uint8_t = 1U);
            double                       icompare(ImageLite::ImageRGBbuffer&, IRECT<int32_t> const&, uint8_t = 1U);
            double                       icompare(ImageLite::ImageData const&, uint8_t = 1U);
            double                       icompare(ImageLite::ImageData const&, IRECT<int32_t> const&, uint8_t = 1U);

        private:
            //
            int32_t                 m_pad = 0;
            ImageLite::ImageData    m_idata{};
            //
            template <typename T>
            void                    _init(T const&, ImageData&, BufferType);
            void                    _check_buffer_pos(uint32_t);
            int32_t                 _set_stride();
    };
};

