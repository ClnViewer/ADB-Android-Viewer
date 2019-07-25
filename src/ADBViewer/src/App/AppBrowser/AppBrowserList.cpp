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

AppBrowserList::~AppBrowserList()
    {
        if (m_surface)
            SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }

bool AppBrowserList::init(App *app, AppBrowserPage *abp)
    {
        if ((!app) || (!abp))
            return false;

        m_app = app;
        m_page = abp;
        guiBase::gui.rdst = {};
        guiBase::gui.rsrc = nullptr;

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

            ret = ((m_color = ResManager::colorload(
                                ResManager::IndexColorResource::RES_COLOR_TERMINAL_TXT
                        )));
        }
        while (0);

        guiBase::ActiveOff();
        return ret;
    }

bool AppBrowserList::tinit(SDL_Texture **texture)
    {
        bool isact = guiBase::IsActive();
        if (isact)
            guiBase::ActiveOff();

        do
        {
            SDL_Rect *rect = guiBase::GetGui<SDL_Rect>();
            SDL_Point point = guiPage::getsize(*rect);

            if ((!rect->w) || (!rect->h))
                break;
            if (!guiBase::SurfaceInit(&m_surface, point, m_color[1], s_lock))
                break;
            if (m_drawitems.size())
                draw();
            else if (!guiBase::TextureInit(texture, m_surface, s_lock))
                return false;
        }
        while (0);

        if (isact)
            guiBase::ActiveOn();

        return true;
    }

bool AppBrowserList::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        if (tinit(texture))
            guiBase::ActiveOn();
        return guiBase::IsActive();
    }

void AppBrowserList::start()
    {
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();
        ::memcpy(&gr->rdst, &m_page->list_rdst, sizeof(SDL_Rect));

        m_list_step = (gr->rdst.h / 16);
        m_listpos_default.Default();
        m_listpos_files_local.Default();
        m_listpos_files_device.Default();

        if (!tinit(&gr->texture))
        {
            if (gr->texture)
                SDL_DestroyTexture(gr->texture);

            gr->texture = nullptr;
            return;
        }
        guiBase::ActiveOn();
        {
            draw(
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_BROWSER_START_BANNER,
                    AppConfig::instance().cnf_lang
                ));
        }
    }

void AppBrowserList::stop()
    {
        guiBase::ActiveOff();
        guiRenderer_s *gr = guiBase::GetGui<guiRenderer_s>();

        if (gr->texture)
            SDL_DestroyTexture(gr->texture);
        gr->texture = nullptr;

        if (m_surface)
        {
            std::lock_guard<std::mutex> l_lock(s_lock);
            SDL_FreeSurface(m_surface);
            m_surface = nullptr;
        }
    }

AppBrowserList::ListPosition * AppBrowserList::selectsource()
    {
        switch (m_page->mskey)
        {
            case AppBrowserPage::MenuKey::MENUKEY_INFO:
            case AppBrowserPage::MenuKey::MENUKEY_APK:
                    break;
            case AppBrowserPage::MenuKey::MENUKEY_FILE:
                {
                    switch (m_page->mikey)
                    {
                        case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                return &m_listpos_files_local;
                        case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                return &m_listpos_files_device;
                        default:
                                return nullptr;
                    }
                    break;
                }
            default:
                break;
        }
        return &m_listpos_default;
    }

void AppBrowserList::clickselectfile(AppBrowserPage::DrawItem const & di, int32_t id)
    {
        switch (di.type)
        {
            case AppBrowserPage::FileType::FILETYPE_DIR:
            case AppBrowserPage::FileType::FILETYPE_ROOT:
            case AppBrowserPage::FileType::FILETYPE_BACK:
                {
                    guiBase::PushEvent(id);
                    break;
                }
            case AppBrowserPage::FileType::FILETYPE_FILE:
                {
                    /// Todo: menu
                    break;
                }
            default:
                break;
        }
    }

