
#pragma once

namespace GameDev
{

class StyleDialog
{
    private:
    COLORREF _crefT;
    COLORREF _crefB;
	HBRUSH _hbrush;
	bool _default;

    public:
	COLORREF     RGBColorTxt() const;
	COLORREF     RGBColorTxtDefault() const;
	COLORREF     RGBColorTxtButtonDefault() const;

	COLORREF     RGBColorBg() const;
	COLORREF     RGBColorBgDefault() const;
	COLORREF     RGBColorBgButtonDefault() const;
	COLORREF     RGBColorBgButtonLightDefault() const;
	HBRUSH       BrushBg() const;

	bool         IsDefaultStyle() const;
	void         Set(COLORREF, COLORREF);
	INT_PTR      ColorDialog();
	INT_PTR      ColorStatic(HDC);
	INT_PTR      ColorStatic(HDC, COLORREF);

	StyleDialog();
	StyleDialog(COLORREF, COLORREF);
	~StyleDialog();
};

}

#define __STYLE_BLACK_TXT    RGB(250,250,250)
#define __STYLE_BLACK_BG     RGB(30,30,30)
#define __STYLE_METRO_TXT    RGB(0,0,0)
#define __STYLE_METRO_BG     RGB(255,255,255)
#define __STYLE_METRO_BGBTND RGB(17,158,218)
#define __STYLE_METRO_BGBTNL RGB(65,177,225)
#define __STYLE_METRO_BGBTNT __STYLE_METRO_BG
