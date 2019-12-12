
#pragma once

namespace GUI
{
	class DLL_EXPORT CustomDialog
	{
		public:
			//
			using click_cb = std::function<bool(CustomDialog*, HWND, uint32_t, uint32_t)>;
			using click_cb_t = std::tuple<std::vector<uint32_t>, click_cb>;
			//
			virtual ~CustomDialog();
			//
			uint32_t                 Show(HINSTANCE, HWND, DWORD = 0U, bool = true);
			void                     SetTitle(std::string const&);
			void                     SetOnCreate(click_cb);
			void                     SetOnCommand(uint32_t, click_cb);
			void                     SetOnCommand(uint32_t, uint32_t, click_cb);
			void                     SetCtrl(std::vector<std::tuple<bool, int32_t, std::string, std::string>> const &);
			std::vector<std::string> GetCtrlValue();
			std::string              GetCtrlText(uint32_t);
			//
			LRESULT                  Command(uint32_t, uint32_t, void* = nullptr);
			void                     RadioBtnGroupCheck(uint32_t, uint32_t, uint32_t);
			void                     RadioBtnSetCheck(uint32_t, bool);
			bool                     RadioBtnIsCheck(uint32_t);

		private:
			//
			HWND      m_hdlg = nullptr;
			HWND      m_phwnd = nullptr;
			HINSTANCE m_hinst = nullptr;
			uint32_t  m_resid = 0;
			//
			std::string                                                      m_title;
			click_cb                                                         m_create = [](CustomDialog*, HWND, uint32_t, uint32_t){ return true; };
			std::vector<click_cb_t>                                          m_click;
			std::vector<std::string>                                         m_return;
			std::vector<std::tuple<bool, int32_t, std::string, std::string>> m_setctrl;
			//
			void     CloseReturn(int32_t);
			void     ClearHandle();
			uint32_t ShowModal();
			uint32_t ShowModalless();
			void     OnEvent(uint32_t, uint32_t);
			//
			static LRESULT StaticProc(HWND, UINT, WPARAM, LPARAM);

		protected:
			virtual void DlgProc(UINT, WPARAM, LPARAM);
	};
};

/*
	// Example:

    GUI::CustomDialog cdlg;
    cdlg.SetTitle("Ohhhhhhhhhhh!");
    cdlg.SetCtrl(
        {
            { true, IDC_EDIT1, "aaaaaa", "caption 1" },
            { true, IDC_EDIT2, "bbbbbb", "caption 2" }
        });

	cdlg.SetOnCommand(
		IDOK,
		[&](GUI::CustomDialog *p, HWND hwnd, uint32_t cmd)
		{
			// ... code ...
			return true;
		});

    auto ret = cdlg.Show(hinstance, hwnd, IDD_DIALOG2);
    if (ret == IDOK)
	{
		auto v = cdlg.GetCtrlValue();
		for (auto & s : v)
			MessageBoxA(hwnd, s.c_str(), "+++", 0);
	}
*/