void AppBrowserList::lineSelect(SDL_Event *ev)
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        SDL_Rect *rect = guiBase::GetGui<SDL_Rect>();
        int32_t  l_max = static_cast<int32_t>(m_drawitems.size()),
                 l_pad = (l_max - ((l_max / m_list_step) * m_list_step)),
                 l_pos = ((src->idx > 0) ? (src->idx - 1) : 0),
                 l_sel = (ev->motion.y - rect->y);
        bool     l_isstart = (l_pos <= m_list_step),
                 l_isend = (l_pos >= (l_max - l_pad - 1));

        /*
        FILE *fp = fopen("__selcted_in__.txt", "a+");
        fprintf(fp, "- _idx:[%d] _sel:[%d] _step:[%d] (motion:%d end:%d max:%d pos:%d pad:%d) [%d] (%d/%d)\n",
                src->idx, src->sel, m_list_step,
                (ev->motion.y - rect->y),
                (l_max - m_list_step),
                l_max, l_pos, l_pad,
                (l_max % m_list_step),
                l_isstart, l_isend
            );
        fclose(fp);
        */

        if (l_isstart)
        {
            src->idx = 0;
            src->sel = (l_sel / 16);
        }
        else if (l_isend)
        {
            //src->idx = ((l_pos - l_pad) - 1); // ?
            src->idx = ((l_pos - m_list_step) + 1);
            src->sel = ((l_sel / 16) + src->idx);
        }
        else
        {
            src->idx = (l_pos - m_list_step);
            src->sel = ((l_sel / 16) + src->idx);
        }

        draw(src);

        if ((src->sel < 0) || (l_max <= src->sel))
            return;

        if (ev->button.clicks == 1)
        {
            if (!m_drawitems[src->sel].desc.empty())
                AddMessageQueue(
                    m_drawitems[src->sel].desc.c_str(),
                    10U,
                    -1
                );
        }
        else if (ev->button.clicks > 1)
        {
            switch (m_page->mskey)
            {
                case AppBrowserPage::MenuKey::MENUKEY_INFO:
                    {
                        /// Todo: menu
                        break;
                    }
                case AppBrowserPage::MenuKey::MENUKEY_APK:
                    {
                        if (m_drawitems[src->sel].cmds.empty())
                            break;
                        /// Todo: menu
                        break;
                    }
                case AppBrowserPage::MenuKey::MENUKEY_FILE:
                    {
                        if (m_drawitems[src->sel].cmds.empty())
                            break;

                        switch (m_page->mikey)
                        {
                            case AppBrowserPage::MenuInput::MENUINPUT_PC:
                                {
                                    clickselectfile(m_drawitems[src->sel], ID_CMD_POP_MENU105);
                                    break;
                                }
                            case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                                {
                                    clickselectfile(m_drawitems[src->sel], ID_CMD_POP_MENU106);
                                    break;
                                }
                            default:
                                break;
                        }
                        break;
                    }
                default:
                        break;
            }
        }
    }

void AppBrowserList::lineUp()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        src->idx = (src->idx - m_list_step - 2);
        src->idx = ((src->idx >= 0) ? src->idx : 0);
        draw(src);
    }

void AppBrowserList::lineDown()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        int32_t l_sz  = static_cast<int32_t>(m_drawitems.size());
        if (m_list_step > l_sz)
            return;

        src->idx = (src->idx - m_list_step);
        src->idx = ((src->idx >= 0) ?
                src->idx : (l_sz - m_list_step)
            );
        draw(src);
    }

void AppBrowserList::pageUp()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        src->idx = (src->idx - (m_list_step * 2));
        src->idx = ((src->idx >= 0) ? src->idx : 0);
        draw(src);
    }

void AppBrowserList::pageDown()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        int32_t l_sz = static_cast<int32_t>(m_drawitems.size());
        if (m_list_step > l_sz)
            return;

        src->idx = (src->idx + m_list_step);
        src->idx = ((src->idx >= l_sz) ?
                      (((l_sz - m_list_step) < 0) ? 0 : (l_sz - m_list_step)) :
                      src->idx
            );
        draw(src);
    }

void AppBrowserList::pageHome()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        src->idx = 0;
        draw(src);
    }

void AppBrowserList::pageEnd()
    {
        if (!guiBase::IsActive())
            return;

        AppBrowserList::ListPosition *src;
        if (!(src = selectsource()))
            return;

        int32_t l_sz = static_cast<int32_t>(m_drawitems.size());
        if (m_list_step > l_sz)
            return;

        src->idx = (l_sz - m_list_step);
        src->idx = ((src->idx < 0) ? 0 : src->idx);
        draw(src);
    }

void AppBrowserList::listselect(AppBrowserList::ListPosition *src)
    {
        do
        {
            if (src->sel < 0)
                break;

            if (static_cast<int32_t>(m_drawitems.size()) <= src->sel)
                break;

            if (!((src->sel >= src->idx) &&
                (src->sel <= (src->idx + m_list_step - 1))))
                break;

            return;
        }
        while (0);

        src->sel = -1;
    }
