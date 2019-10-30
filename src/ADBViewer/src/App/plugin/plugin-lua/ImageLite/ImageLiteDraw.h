
#pragma once

namespace ImageLite
{
    class Draw
    {
        public:
            //
            struct ImageDrawSelect
            {
                bool                       active, backup;
                ImageLite::IRECT<int32_t>  r;
                ImageLite::IPOINT<int32_t> pb;

                ImageDrawSelect()
                    : active(false), backup(true), r{}, pb{} {}

                template<typename T>
                void init(T const & p)
                {
                    do
                    {
                        if ((p.x < 0) || (p.y < 0))
                        {
                            r = {};
                            active = false;
                            break;
                        }
                        r.x = p.x;
                        r.y = p.y;
                        active = true;
                    }
                    while (0);
                }
                template<typename T>
                bool set(T const & p)
                {
                    if ((!active) || (p.x <= 0) || (p.y <= 0))
                        return false;

                    ImageLite::IPOINT<int32_t> pt;
                    pt.set<int32_t>(
                                (p.x - r.x),
                                (p.y - r.y)
                            );
                    pb.x = ((backup) ?
                                 r.w :
                                 ((pt.x > r.w) ? pt.x : pb.x)
                            );
                    pb.y = ((backup) ?
                                 r.h :
                                 ((pt.y > r.h) ? pt.y : pb.y)
                            );
                    r.w    = ((pt.x > 0) ? pt.x : 0);
                    r.h    = ((pt.y > 0) ? pt.y : 0);
                    backup = false;
                    return true;
                }
                template <typename T>
                T get(int32_t xborder, int32_t yborder)
                {
                    return {
                        (r.x - xborder),
                        (r.y - yborder),
                        r.w,
                        r.h
                    };
                }
                template <typename T>
                T get(int32_t border = 0)
                {
                    return {
                        (r.x - border),
                        (r.y - border),
                        r.w,
                        r.h
                    };
                }
                void erase()
                {
                    r  = {};
                    pb = {};
                    active = false;
                    backup = true;
                }
                bool empty(bool is = false)
                {
                    if (is)
                        return ((r.w <= 0) || (r.h <= 0));
                    return ((r.x <= 0) || (r.y <= 0));
                }
                std::string print(int32_t border = 0)
                {
                    ImageLite::IRECT<int32_t> ir = get<ImageLite::IRECT<int32_t>>(border);
                    std::stringstream ss;
                    print_stream_(ss, ir);
                    return ss.str();
                }
                std::string print(ImageLite::ImageData & idata, int32_t border = 0)
                {
                    ImageLite::IRECT<int32_t> ir = get<ImageLite::IRECT<int32_t>>(border);
                    std::stringstream ss;
                    print_stream_(ss, ir);
                    print_stream_(ss, ir, idata);
                    return ss.str();
                }
                RECT eraseregion()
                {
                    backup = true;
                    return {
                        ((r.x > 25) ? (r.x - 25) : 0),
                        ((r.y > 25) ? (r.y  - 25) : 0),
                        (r.x  + 25  + ((pb.x > r.w) ? pb.x : r.w)),
                        (r.y  + 25  + ((pb.y > r.h) ? pb.y : r.h))
                    };
                }

            private:

                void print_stream_(
                        std::stringstream & ss,
                        ImageLite::IRECT<int32_t> & ir)
                {
                    ss << "  x:" << ir.x;
                    ss << "  y:" << ir.y;
                    if (ir.w)
                        ss << "  w:" << ir.w;
                    if (ir.h)
                        ss << "  h:" << ir.h;
                }
                void print_stream_(
                        std::stringstream & ss,
                        ImageLite::IRECT<int32_t> & ir,
                        ImageLite::ImageData & idata)
                {
                    uint32_t pos = (
                        (ir.y * (
                                 (idata.point.x * 3) +
                                 (idata.point.x % 4)
                                 )) + (ir.x * 3)
                        );
                    if ((pos + 2) >= idata.buffer.size())
                        return;

                    ss << "  |  RGB { ";
                    ss << static_cast<int32_t>(idata.buffer[pos + 0]) << ", ";
                    ss << static_cast<int32_t>(idata.buffer[pos + 1]) << ", ";
                    ss << static_cast<int32_t>(idata.buffer[pos + 2]) << " }";
                }
            };

            Draw(HINSTANCE = nullptr, HWND = nullptr);   // using .add(...) to set image
            Draw(                                        // set one image
                 ImageRGBbuffer &
            );
            Draw(                                        // set two images
                ImageRGBbuffer &,
                ImageRGBbuffer &
            );
            ~Draw();
            //
            void                    init(HINSTANCE = nullptr, HWND = nullptr);
            void                    close(bool = true);
            bool                    show(std::string const&, HINSTANCE = nullptr, HWND = nullptr);
            void                    add(int32_t, ImageRGBbuffer&);
            void                    add(int32_t, ImageRGBbuffer&, std::string const&);
            bool                    isactive();

        private:
            //
            HWND                    m_hwnd      = nullptr;
            HWND                    m_hparent   = nullptr;
            HWND                    m_hstatus   = nullptr;
            HINSTANCE               m_hinst     = nullptr;
            HIMAGELIST              m_imgl      = nullptr;
            ULONG_PTR               m_idgdi     = 0;
            ImageData               m_idata[2]{};
            ImageDrawSelect         m_select{};
            std::atomic<int32_t>    m_scale     = 1;
            std::atomic<int32_t>    m_index     = 0;
            std::atomic<bool>       m_isrun     = false;
            std::atomic<bool>       m_issysmenu = false;
            std::atomic<bool>       m_iswintop  = false;
            std::thread             m_thr;
            //
            LRESULT                 wndProc_(HWND, UINT, WPARAM, LPARAM);
            static LRESULT CALLBACK wndRedirect_(HWND, UINT, WPARAM, LPARAM);
            //
            HWND                    gethwnd();
            HINSTANCE               gethinstance();
            POINT                   getwinsize(int32_t = 0);
            POINT                   getwinpadding();
            //
            void                    setwinsize(POINT const &);
            void                    setwintop();
            bool                    setup(HINSTANCE);
            bool                    initgdi();
            void                    sysmenu();
            void                    sysmenu(uint32_t,uint32_t);
            void                    statusbar();
            void                    statusbar(std::string const&);
            void                    errbox(std::string const&);
            void                    errhandle(std::string const&);
    };
};


