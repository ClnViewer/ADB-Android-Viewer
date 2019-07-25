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

bool AppBrowser::mevent4(SDL_Event *ev, SDL_Rect *r)
    {
        int32_t pos = 0;

        switch (ev->type)
        {
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
                {
                    pos = (ev->motion.x - r->x);
                }
        }
        switch (ev->type)
        {
            case SDL_MOUSEMOTION:
                {
                    mevent4motion(pos);
                    guiBase::PushEvent(ID_CMD_POP_MENU27);
                    return true;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    mevent4button(pos);
                    return true;
                }

        }
        return false;
    }

void AppBrowser::mevent4motion(int32_t pos)
    {
        ResManager::IndexStringResource idx = ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN;
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 1   ... 32:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M4_FILE;
                    break;
                }
            case 38   ... 68:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M4_INFO;
                    m_page.mikey = AppBrowserPage::MenuInput::MENUINPUT_NONE;
                    break;
                }
            case 76   ... 106:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M4_APK;
                    m_page.mikey = AppBrowserPage::MenuInput::MENUINPUT_NONE;
                    break;
                }
            default:
                    break;
        }
#       pragma GCC diagnostic pop

        if (idx != ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN)
            AddMessageQueue(
                ResManager::stringload(
                    idx,
                    AppConfig::instance().cnf_lang
                ),
                5U,
                -1
            );
    }

void AppBrowser::mevent4button(int32_t pos)
    {
        uint32_t m4 = 0U, m9 = 0U;
        AppBrowserPage::MenuKey key = m_page.mskey;
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 1   ... 32:
                {
                    key = AppBrowserPage::MenuKey::MENUKEY_FILE;
                    m4 = 1U;
                    m9 = 1U;
                    break;
                }
            case 38   ... 68:
                {
                    key = AppBrowserPage::MenuKey::MENUKEY_INFO;
                    m4 = 2U;
                    m9 = 3U;
                    break;
                }
            case 76   ... 106:
                {
                    key = AppBrowserPage::MenuKey::MENUKEY_APK;
                    m4 = 3U;
                    m9 = 4U;
                    break;
                }
            default:
                    break;
        }
#       pragma GCC diagnostic pop

        if (key == m_page.mskey)
            return;

        m_page.mskey = key;
        m_icon_menu4.On(m4);
        m_icon_menu9.On(m9);

        /// push event ?
        switch (key)
        {
            case AppBrowserPage::MenuKey::MENUKEY_APK:
                {
                    m_page.mikey = AppBrowserPage::MenuInput::MENUINPUT_NONE;
                    break;
                }
            case AppBrowserPage::MenuKey::MENUKEY_FILE:
                {
                    m_page.mikey = AppBrowserPage::MenuInput::MENUINPUT_PC;
                    guiBase::PushEvent(ID_CMD_POP_MENU105);
                    break;
                }
            case AppBrowserPage::MenuKey::MENUKEY_INFO:
                {
                    m_page.mikey = AppBrowserPage::MenuInput::MENUINPUT_NONE;
                    guiBase::PushEvent(ID_CMD_POP_MENU107);
                    break;
                }
            default:
                break;
        }
    }
