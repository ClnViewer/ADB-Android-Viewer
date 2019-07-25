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

bool AppBrowserPage::getwin(SDL_Window *sdlw)
    {
        do
        {
            if (!sdlw)
                break;

            SDL_Point & point_default = AppConfig::instance().cnf_disp_point;
            SDL_Point point_menu4 = ResManager::spritesize(
                    ResManager::IndexSpriteResource::RES_SPRITE_BROWSER_MENU4
                );
            SDL_Point point_menu9 = ResManager::spritesize(
                    ResManager::IndexSpriteResource::RES_SPRITE_BROWSER_MENU9
                );
            SDL_Point point_display = guiPage::getdisplay();
            if ((point_display.x < 0) && (point_display.y < 0))
                    break;

            if (AppConfig::instance().cnf_isfullscreen)
            {
                rbase.h = frame.h = point_display.y;
                rbase.w = point_display.x;
                frame.w = point_display.x - point_default.x;
            }
            else
            {
                SDL_Rect rect = guiPage::getwindow(sdlw);
                if ((!rect.x) && (!rect.y) && (!rect.w) && (!rect.h))
                    break;

                rbase.h = frame.h = rect.h;
                rbase.w = point_display.x - rect.x;
                frame.w = point_display.x - rect.x - point_default.x;
            }

            rbase.x = frame.x = point_default.x;
            rbase.y = frame.y = 0;

            /// Out page:
            /// Elements:
                /// Minimum windows size
            minsize = point_menu9;
            minsize.x += 16;
                /// Menu mode element
            menu4.w = point_menu4.x;
            menu4.h = point_menu4.y;
            menu4.x = (rbase.w - point_menu4.x);
            menu4.y = 8;
                /// Menu banner element
            menu9.w = point_menu9.x;
            menu9.h = point_menu9.y;
            menu9.x = (rbase.w - point_menu9.x);
            menu9.y = (point_menu4.y + 16); /// offset 8 + 8
            /// List element:
            int32_t top_offset = (point_menu4.y + point_menu9.y + 16);
                /// List element - text frame destination rectangle
            list_rdst.w = (frame.w - 16);              /// pad -8 left, -8 right
            list_rdst.h = (frame.h - top_offset - 8);  /// pad -4 top, -4 bottom;
            list_rdst.x = (frame.x +                   /// pad +4 or +8 offset right
                           ((frame.w > (point_menu9.x + 50)) ? 8 : 4)
                    );
            list_rdst.y = (frame.y + top_offset + 4);  /// pad +4 offset top

            return true;
        }
        while (0);

        return false;
    }

bool AppBrowserPage::init(SDL_Window *sdlw)
    {
        return getwin(sdlw);
    }
