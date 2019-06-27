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


