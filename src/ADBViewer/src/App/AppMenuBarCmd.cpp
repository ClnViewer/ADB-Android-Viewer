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

static inline const LPCSTR l_openApkFilter = "APK files (*.apk)\0*.apk\0\0";
static inline const LPCSTR l_openApkExt = "apk";
static inline const LPCSTR l_openCurDir = ".\\";

int32_t AppMenuBar::mb_cmd_QUIT(SDL_Event *ev)
    {
        AppConfig::instance().cnf_isrun = false;
        return 0;
    }

int32_t AppMenuBar::mb_cmd_START(SDL_Event *ev)
    {
        if (!AppConfig::instance().cnf_isstop)
        {
            m_app->m_appmsgbar.PrintInfo(
                MgrType::MGR_MENU,
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_ADBCONNECTED,
                    AppConfig::instance().cnf_lang
                ),
                -1, ev
            );
            return 0;
        }
        if (!AppConfig::instance().cnf_adb.IsDeviceID())
            AppConfig::instance().cnf_adb.GetDeviceListUI();

        m_app->m_appmsgbar.PrintInfo(
            MgrType::MGR_MENU,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ADBCONNECT,
                AppConfig::instance().cnf_lang
                ),
                -1, ev
        );
        return ID_CMD_POP_MENU97;
    }

int32_t AppMenuBar::mb_cmd_STOP(SDL_Event *ev)
    {
        if (AppConfig::instance().cnf_isstop)
        {
            m_app->m_appmsgbar.PrintInfo(
                MgrType::MGR_MENU,
                ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_ADBDISCONNECTED,
                    AppConfig::instance().cnf_lang
                ),
                -1, ev
            );
            return 0;
        }
        m_app->m_appmsgbar.PrintInfo(
            MgrType::MGR_MENU,
                ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ADBDISCONNECT,
                AppConfig::instance().cnf_lang
            ),
            -1, ev
        );
        return ID_CMD_POP_MENU98;
    }

int32_t AppMenuBar::mb_cmd_ADBSET(SDL_Event *ev)
    {
        AppConfig::instance().cnf_adb.GetDeviceSetupUI();
        return 0;
    }

int32_t AppMenuBar::mb_cmd_SCALE(SDL_Event *ev)
    {
        AppConfig::instance().cnf_disp_ratio =
            ((AppConfig::instance().cnf_disp_ratio.load() >= 2U) ? 1U : 2U);
        guiBase::PushEvent(ID_CMD_POP_MENU25); /// close terminal is opened
        return ID_CMD_POP_MENU99;
    }

int32_t AppMenuBar::mb_cmd_POSINFO(SDL_Event *ev)
    {
        AppConfig::instance().cnf_ispos = !(AppConfig::instance().cnf_ispos);
        if (!AppConfig::instance().cnf_ispos)
            m_app->m_appmsgbar.PrintInfo(MgrType::MGR_MENU, "", -1, ev);
        return 0;
    }

int32_t AppMenuBar::mb_cmd_CAPTURE_D(SDL_Event *ev)
    {
        return ID_CMD_POP_MENU4;
    }

int32_t AppMenuBar::mb_cmd_FULLSCREEN(SDL_Event *ev)
    {
        SDL_SetWindowFullscreen(
            guiBase::GetGui<SDL_Window>(),
            ((AppConfig::instance().cnf_isfullscreen) ? 0U : SDL_WINDOW_FULLSCREEN_DESKTOP)
        );
        if (AppConfig::instance().cnf_isfullscreen)
        {
            SDL_SetWindowSize(
                guiBase::GetGui<SDL_Window>(),
                AppConfig::instance().cnf_disp_point.x,
                AppConfig::instance().cnf_disp_point.y
            );
            SDL_SetWindowPosition(
                guiBase::GetGui<SDL_Window>(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED
            );
        }
        AppConfig::instance().cnf_isfullscreen = !(AppConfig::instance().cnf_isfullscreen);
        guiBase::PushEvent(ID_CMD_POP_MENU25); /// close terminal is opened
        return 0;
    }

int32_t AppMenuBar::mb_cmd_APK(SDL_Event *ev)
    {
        std::string fname;
        if (AppSysDialog::openfile(
                            guiBase::GetGui<SDL_Window>(),
                            fname,
                            l_openApkFilter,
                            l_openApkExt,
                            l_openCurDir
                ))
            AppConfig::instance().cnf_adb.InstallApk(fname);
        return 0;
    }
