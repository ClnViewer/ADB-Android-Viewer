
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_NOTIFY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) hWnd;
    (void) uMsg;
    (void) wParam;

    switch (((LPNMHDR)lParam)->code)
    {
    case NM_DBLCLK:
        {
            if (_sel > -1)
            {
                ::EndDialog(hWnd, _sel);
                break;
            }
            return 0;
        }
    case LVN_ITEMCHANGED:
        {

        NMLISTVIEW *nlv = reinterpret_cast<NMLISTVIEW*>(lParam);
        if (nlv)
            if (
                (nlv->hdr.code == LVN_ITEMCHANGED) &&
                (nlv->hdr.idFrom == DLG_LISTV) &&
                (nlv->uNewState & LVIS_SELECTED) &&
                ((_sel = ListView_GetNextItem(_hwnd, -1, LVNI_ALL | LVNI_SELECTED)) >= 0)
            )
            {
                LVITEMW lvi{};
                _key.clear(); _key.resize(100);
                _val.clear(); _val.resize(100);

                lvi.iSubItem = 0;
                lvi.cchTextMax = 100;
                lvi.pszText = (LPWSTR) _key.data();
                SendMessageW(_hwnd, LVM_GETITEMTEXTW, (WPARAM)_sel, (LPARAM) (LVITEMW*)&lvi);

                lvi.iSubItem = 1;
                lvi.cchTextMax = 100;
                lvi.pszText = (LPWSTR) _val.data();
                SendMessageW(_hwnd, LVM_GETITEMTEXTW, (WPARAM)_sel, (LPARAM) (LVITEMW*)&lvi);

                SetDlgItemTextW(hWnd, DLG_LISTI, static_cast<LPCWSTR>(_val.c_str()));
            }

        break;
        }
        default:
            break;
    }

  return 1;
}

}
