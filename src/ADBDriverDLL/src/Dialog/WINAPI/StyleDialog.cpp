
#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0501

#include "../../DriverInternal.h"
#include "StyleDialog.h"


namespace GameDev
{

INT_PTR StyleDialog::ColorDialog()
{
    return (INT_PTR)BrushBg();
}

INT_PTR StyleDialog::ColorStatic(HDC hdc)
{
    ::SetTextColor(hdc, RGBColorTxt());
    ::SetBkMode(hdc, TRANSPARENT);
    return (INT_PTR)BrushBg();
}

INT_PTR StyleDialog::ColorStatic(HDC hdc, COLORREF clrref)
{
    ::SetTextColor(hdc, clrref);
    ::SetBkMode(hdc, TRANSPARENT);
    return (INT_PTR)BrushBg();
}

bool StyleDialog::IsDefaultStyle() const
{
    return _default;
}

HBRUSH StyleDialog::BrushBg() const
{
    return _hbrush;
}

COLORREF StyleDialog::RGBColorBg() const
{
    return _crefB;
}

COLORREF StyleDialog::RGBColorTxt() const
{
    return _crefT;
}

COLORREF StyleDialog::RGBColorTxtDefault() const
{
    return __STYLE_METRO_TXT;
}

COLORREF StyleDialog::RGBColorBgDefault() const
{
    return __STYLE_METRO_BG;
}

COLORREF StyleDialog::RGBColorBgButtonDefault() const
{
    return __STYLE_METRO_BGBTND;
}

COLORREF StyleDialog::RGBColorBgButtonLightDefault() const
{
    return __STYLE_METRO_BGBTNL;
}

COLORREF StyleDialog::RGBColorTxtButtonDefault() const
{
    return __STYLE_METRO_BGBTNT;
}

void StyleDialog::Set(COLORREF creft, COLORREF crefb)
{
    if (creft)
    {
        _crefT = creft;
        _default = false;
    }
    else
        _crefT = RGBColorTxtDefault();

    if (crefb)
    {
        _hbrush = CreateSolidBrush(crefb);
        _crefB = crefb;
        _default = false;
    }
    else
    {
        _hbrush = CreateSolidBrush(RGBColorBgDefault());
        _crefB = RGBColorBgDefault();
    }
}

StyleDialog::StyleDialog()
 : _crefT(RGBColorTxtDefault()), _crefB(RGBColorBgDefault()),
   _hbrush(CreateSolidBrush(RGBColorBgDefault())), _default(true) {}

StyleDialog::StyleDialog(COLORREF creft, COLORREF crefb)
 : _default(true)
{
    Set(creft, crefb);
}

StyleDialog::~StyleDialog()
{
    if (_hbrush)
        DeleteObject(_hbrush);
    _hbrush = nullptr;
}

}


