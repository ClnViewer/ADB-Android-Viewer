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

bool AppBrowserList::uevent(SDL_Event *ev, const void *instance)
    {
        AppBrowserList *abl = static_cast<AppBrowserList*>(
                const_cast<void*>(instance)
            );

        if (!abl)
            return false;

        switch (ev->user.code)
        {
            case ID_CMD_POP_MENU105:
            case ID_CMD_POP_MENU106:
            case ID_CMD_POP_MENU107:
                break;
            default:
                return false;
        }
        ///
        bool l_isselect;
        AppBrowserList::ListPosition *src;
        ///
        if (!(src = abl->selectsource()))
            return true;

        l_isselect = (
            (src->sel >= 0) &&
            (static_cast<int32_t>(abl->m_drawitems.size()) > src->sel));
        ///
        switch (ev->user.code)
        {
            case ID_CMD_POP_MENU105:
                {
                    if (m_isclick)
                        return true;
                    m_isclick = true;
                    ///
                    std::vector<AppBrowserPage::DrawItem> vdi;
                    std::string sroot = ((l_isselect) ?
                                abl->m_drawitems[src->sel].cmds :
                                AppConfig::instance().cnf_browser_dir_local
                             );

                    if (abl->m_parser.dir_list_local(sroot, vdi))
                    {
                        if (l_isselect)
                            AppConfig::instance().cnf_browser_dir_local = sroot;

                        src->Default();
                        abl->draw(vdi, src);
                    }
                    else
                    {
                        abl->m_drawitems.clear();
                        src->Default();
                        abl->draw(src);
                    }
                    m_isclick = false;
                    return true;
                }
            case ID_CMD_POP_MENU106:
                {
                    if (m_isclick)
                        return true;
                    m_isclick = true;
                    ///
                    std::vector<AppBrowserPage::DrawItem> vdi;
                    std::string s;
                    std::string cmds = "ls -l ";
                    std::string sroot = ((l_isselect) ?
                                abl->m_drawitems[src->sel].cmds :
                                AppConfig::instance().cnf_browser_dir_device
                             );
                    cmds += sroot;

                    if (!AppConfig::instance().cnf_adb.SendToShell(cmds, s))
                        break;

                    if (s.empty())
                        break;

                    if (abl->m_parser.dir_list_device(s, vdi, sroot))
                    {
                        if (l_isselect)
                            AppConfig::instance().cnf_browser_dir_device = sroot;

                        abl->draw(vdi, src);
                    }
                    m_isclick = false;
                    return true;
                }
            case ID_CMD_POP_MENU107:
                {
                    if (m_isclick)
                        return true;
                    m_isclick = true;
                    ///
                    do
                    {
                        std::string s;
                        std::string cmds;

                        switch (abl->m_page->mskey)
                        {
                            case AppBrowserPage::MenuKey::MENUKEY_APK:
                                {
                                    cmds = "pm list packages -f";
                                    switch (abl->m_page->mikey)
                                    {
                                        case AppBrowserPage::MenuInput::MENUINPUT_APK_ENABLED:
                                                cmds += " -e"; break;
                                        case AppBrowserPage::MenuInput::MENUINPUT_APK_DISABLED:
                                                cmds += " -d"; break;
                                        case AppBrowserPage::MenuInput::MENUINPUT_APK_SYSTEM:
                                                cmds += " -s"; break;
                                        case AppBrowserPage::MenuInput::MENUINPUT_APK_BRAND:
                                                cmds += " -3"; break;
                                        default:
                                                break;
                                    }
                                    break;
                                }
                            case AppBrowserPage::MenuKey::MENUKEY_INFO:
                                    cmds = "getprop"; break;
                            default:
                                break;
                        }

                        if (cmds.empty())
                            break;

                        if (!AppConfig::instance().cnf_adb.SendToShell(cmds, s))
                            break;

                        if (s.empty())
                            break;

                        switch (abl->m_page->mskey)
                        {
                            case AppBrowserPage::MenuKey::MENUKEY_APK:
                                {
                                    abl->draw(s,
                                        [=](std::string const & str, AppBrowserPage::DrawItem & di)
                                        {
                                            return abl->m_parser.apk_name(str, di);
                                        }
                                    );
                                    break;
                                }
                            case AppBrowserPage::MenuKey::MENUKEY_INFO:
                                {
                                    abl->draw(s,
                                        [=](std::string const & str, AppBrowserPage::DrawItem & di)
                                        {
                                            return abl->m_parser.inf_name(str, di);
                                        }
                                    );
                                    break;
                                }
                            default:
                                    break;
                        }
                        m_isclick = false;
                        return true;
                    }
                    while (0);

                    abl->m_drawitems.clear();
                    src->Default();
                    abl->draw(src);

                    m_isclick = false;
                    return true;
                }
        }
        return false;
    }

bool AppBrowserList::event(SDL_Event *ev, const void *instance)
    {
        AppBrowserList *abl = static_cast<AppBrowserList*>(
                const_cast<void*>(instance)
            );

        if (!abl)
            return false;

        if (!guiBase::IsActive())
            return false;

        if (ev->type == SDL_MOUSEMOTION)
        {
            if (!abl->guiBase::IsRegion(ev, abl->guiBase::GetGui<SDL_Rect>()))
                return false;
            abl->guiBase::PushEvent(ID_CMD_POP_MENU26);
            return true;
        }
        return false;
    }
