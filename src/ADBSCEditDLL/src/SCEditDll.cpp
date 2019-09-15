
#include "SCEditDll.h"
#include "SCEditInternal.h"

static Editor::SCEdit sc;
static HINSTANCE g_hInstance = nullptr;

bool DLL_EXPORT RunEdit(std::string const & path_, std::string const & text_, bool runtype, HICON hicon)
	{
		sc.Init(path_, text_, g_hInstance, hicon);
		return sc.Run(runtype);
	}

bool DLL_EXPORT RunEditOpen(std::string const & path_, bool runtype, HICON hicon)
	{
		sc.Init(path_, g_hInstance, hicon);
		return sc.Run(runtype);
	}

bool DLL_EXPORT RunEditEmpty(bool runtype, HICON hicon)
	{
		sc.Init(g_hInstance, hicon);
		return sc.Run(runtype);
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
                ictrl.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES; //ICC_STANDARD_CLASSES;
                (void) ::InitCommonControlsEx(&ictrl);
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
