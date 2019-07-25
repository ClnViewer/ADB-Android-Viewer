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

bool AppTerminalPage::getwin(SDL_Window *sdlw)
    {
        do
        {
            if (!sdlw)
                break;

            SDL_Point & point_default = AppConfig::instance().cnf_disp_point;
            SDL_Point point_img_close = ResManager::imagesize(
                    ResManager::IndexImageResource::RES_IMG_TERMCLOSE
                );
            SDL_Point point_img_edit = ResManager::imagesize(
                    ResManager::IndexImageResource::RES_IMG_EDITMENU
                );
            SDL_Point point_display = guiPage::getdisplay();
            if ((point_display.x < 0) && (point_display.y < 0))
                    break;

            int32_t input_offset = (((point_img_close.y / 3) * 2) + 2);

            if (AppConfig::instance().cnf_isfullscreen)
                rbase.h = point_display.y;
            else
            {
                SDL_Rect rect = guiPage::getwindow(sdlw);
                if ((!rect.x) && (!rect.y) && (!rect.w) && (!rect.h))
                    break;

                rbase.h = point_display.y - rect.y - AppConfig::instance().cnf_term_bottom_pad;
            }

            rbase.w = point_default.x;
            rbase.x = point_img_close.x;
            rbase.y = point_default.y;
            /// image button close
            btn_r_close.x = 0;
            btn_r_close.y = AppConfig::instance().cnf_disp_point.y;
            /// image button edit
            btn_r_edit.x = 0;
            btn_r_edit.y = (rbase.h - point_img_edit.y);
            /// class input (Point)
            c_p_input.x = rbase.x;
            c_p_input.y = (rbase.h - input_offset);
            /// class output
            c_p_output.w = 0;
            c_p_output.h = 0;
            c_p_output.x = 0;
            c_p_output.y = 0;
            /// Out page:
            /// Out page - terminal text frame destination rectangle
            out_rdst.w = (rbase.w - point_img_close.x);
            out_rdst.h = (c_p_input.y - AppConfig::instance().cnf_disp_point.y - 16);
            out_rdst.x = point_img_close.x;
            out_rdst.y = rbase.y;
            /// Out page - terminal text frame source rectangle
            out_rsrc.w = out_rdst.w;
            out_rsrc.h = out_rdst.h;
            out_rsrc.x = out_rsrc.y = 0;
            /// Out page - terminal surface/texture size
            out_ssize.x = out_rdst.w;
            out_ssize.y = (out_rdst.h * SCROLL_NUM_SCREEN);
                /// limit SDL texture size 8192x8192
            out_ssize.y = ((out_ssize.y > 8192) ? 8192 : out_ssize.y);
            /// Out page - position scroll
            out_pos.x = out_pos.y = 0;
                /// max page, normalize number from limit SDL texture size
                /// using in AppTerminalOutput::pageDown() to check limit
            out_mpage = ((out_ssize.y / out_rdst.h) - 1);
            out_npage = 0;
            out_tpage = 0;
            /// class page number - informer
            c_p_pnum.x = (AppConfig::instance().cnf_disp_point.x - 32);
            c_p_pnum.y = AppConfig::instance().cnf_disp_point.y;

            return true;
        }
        while (0);

        return false;
    }

bool AppTerminalPage::init(SDL_Window *sdlw)
    {
        return getwin(sdlw);
    }

std::string AppTerminalPage::getprompt()
    {
        do
        {
            if (!AppConfig::instance().cnf_adb.IsDeviceID())
                break;

            std::stringstream ss;
            std::string sid = AppConfig::instance().cnf_adb.GetDeviceID<std::string>();
            if (sid.empty())
                break;

            ss << sid << AppTerminalPage::term_prompt;
            return ss.str();
        }
        while (0);

        return AppTerminalPage::term_prompt;
    }
