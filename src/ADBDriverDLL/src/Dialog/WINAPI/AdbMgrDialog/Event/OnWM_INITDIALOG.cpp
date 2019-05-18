
#include "../AdbMgrDialogInternal.h"

namespace GameDev
{

INT_PTR AdbMgrDialog::_OnWM_INITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        ::SetDlgItemTextW(hWnd, DLG_ADBPATH, static_cast<LPCWSTR>(_adb->GetAdbBin()));

        if (::_waccess(_adb->GetAdbBin(), F_OK) < 0)
        {
            string_from_res<std::wstring> wres{};
            ::SetDlgItemTextW(hWnd, DLG_ADBINFO, static_cast<LPCWSTR>(
                                    wres.go(hmodule_get(), IDS_ERR3).c_str()
                                )
                            );
        }

  return 0;
}

}
