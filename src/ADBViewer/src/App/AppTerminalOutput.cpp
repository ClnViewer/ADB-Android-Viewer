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


bool AppTerminalOutput::init(App *app, SDL_Rect & rect)
    {
        if (!app)
            return false;

        m_app = app;
        guiBase::gui.rect = rect;

        bool ret = guiBase::initgui(app);
        guiBase::ActiveOff();
        return ret;
    }

bool AppTerminalOutput::tinit(SDL_Texture **texture)
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
                guiBase::GetGui<SDL_Renderer>(),
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
        if (!guiBase::gui.texture)
            if (!tinit(&guiBase::gui.texture))
                return;
        guiBase::ActiveOn();
    }

void AppTerminalOutput::stop()
    {
        guiBase::ActiveOff();

        if (guiBase::gui.texture)
            SDL_DestroyTexture(guiBase::gui.texture);

        guiBase::gui.texture = nullptr;
    }

void AppTerminalOutput::draw(std::string const & s)
    {
    }

