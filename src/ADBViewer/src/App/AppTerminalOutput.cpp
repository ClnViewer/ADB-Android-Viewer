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

AppTerminalOutput::~AppTerminalOutput()
    {
        if (m_surface)
            SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }

bool AppTerminalOutput::init(App *app, AppTerminalPage *atp)
    {
        if ((!app) || (!atp))
            return false;

        m_app = app;
        m_page = atp;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = &m_page->out_rsrc;

        bool ret = guiBase::initgui(app);
        do
        {
            if (!ret)
                break;

            ret = ((m_font = ResManager::fontload(
                                ResManager::IndexFontResource::RES_FONT_CONSOLAS
                        )));
            if (!ret)
                break;

            ret = ((m_color_cmd = ResManager::colorload(
                                ResManager::IndexColorResource::RES_COLOR_TERMINAL_CMD
                        )));
            if (!ret)
                break;

            ret = ((m_color_txt = ResManager::colorload(
                                ResManager::IndexColorResource::RES_COLOR_TERMINAL_TXT
                        )));
            if (!ret)
                break;

            ret = m_pagenum.init(
                    app,
                    atp,
                    [=]()
                    {
                        pageUp();
                    }
            );
        }
        while (0);

        guiBase::ActiveOff();
        return ret;
    }

bool AppTerminalOutput::tinit(SDL_Texture **texture)
    {
        if ((!m_page->out_ssize.x) || (!m_page->out_ssize.y))
            return true;

        bool ison = guiBase::IsActive();
        if (ison)
            guiBase::ActiveOff();

        if (m_surface)
            SDL_FreeSurface(m_surface);
        m_surface = nullptr;

        if (!(m_surface = SDL_CreateRGBSurface(
                    SDL_SWSURFACE,
                    m_page->out_ssize.x,
                    m_page->out_ssize.y,
                    32,
#                   if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff
#                   else
                    0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
#                   endif
                )))
            return false;

        SDL_FillRect(m_surface, nullptr, SDL_MapRGB(m_surface->format, 0, 0, 0));
        SDL_Texture *l_texture = SDL_CreateTextureFromSurface(
                guiBase::GetGui<SDL_Renderer>(),
                m_surface
            );

        if (!l_texture)
            return false;

        GuiLock(
            std::swap(*texture, l_texture);
        );

        if (l_texture)
            SDL_DestroyTexture(l_texture);

        if (ison)
            guiBase::ActiveOn();

        return true;
    }

bool AppTerminalOutput::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        if (tinit(texture))
            guiBase::ActiveOn();
        return guiBase::IsActive();
    }

void AppTerminalOutput::start()
    {
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        if (!gr->texture)
            if (!tinit(&gr->texture))
            {
                if (gr->texture)
                    SDL_DestroyTexture(gr->texture);

                gr->texture = nullptr;
                return;
            }

        ::memcpy(&gr->rdst, &m_page->out_rdst, sizeof(SDL_Rect));
        gr->rsrc = &m_page->out_rsrc;

        guiBase::ActiveOn();
    }

void AppTerminalOutput::stop()
    {
        guiBase::ActiveOff();
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        if (gr->texture)
            SDL_DestroyTexture(gr->texture);
        gr->texture = nullptr;

        if (m_surface)
            SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }

void AppTerminalOutput::pageUp()
    {
        if (!m_page)
            return;

        m_page->out_npage  -= 1;
        m_page->out_npage  = ((m_page->out_npage < 0) ? 0 : m_page->out_npage);
        m_page->out_rsrc.y = (m_page->out_rdst.h * m_page->out_npage);
        m_page->out_rsrc.x = 0;
        m_pagenum.draw();
    }

void AppTerminalOutput::pageDown()
    {
        if (!m_page)
            return;

        int32_t n = ((m_page->out_npage >= (SCROLL_NUM_SCREEN - 1)) ? 0 : (m_page->out_npage + 1)),
                y = (m_page->out_rdst.h * n);

        if (y >= m_page->out_pos.y)
            return;

        m_page->out_npage = n;
        m_page->out_rsrc.y = y;
        m_page->out_rsrc.x = 0;
        m_pagenum.draw();
    }

