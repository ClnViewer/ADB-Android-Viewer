/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../SCEditInternal.h"

namespace MDIWin
{
    static inline const char *l_cnf_font = "Segoe UI";
    static inline const LPCSTR l_image_id[] =
    {
        MAKEINTRESOURCE(ID_IMGL_TITLE_BITMAP),
        MAKEINTRESOURCE(ID_IMGL_TOOLBAR_BITMAP),
        MAKEINTRESOURCE(ID_IMGL_REBAR_BITMAP),
        MAKEINTRESOURCE(ID_IMGL_TOOLAPI_BITMAP)
    };

    static HINSTANCE f_getinstance()
    {
        HMODULE hmd;
        if ((!::GetModuleHandleEx(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCTSTR>(MDIWin::Config::instance),
                &hmd)) && (hmd != INVALID_HANDLE_VALUE))
            return nullptr;
        return static_cast<HINSTANCE>(hmd);
    }

    Config::Config()
    {
        m_hbrush[0] = ::CreateSolidBrush(RGB(240,240,240));
        m_hbrush[1] = ::CreateSolidBrush(RGB(250,250,250));
        m_hbrush[2] = ::CreateSolidBrush(RGB(255,255,255));
    }

    Config::~Config()
    {
        uint32_t i;
        for (i = 0U; i < __NELE(m_hbrush); i++)
        {
            if (m_hbrush[i])
                ::DeleteObject(m_hbrush[i]);
            m_hbrush[i] = nullptr;
        }
        //
        for (i = 0U; i < __NELE(m_himl); i++)
        {
            if (m_himl[i])
                ::ImageList_Destroy(m_himl[i]);
            m_himl[i] = nullptr;
        }
        //
        if (m_hfont)
            ::DeleteObject(m_hfont);
        m_hfont = nullptr;
    }

    Config& Config::instance()
    {
        static Config m_instance{};
        return m_instance;
    }

    void Config::init(HINSTANCE hinst_)
    {
        if (hinst_)
            m_hinst = hinst_;
    }

    void Config::clear()
    {
        m_hwnd[0] = nullptr;
        m_hwnd[1] = nullptr;
    }

    HINSTANCE Config::gethinstance()
    {
        if (!m_hinst)
            m_hinst = f_getinstance();
        return m_hinst;
    }

    HBRUSH Config::getbrush(StyleBrush t)
    {
        return m_hbrush[t];
    }

    std::string Config::getfontname()
    {
        return l_cnf_font;
    }

    HFONT Config::getfont()
    {
        if (!m_hfont)
            m_hfont = ::CreateFont(
                            20, 0, 0, 0,
                            FW_DONTCARE,
                            0, 0, 0,
                            ANSI_CHARSET,
                            OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY,
                            DEFAULT_PITCH | FF_SWISS,
                            l_cnf_font
                    );
        if (!m_hfont)
            return reinterpret_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
        return m_hfont;
    }

    HIMAGELIST Config::getiml(ImageButtonList t)
    {
        if (m_himl[t])
            return m_himl[t];

        HBITMAP hbmp = nullptr;

        do
        {
            if (!(hbmp = ::LoadBitmap(m_hinst, l_image_id[t])))
                break;

            BITMAP bmp{};
            if (!::GetObject(hbmp, sizeof(bmp), &bmp))
                break;

            if ((!bmp.bmWidth) || (!bmp.bmHeight))
                break;

            int32_t imgmax = (bmp.bmWidth / bmp.bmHeight);
            if (!imgmax)
                break;

            m_himl[t] = ::ImageList_Create(
                            bmp.bmHeight,
                            bmp.bmHeight,
                            ILC_COLOR16 | ILC_MASK,
                            imgmax,
                            0
                    );
            if (!m_himl[t])
                break;

            ::ImageList_AddMasked(m_himl[t], hbmp, RGB(128,0,128));
            ::DeleteObject(hbmp); hbmp = nullptr;

            if (ImageList_GetImageCount(m_himl[t]) < imgmax)
                break;

            return m_himl[t];
        }
        while (0);

        if (m_himl[t])
            ::ImageList_Destroy(m_himl[t]);
        m_himl[t] = nullptr;

        if (hbmp)
            ::DeleteObject(hbmp);
        return nullptr;
    }

    uint32_t Config::getiml(ImageButtonList t, ImageLite::IPOINT<int32_t> & ip)
    {
        if (!m_himl[t])
            getiml(t);

        if (!m_himl[t])
            return 0U;

        ::ImageList_GetIconSize(m_himl[t], &ip.x, &ip.y);
        return static_cast<uint32_t>(::ImageList_GetImageCount(m_himl[t]));
    }
};
