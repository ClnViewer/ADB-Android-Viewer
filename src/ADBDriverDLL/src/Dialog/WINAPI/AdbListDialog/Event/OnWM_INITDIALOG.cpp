
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_INITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) uMsg;
    (void) wParam;
    (void) lParam;

        std::wstring ws{};
        string_from_res<std::wstring> wres{};
        _hwnd = ::GetDlgItem(hWnd, DLG_LISTV);

        DWORD st =
            LVS_EX_FULLROWSELECT |
            LVS_EX_GRIDLINES |
            LVS_EX_BORDERSELECT |
            LVS_EX_DOUBLEBUFFER;
        ListView_SetExtendedListViewStyleEx(_hwnd, st, st);

        LVCOLUMNW col{};
        col.mask = LVCF_TEXT | LVCF_WIDTH;
        col.fmt = 0;

        col.cx = _p.x;
        ws = wres.go(_hinst, IDS_LSTV1).c_str();
        col.pszText = (LPWSTR) ws.c_str();
        SendMessageW(_hwnd, LVM_INSERTCOLUMNW, (WPARAM)((int)0), (LPARAM) (const LVCOLUMNW*)&col);

        col.cx = _p.y;
        ws = wres.go(_hinst, IDS_LSTV2).c_str();
        col.pszText = (LPWSTR) ws.c_str();
        SendMessageW(_hwnd, LVM_INSERTCOLUMNW, (WPARAM)((int)01), (LPARAM) (const LVCOLUMNW*)&col);

        LVITEMW lvi{};
        uint32_t i;

        for (i = 0U; i < _dict.size(); i++)
        {
            SendMessageW(_hwnd, LVM_INSERTITEMW, (WPARAM)0, (LPARAM) (const LVITEMW*)&lvi);
        }

        i = 0U;
        SelectedList::iterator it = _dict.begin();
        while(it != _dict.end())
        {
            memset(&lvi, 0, sizeof(LVITEMW));
            lvi.iSubItem = 0;
            lvi.pszText = (LPWSTR) ((_isreverse) ? it->second.c_str() : it->first.c_str());
            SendMessageW(_hwnd, LVM_SETITEMTEXTW, (WPARAM)i, (LPARAM) (const LVITEMW*)&lvi);

            lvi.iSubItem = 1;
            lvi.pszText = (LPWSTR) ((_isreverse) ? it->first.c_str() : it->second.c_str());
            SendMessageW(_hwnd, LVM_SETITEMTEXTW, (WPARAM)i, (LPARAM) (const LVITEMW*)&lvi);

            it++; i++;
        }
  return 1;
}

}
