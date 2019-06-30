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

/*
#include <curses.h>
PDCEX SDL_Window  *pdc_window = nullptr;
PDCEX SDL_Surface *pdc_screen = nullptr;
PDCEX int32_t      pdc_yoffset = 0;
*/

bool AppTerminal::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        //pdc_window = m_app->m_window;

        return guiBase::initgui(app);
    }

bool AppTerminal::tinit(SDL_Texture **texture)
    {
        /*
        guiBase::gui.rect.w = AppConfig::instance().cnf_disp_point.x;
        guiBase::gui.rect.h = 300;
        guiBase::gui.rect.x = 0;
        guiBase::gui.rect.y = AppConfig::instance().cnf_disp_point.y;

        if (!pdc_screen)
            pdc_screen = SDL_CreateRGBSurface(
                0U,
                guiBase::gui.rect.w,
                guiBase::gui.rect.h,
                16,
#               if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff
#               else
                0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
#               endif
            );
        if (!pdc_screen)
            return false;

        SDL_Texture *l_texture = SDL_CreateTextureFromSurface(
                guiBase::getgui()->m_renderer,
                pdc_screen
            );
        if (!l_texture)
            return false;

        GuiLock(
            std::swap(*texture, l_texture);
        );

        if (l_texture)
            SDL_DestroyTexture(l_texture);
        */

        return true;
    }

bool AppTerminal::evresize(SDL_Texture **texture)
    {
        if ((!texture) || (!*texture))
            return false;

        guiBase::ActiveOff();
        if (tinit(texture))
            guiBase::ActiveOn();
        return guiBase::IsActive();
    }

bool AppTerminal::event(SDL_Event *ev, const void *instance)
{
    AppTerminal *asc = static_cast<AppTerminal*>(
                const_cast<void*>(instance)
            );

    if (ev->type == AppConfig::instance().cnf_uevent)
    {
        switch(ev->user.code)
        {
            /// x
            case ID_CMD_POP_MENU4:
                {
                    return false;
                }
        }
    }
    return false;
}