void AppTerminalOutput::draw_cmd(std::string const & s)
    {
        if (s.empty())
            return;

        std::string sid;
        std::stringstream ss;
        m_page->out_tpage = 0;

        if (AppConfig::instance().cnf_adb.IsDeviceID())
            sid = AppConfig::instance().cnf_adb.GetDeviceID<std::string>();
        if (!sid.empty())
            ss << sid;
        ss << AppTerminalPage::term_prompt << s;
        drawline(ss.str(), m_color_cmd, true);
    }

void AppTerminalOutput::draw_txt(std::string const & s)
    {
        if (s.empty())
            return;

        do
        {
            std::vector<std::string> vs;
            std::string found("\n");
            size_t prev = 0U,
                   pos  = 0U;
            do
            {
                pos = s.find(found, prev);
                if (pos == std::string::npos)
                    pos = s.length();

                std::string ss = s.substr(prev, (pos - prev));
                if (!ss.empty())
                    vs.push_back(ss);

                prev = pos + found.length();
            }
            while (pos < s.length() && prev < s.length());

            if (!vs.size())
                break;

            for (auto & str : vs)
                drawline(str, m_color_txt, false);

            return;
        }
        while (0);

        drawline(s, m_color_txt, false);
    }

void AppTerminalOutput::drawline(std::string const & s, SDL_Color *color, bool isclean)
    {
        if (s.empty())
            return;

        SDL_Surface *l_surface = nullptr;
        SDL_Texture *l_texture = nullptr;
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();
        isclean = (((!m_surface) || (m_page->out_ssize.y <= m_page->out_pos.y)) ? true : isclean);

        do
        {
            if (isclean)
            {
                m_page->out_rsrc.x = 0;
                m_page->out_rsrc.y = 0;
                m_page->out_pos.y = 0;
                m_page->out_npage = 0;

                if (!tinit(&gr->texture))
                    break;
            }
            else if (m_page->out_rdst.h < m_page->out_pos.y)
            {
                m_page->out_npage  = (m_page->out_pos.y / m_page->out_rdst.h);
                m_page->out_rsrc.y = (m_page->out_rdst.h * m_page->out_npage);
                m_page->out_rsrc.x = 0;
            }
            if (!(l_surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                s.c_str(),
                                color[0],
                                color[1]
                            )))
                break;

            SDL_Rect drect{},
                     srect{};

            SDL_Point point_img_menu = ResManager::imagesize(
                    ResManager::IndexImageResource::RES_IMG_MENU_ACTIVE
            );

            drect.w = l_surface->w;
            drect.h = l_surface->h;
            drect.x = point_img_menu.x;
            drect.y = m_page->out_pos.y;

            srect.w = l_surface->w;
            srect.h = l_surface->h;
            srect.x = 0;
            srect.y = 0;

            SDL_BlitSurface(l_surface, &srect, m_surface, &drect);
            l_texture = SDL_CreateTextureFromSurface(
                guiBase::GetGui<SDL_Renderer>(),
                m_surface
            );

            if (!l_texture)
                break;

            GuiLock(
                std::swap(gr->texture, l_texture);
            );
            m_page->out_pos.y += l_surface->h;

            if (l_texture)
                SDL_DestroyTexture(l_texture);
            SDL_FreeSurface(l_surface);

            guiBase::ActiveOn();

            if (m_page->out_npage != m_page->out_tpage)
            {
                m_page->out_tpage = m_page->out_npage;
                m_pagenum.draw();
            }
            return;
        }
        while (0);

        guiBase::ActiveOff();

        if (l_texture)
            SDL_DestroyTexture(l_texture);
        if (l_surface)
            SDL_FreeSurface(l_surface);
    }

bool AppTerminalOutput::event(SDL_Event *ev, const void *instance)
    {
        AppTerminalOutput *ato = static_cast<AppTerminalOutput*>(
                const_cast<void*>(instance)
            );

        if (!ato)
            return false;

        if (!guiBase::IsActive())
            return false;

        if (ev->type == SDL_MOUSEMOTION)
        {
            if (!ato->guiBase::IsRegion(ev, ato->guiBase::GetGui<SDL_Rect>()))
                return false;
            ato->guiBase::PushEvent(ID_CMD_POP_MENU25);
            return true;
        }
        return false;
    }
