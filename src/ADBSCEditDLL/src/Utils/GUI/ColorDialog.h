
#pragma once

namespace GUI
{
	class DLL_EXPORT ColorDialog
	{
		private:
			//
			HWND      m_phwnd = nullptr;
			COLORREF  m_color = 0;
			//

		public:
			//
			virtual ~ColorDialog();
			//
			bool                     Show(HWND = nullptr);
			bool                     Show(HWND, COLORREF&);
			std::optional<COLORREF>  GetCtrlValue();
			std::string              GetCtrlString();
			//
	};
};

/*
	// Example:

    GUI::ColorDialog clrd;
    bool b = clrd.Show(hwnd);
    if (b)
	{
		std::string s = clrd.GetCtrlString();
		MessageBoxA(hwnd, s.c_str(), "+++", 0);
	}

*/
