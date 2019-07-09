/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

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

#include "../ADBViewer.h"
#include "../extern/lodepng.h"

static inline const LPCSTR l_openBmpFilter = "Bitmap files (*.bmp)\0*.bmp\0PNG files (*.png)\0*.png\0\0";
static inline const LPCSTR l_openBmpExt = "bmp";
static inline const LPCSTR l_openCurDir = ".\\";

bool AppScreen::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        return guiBase::initgui(app);
    }

uint32_t AppScreen::getpixel(SDL_Surface *s, int32_t x, int32_t y)
    {
        uint8_t *p = (uint8_t*)s->pixels + y * s->pitch + x * s->format->BytesPerPixel;
        switch (s->format->BytesPerPixel)
        {
            case 1: return static_cast<uint32_t>(*(uint8_t*)p);
            case 2: return static_cast<uint32_t>(*(uint16_t*)p);
            case 3:
#           if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
#           else
                return p[2] | p[1] << 8 | p[0] << 16;
#           endif
            case 4:
#           if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
#           else
                return p[2] | p[1] << 8 | p[0] << 16 | p[3] << 24;
#           endif
        }
        return 0U;
    }


SDL_Surface * AppScreen::getscreen()
    {
        SDL_Surface *l_ss_surface = nullptr;
        SDL_Rect *r = m_app->m_appvideo.guiBase::GetGui<SDL_Rect>();

        do
        {
            l_ss_surface = SDL_CreateRGBSurface(
                0U,
                r->w,
                r->h,
                32,
#               if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff
#               else
                0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
#               endif
            );
            if (!l_ss_surface)
                break;

            SDL_RenderReadPixels(
                guiBase::GetGui<SDL_Renderer>(),
                r,
                SDL_GetWindowPixelFormat(guiBase::GetGui<SDL_Window>()),
                l_ss_surface->pixels,
                l_ss_surface->pitch
            );
        }
        while (0);

        return l_ss_surface;
    }


bool AppScreen::screencopy()
    {
        bool         ret = false;
        SDL_Surface *l_ss_surface = nullptr;
        void        *l_ss_screen = nullptr;

        do
        {
            if (!m_app)
                break;

            if (!(l_ss_surface = getscreen()))
                break;

            if (!(l_ss_screen = getsysscreen(l_ss_surface)))
                break;

            if (!(setclipboard(l_ss_screen)))
                break;

            AddMessageQueue(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_CLIPBOARD,
                    AppConfig::instance().cnf_lang
                ),
                10U,
                -1
            );

            ret = true;
        }
        while (0);

        if (l_ss_surface)
            SDL_FreeSurface(l_ss_surface);
        if (l_ss_screen)
            freesysscreen(l_ss_screen);

        return ret;
    }

bool AppScreen::screenshot(bool isdialog)
{
    {
        bool         ret = false;
        SDL_Surface *l_ss_surface = nullptr;

        do
        {
            if (!m_app)
                break;

            std::string fname(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_CAPFILENAME,
                    AppConfig::instance().cnf_lang
                )
            );

#           if !defined(OS_WIN)
            isdialog = false;
#           endif

            if (isdialog)
            {
                if (!AppSysDialog::savefile(
                        guiBase::GetGui<SDL_Window>(),
                        fname,
                        l_openBmpFilter,
                        l_openBmpExt,
                        l_openCurDir
                    ))
                    break;
            }
            else
            {
                std::stringstream ss;
                ss << fname << std::to_string(time(NULL));
                ss << AppConfig::instance().cnf_save_fmt.c_str();
                fname.assign(ss.str().c_str());
            }

            if (!(l_ss_surface = getscreen()))
                break;

            ///
            {
                size_t pos;

                if (
                    ((pos = fname.find_last_of(".")) != std::wstring::npos) &&
                    (fname.compare(pos, 4, AppConfig::instance().GetImageSaveFmt(1U)) == 0)
                    )
                {
                    if (!savepng(l_ss_surface, fname))
                        break;
                }
                else
                    if (SDL_SaveBMP(l_ss_surface, fname.c_str()))
                        break;
            }

            std::stringstream ss;
            ss << ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_FILESAVE,
                    AppConfig::instance().cnf_lang
                );
            ss << fname.c_str();

            AddMessageQueue(
                ss.str().c_str(),
                10U,
                -1
            );

            ret = true;
        }
        while (0);

        if (l_ss_surface)
            SDL_FreeSurface(l_ss_surface);

        return ret;
    }
}

bool  AppScreen::savepng(SDL_Surface *s, std::string const & fpath)
{
    do
    {
        if (!s)
            break;

        std::vector<uint8_t> l_spng;
        l_spng.resize(s->w * s->h * 4);

        for(int32_t y = 0U, offset; y < s->h; y++)
        {
            offset = ((4 * s->w) * y);

            for(int32_t x = 0U; x < s->w; x++)
            {
                uint32_t pixel  = getpixel(s, x, y),
                         pos = (offset + (x * 4));

#               if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                l_spng[(pos + 0)] = (pixel  >> 24);
                l_spng[(pos + 1)] = ((pixel >> 16) & 0xff);
                l_spng[(pos + 2)] = ((pixel >> 8)  & 0xff);
                l_spng[(pos + 3)] = (pixel         & 0xff);
#               else
                l_spng[(pos + 0)] = (pixel         & 0xff);
                l_spng[(pos + 1)] = ((pixel >> 8)  & 0xff);
                l_spng[(pos + 2)] = ((pixel >> 16) & 0xff);
                l_spng[(pos + 3)] = (pixel  >> 24);
#               endif
            }
        }

        std::vector<uint8_t> l_vpng;
        if (lodepng::encode(l_vpng, l_spng, s->w, s->h))
            break;

        lodepng::save_file(l_vpng, fpath.c_str());
        return true;
    }
    while (0);

    return false;
}

bool AppScreen::uevent(SDL_Event *ev, const void *instance)
    {
        AppScreen *asc = static_cast<AppScreen*>(
                const_cast<void*>(instance)
            );

        if (!asc)
            return false;

        switch(ev->user.code)
        {
            /// Save Screen to default name
            case ID_CMD_POP_MENU4:
                {
                    asc->screenshot(false);
                    return true;
                }
            /// Save Screen to select user dialog name
            case ID_CMD_POP_MENU5:
                {
                    asc->screenshot(true);
                    return true;
                }
            /// Copy Screen to clipboard
            case ID_CMD_POP_MENU22:
                {
                    asc->screencopy();
                    return true;
                }
        }
        return false;
    }


