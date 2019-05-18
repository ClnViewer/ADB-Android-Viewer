
#include "../AdbListDialogInternal.h"

namespace GameDev
{

INT_PTR AdbListDialog::_OnWM_DRAWITEM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    (void) hWnd;
    (void) uMsg;
    (void) wParam;

            const DRAWITEMSTRUCT *di = (DRAWITEMSTRUCT*)lParam;

            if ((!di) || (di->CtlType != ODT_BUTTON))
                return 0;

            SIZE sz;
            std::wstring wtxt{}; wtxt.resize(100);
            int tsz = ::GetWindowTextW(di->hwndItem, wtxt.data(), wtxt.size());
            wtxt.resize(tsz);
            ::GetTextExtentPoint32W(di->hDC, wtxt.c_str(), wtxt.length(), &sz);

            if (di->itemState & ODS_SELECTED)
            {
                (void) ::SetTextColor(
                            di->hDC,
                            ((_style.IsDefaultStyle()) ?
                                _style.RGBColorTxtButtonDefault() : _style.RGBColorBg()
                             )
                        );
                (void) ::SetBkColor(
                            di->hDC,
                            ((_style.IsDefaultStyle()) ?
                                _style.RGBColorBgButtonLightDefault() : _style.RGBColorTxt()
                             )
                        );
            }
            else
            {
                (void) ::SetTextColor(
                            di->hDC,
                            ((_style.IsDefaultStyle()) ?
                                _style.RGBColorTxtButtonDefault() : _style.RGBColorTxt()
                             )
                        );
                (void) ::SetBkColor(
                            di->hDC,
                            ((_style.IsDefaultStyle()) ?
                                _style.RGBColorBgButtonDefault() : _style.RGBColorBg()
                             )
                        );
            }

            ::ExtTextOutW(
                di->hDC,
                (((di->rcItem.right - di->rcItem.left) - sz.cx) / 2),
                (((di->rcItem.bottom - di->rcItem.top) - sz.cy) / 2),
                ETO_OPAQUE | ETO_CLIPPED,
                &di->rcItem,
                wtxt.c_str(),
                wtxt.length(),
                NULL
            );
            ::DrawEdge(
                di->hDC,
                const_cast<LPRECT>(&di->rcItem),
                (di->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED),
                BF_RECT
            );

            /// Focus border set
            /*
            if (di->itemState & ODS_SELECTED)
                DrawFocusRect(di->hDC, &di->rcItem);
            */

  return 1;
}

}
