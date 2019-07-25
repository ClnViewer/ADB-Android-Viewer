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

#include "ADBViewer.h"

int main( int argc __ATTR_UNUSED, char * argv[])
{
#   if defined(_DEBUG)
    static const char *CrashLogFileName = "ADBViewer.rpt";
    ExcHndlInit();
    ExcHndlSetLogFileNameA(CrashLogFileName);
#   endif

    try
    {
        App app(argv[0]);
        app.loop();
    }
    catch(std::exception const & _ex)
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
            ),
            _ex.what(),
            nullptr
        );
    }
    catch(...)
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
            ),
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN,
                AppConfig::instance().cnf_lang
            ),
            nullptr
        );
    }
    return 0;
}
