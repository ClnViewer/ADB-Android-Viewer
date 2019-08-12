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

bool AppBrowser::mevent9(SDL_Event *ev, SDL_Rect *r)
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
                    switch (m_page.mskey)
                    {
                        case AppBrowserPage::MenuKey::MENUKEY_NONE:
                        case AppBrowserPage::MenuKey::MENUKEY_INFO:
                            {
                                guiBase::PushEvent(ID_CMD_POP_MENU26);
                                break;
                            }
                        case AppBrowserPage::MenuKey::MENUKEY_FILE:
                            {
                                mevent9motion_file(pos);
                                guiBase::PushEvent(ID_CMD_POP_MENU27);
                                break;
                            }
                        case AppBrowserPage::MenuKey::MENUKEY_APK:
                            {
                                mevent9motion_apk(pos);
                                guiBase::PushEvent(ID_CMD_POP_MENU27);
                                break;
                            }
                        default:
                            break;
                    }
                    return true;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch (m_page.mskey)
                    {
                        case AppBrowserPage::MenuKey::MENUKEY_NONE:
                        case AppBrowserPage::MenuKey::MENUKEY_INFO:
                            {
                                break;
                            }
                        case AppBrowserPage::MenuKey::MENUKEY_FILE:
                            {
                                mevent9button_file(pos);
                                guiBase::PushEvent(ID_CMD_POP_MENU27);
                                break;
                            }
                        case AppBrowserPage::MenuKey::MENUKEY_APK:
                            {
                                mevent9button_apk(pos);
                                guiBase::PushEvent(ID_CMD_POP_MENU27);
                                break;
                            }
                        default:
                            break;
                    }
                    return true;
                }

        }
        return false;
    }

void AppBrowser::mevent9motion_file(int32_t pos)
    {
        ResManager::IndexStringResource idx = ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN;
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 54   ... 154:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_FILE_ANDROID;
                    break;
                }
            case 156   ... 198:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_FILE_PC;
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

void AppBrowser::mevent9button_file(int32_t pos)
    {
        uint32_t m9 = 0U;
        AppBrowserPage::MenuInput key = m_page.mikey;

#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 54   ... 154:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_ANDROID;
                    m9 = 2U;
                    break;
                }
            case 156   ... 198:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_PC;
                    m9 = 1U;
                    break;
                }
            default:
                    break;
        }
#       pragma GCC diagnostic pop

        if (key != m_page.mikey)
        {
            m_page.mikey = key;
            m_icon_menu9.On(m9);

            switch (key)
            {
                case AppBrowserPage::MenuInput::MENUINPUT_PC:
                    {
                        /// read local drive..
                        guiBase::PushEvent(ID_CMD_POP_MENU108);
                        break;
                    }
                case AppBrowserPage::MenuInput::MENUINPUT_ANDROID:
                    {
                        /// read device file system..
                        guiBase::PushEvent(ID_CMD_POP_MENU109);
                        break;
                    }
                default:
                    break;
            }
        }
    }

void AppBrowser::mevent9motion_apk(int32_t pos)
    {
        ResManager::IndexStringResource idx = ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN;
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 54   ... 84:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_APK_E;
                    break;
                }
            case 90   ... 121:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_APK_D;
                    break;
                }
            case 128   ... 158:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_APK_S;
                    break;
                }
            case 165   ... 195:
                {
                    idx = ResManager::IndexStringResource::RES_STR_BROWSER_M9_APK_O;
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

void AppBrowser::mevent9button_apk(int32_t pos)
    {
        uint32_t m9 = 0U;
        AppBrowserPage::MenuInput key = m_page.mikey;

#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wpedantic"
        switch (pos)
        {
            case 54   ... 84:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_APK_ENABLED;
                    m9 = 5U;
                    break;
                }
            case 90   ... 121:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_APK_DISABLED;
                    m9 = 6U;
                    break;
                }
            case 128   ... 158:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_APK_SYSTEM;
                    m9 = 7U;
                    break;
                }
            case 165   ... 195:
                {
                    key = AppBrowserPage::MenuInput::MENUINPUT_APK_BRAND;
                    m9 = 8U;
                    break;
                }
            default:
                    break;
        }
#       pragma GCC diagnostic pop

        if (key != m_page.mikey)
        {
            m_page.mikey = key;
            m_icon_menu9.On(m9);

            /// push command to ADB
            guiBase::PushEvent(ID_CMD_POP_MENU107);
        }
    }
