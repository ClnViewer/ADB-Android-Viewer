/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

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

#include "SCEditDll.h"
#include "SCEditInternal.h"

static Editor::SCEApp sce;
static HINSTANCE g_hInstance = nullptr;
static const char g_appname[] = "SCEdit";

bool DLL_EXPORT RunEdit(std::string const & path_, std::string const & text_, bool runtype, HICON hicon)
	{
		sce.init(path_, text_, g_hInstance, hicon);
		sce.appname(g_appname);
		return sce.run(runtype);
	}

bool DLL_EXPORT RunEditOpen(std::string const & path_, bool runtype, HICON hicon)
	{
		sce.init(path_, g_hInstance, hicon);
		sce.appname(g_appname);
		return sce.run(runtype);
	}

bool DLL_EXPORT RunEditEmpty(bool runtype, HICON hicon)
	{
		sce.init(g_hInstance, hicon);
		sce.appname(g_appname);
		return sce.run(runtype);
	}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            {
                g_hInstance = hinstDLL;
                //
                INITCOMMONCONTROLSEX ictrl{};
                ictrl.dwSize = sizeof(INITCOMMONCONTROLSEX);
                ictrl.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES;
                (void) ::InitCommonControlsEx(&ictrl);
                //
                break;
            }
        case DLL_PROCESS_DETACH:
            {
                g_hInstance = nullptr;
                break;
            }
        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
