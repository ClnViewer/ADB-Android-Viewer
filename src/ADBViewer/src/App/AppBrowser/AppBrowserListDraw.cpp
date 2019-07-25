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

#include "../../ADBViewer.h"

void AppBrowserList::draw()
    {
        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        src->Default();
        draw(src);
    }

void AppBrowserList::draw(std::vector<AppBrowserPage::DrawItem> & v)
    {
        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        draw(v, src);
    }

void AppBrowserList::draw(std::vector<AppBrowserPage::DrawItem> & v, AppBrowserList::ListPosition *src)
    {
        if (!src)
            return;

        m_drawitems.clear();
        m_drawitems.assign(v.begin(), v.end());

        src->Default();
        draw(src);
    }

void AppBrowserList::draw(std::string const & s)
    {
        draw(s, nullptr);
    }

void AppBrowserList::draw(std::string const & s, std::function<bool(std::string const &, AppBrowserPage::DrawItem &)> fun)
    {
        if (s.empty())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        src->Default();
        m_drawitems.clear();

        do
        {
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
                {
                    if (fun)
                    {
                        AppBrowserPage::DrawItem di(
                            m_color[0],
                            m_color[1]
                        );
                        if (fun(ss, di))
                            m_drawitems.push_back(di);
                    }
                    else
                    {
                        AppBrowserPage::DrawItem di(
                            ss,
                            m_color[0],
                            m_color[1]
                        );
                        m_drawitems.push_back(di);
                    }
                }
                prev = pos + found.length();
            }
            while (pos < s.length() && prev < s.length());

            if (!m_drawitems.size())
                break;

            draw(src);
        }
        while (0);
    }

void AppBrowserList::draw(AppBrowserList::ListPosition *src)
    {
        if (!src)
            return;

        if ((!m_drawitems.size()) || (static_cast<int32_t>(m_drawitems.size()) <= src->idx))
            return;

        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();
        SDL_Point l_sursize = guiPage::getsize(gr->rdst);
        if (!guiBase::SurfaceInit(&m_surface, l_sursize, m_color[1], s_lock))
            return;

        listselect(src);

        int32_t l_start = src->idx,
                l_end   = (src->idx + m_list_step),
                l_size  = static_cast<int32_t>(m_drawitems.size());

        for (; ((src->idx <= l_end) && (src->idx < l_size)); src->idx++)
        {
            AppBrowserPage::DrawItem di = m_drawitems[src->idx];
            if (src->sel == src->idx)
                std::swap(di.ctxt, di.cbg);
            drawline(di, ((src->idx - l_start) * 16));
        }

        if (guiBase::TextureInit(&gr->texture, m_surface, s_lock))
            guiBase::ActiveOn();
        else
        {
            if (gr->texture)
            {
                GuiLock(
                    SDL_DestroyTexture(gr->texture);
                    gr->texture = nullptr;
                );
            }
            guiBase::ActiveOff();
        }
    }

void AppBrowserList::drawline(AppBrowserPage::DrawItem const & dis, int32_t pos)
    {
        SDL_Surface *l_surface = nullptr;
        SDL_Surface *t_surface = nullptr;

        do
        {
            std::lock_guard<std::mutex> l_lock(s_lock);

            if (!m_surface)
                break;

            if (!(t_surface = TTF_RenderUTF8_Shaded(
                                m_font,
                                dis.s.c_str(),
                                dis.ctxt,
                                dis.cbg
                            )))
                break;

            SDL_Point point = { m_page->list_rdst.w, t_surface->h };
            SDL_Rect  drect{};

            if (!guiBase::SurfaceInit(&l_surface, point, dis.cbg))
                break;

            drect.x = 0;
            drect.y = 0;
            drect.w = t_surface->w;
            drect.h = t_surface->h;

            if (SDL_BlitSurface(t_surface, nullptr, l_surface, &drect))
                break;

            drect.x = 0;
            drect.y = pos;
            drect.w = l_surface->w;
            drect.h = l_surface->h;

            SDL_BlitSurface(l_surface, nullptr, m_surface, &drect);
        }
        while (0);

        if (l_surface)
            SDL_FreeSurface(l_surface);
        if (t_surface)
            SDL_FreeSurface(t_surface);
    }
