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

#include "../../SCEditInternal.h"
#include "DialogInternal.h"

namespace GUI
{
	//

	ColorDialog::~ColorDialog()
	{
	}

	std::optional<COLORREF> ColorDialog::GetCtrlValue()
	{
		if (!m_color)
			return {};
		return { m_color };
	}

	std::string ColorDialog::GetCtrlString()
	{
		if (!m_color)
			return std::string();

		std::stringstream ss;
		ss << "{ " << (uint32_t)GetRValue(m_color);
		ss << ", " << (uint32_t)GetGValue(m_color);
		ss << ", " << (uint32_t)GetBValue(m_color);
		ss << " }";
		return ss.str();
	}

	bool ColorDialog::Show(HWND phwnd_)
	{
		COLORREF color_ = RGB(255,255,255);
		return Show(phwnd_, color_);
	}

	bool ColorDialog::Show(HWND phwnd_, COLORREF & color_)
	{
		m_color = 0;
		if (phwnd_)
			m_phwnd = phwnd_;
		//
		CHOOSECOLOR cc{};
		COLORREF    gcolor[16]{};
		//
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = m_phwnd;
		cc.lpCustColors = reinterpret_cast<LPDWORD>(gcolor);
		cc.rgbResult = color_;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;
		//
		if (::ChooseColor(&cc))
		{
			m_color = color_ = cc.rgbResult;
			return true;
		}
		return false;
	}

};
