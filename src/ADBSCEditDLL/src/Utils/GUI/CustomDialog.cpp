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
#include <functional>
#include "DialogInternal.h"

namespace GUI
{
	//

	CustomDialog::~CustomDialog()
	{
		ClearHandle();
	}

	void CustomDialog::ClearHandle()
	{
		if (m_hdlg)
			::DestroyWindow(m_hdlg);
		m_hdlg = nullptr;
	}
	void CustomDialog::CloseReturn(int32_t ret)
	{
		::EndDialog(m_hdlg, ret);
		m_hdlg = nullptr;
	}

	uint32_t CustomDialog::Show(HINSTANCE hinst_, HWND phwnd_, DWORD resid_, bool ismodal_)
	{
		m_hinst = hinst_;
		m_phwnd = phwnd_;
		if (resid_)
			m_resid = resid_;
		m_return.clear();

		if (!m_resid)
			return IDCANCEL;

		ClearHandle();

		if (ismodal_)
			return ShowModal();
		else
			return ShowModalless();
	}

	uint32_t CustomDialog::ShowModal()
	{
		uint32_t ret = ::DialogBoxParam(
					m_hinst,
					MAKEINTRESOURCE(m_resid),
					m_phwnd,
					(DLGPROC)GUI::CustomDialog::StaticProc,
					(LPARAM)this
			);
		m_hdlg = nullptr;
		return ret;
	}

	uint32_t CustomDialog::ShowModalless()
	{
		m_hdlg = ::CreateDialogParam(
					m_hinst,
					MAKEINTRESOURCE(m_resid),
					m_phwnd,
					(DLGPROC)GUI::CustomDialog::StaticProc,
					(LPARAM)this
			);
		if (!m_hdlg)
			return IDCANCEL;

		::ShowWindow(m_hdlg, SW_SHOW);
		return IDOK;
	}

	void CustomDialog::SetTitle(std::string const & s)
	{
		m_title = s;
	}
	void CustomDialog::SetCtrl(std::vector<std::tuple<bool, int32_t, std::string, std::string>> const & var)
	{
		m_setctrl = var;
	}
	std::vector<std::string> CustomDialog::GetCtrlValue()
	{
		return m_return;
	}
	void CustomDialog::SetOnCreate(click_cb fun)
	{
		m_create = fun;
	}
	void CustomDialog::SetOnCommand(uint32_t id, click_cb fun)
	{
		m_click.push_back({ { id }, fun });
	}
	void CustomDialog::SetOnCommand(uint32_t rnbegin, uint32_t rnend, click_cb fun)
	{
		uint32_t sz = ((rnend > rnbegin) ?
							(rnend - rnbegin) : ((rnend == rnbegin) ? 1 : 0)
						);
		if (!sz)
			return;

		std::vector<uint32_t> v(sz);
		for (uint32_t i = rnbegin; rnend >= i; i++)
			v.push_back(i);

		m_click.push_back({ v, fun });
	}
	std::string CustomDialog::GetCtrlText(uint32_t id)
	{
		if (!m_hdlg)
			return std::string();

		HWND hwnd = ::GetDlgItem(m_hdlg, id);
		if (!hwnd)
			return std::string();

		std::string s;
		std::vector<char> v(2048 + 1);
		if (::GetWindowTextA(hwnd, &v[0], 2048))
			s = &v[0];
		return s;
	}
	LRESULT CustomDialog::Command(uint32_t msg_, uint32_t wp_, void *lp_)
	{
		if (!m_hdlg)
			return -1;
		return ::SendMessage(m_hdlg, msg_, (WPARAM)wp_, reinterpret_cast<LPARAM>(lp_));
	}
	void CustomDialog::RadioBtnGroupCheck(uint32_t rb_begin, uint32_t rb_end, uint32_t rb_check)
	{
		if (!m_hdlg)
			return;
		::CheckRadioButton(m_hdlg, rb_begin, rb_end, rb_check);
	}
	void CustomDialog::RadioBtnSetCheck(uint32_t id, bool ischeck)
	{
		if (!m_hdlg)
			return;
		HWND hwnd = ::GetDlgItem(m_hdlg, id);
		if (hwnd)
			::SendMessage(hwnd, BM_SETCHECK, (WPARAM)static_cast<int32_t>(ischeck), (LPARAM)0);
	}
	bool CustomDialog::RadioBtnIsCheck(uint32_t id)
	{
		if (!m_hdlg)
			return false;
		return (::IsDlgButtonChecked(m_hdlg, id));
	}

	//

	void CustomDialog::OnEvent(uint32_t idx, uint32_t cmd)
	{
		for (auto & [ vid, fun ] : m_click)
			for (auto & id : vid)
				if (idx == id)
				{
					if (!fun(this, ::GetDlgItem(m_hdlg, id), idx, cmd))
						CloseReturn(IDCANCEL);
					break;
				}
	}

	void CustomDialog::DlgProc(UINT umsg_, WPARAM wp_, LPARAM)
	{
		if (umsg_ == WM_COMMAND)
		{
			uint32_t idx = LOWORD(wp_);
			OnEvent(idx, HIWORD(wp_));
			//
			switch (idx)
			{
				case IDCANCEL:
				{
					CloseReturn(IDCANCEL);
					break;
				}
				case IDOK:
				{
					for (auto & [ b, id, txt, banner ] : m_setctrl)
					{
						if (!b)
							continue;

						HWND hwnd_ = ::GetDlgItem(m_hdlg, id);
						if (hwnd_)
						{
							std::vector<char> v(1024);
							if (::GetWindowTextA(hwnd_, &v[0], 1024))
							{
								std::string s = &v[0];
								if (!s.empty())
									m_return.push_back(s);
							}
						}
					}
					CloseReturn(IDOK);
					break;
				}
				default:
					break;
			}
		}
		else if (umsg_ == WM_CLOSE)
			CloseReturn(IDCANCEL);
	}

	LRESULT CustomDialog::StaticProc(HWND hdlg_, UINT umsg_, WPARAM wp_, LPARAM lp_)
	{
		GUI::CustomDialog *pDlg = nullptr;
		if (umsg_ == WM_INITDIALOG)
		{
			pDlg = reinterpret_cast<GUI::CustomDialog*>(lp_);
			pDlg->m_hdlg = hdlg_;
			//
			::SetWindowLong(hdlg_, GWL_USERDATA, reinterpret_cast<long>(pDlg));
			//
			if (!pDlg->m_title.empty())
				::SendMessageA(hdlg_, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(pDlg->m_title.c_str()));
			//
			if (pDlg->m_setctrl.size())
				for (auto & [ b, id, txt, banner ] : pDlg->m_setctrl)
				{
					if (!txt.empty())
						::SendDlgItemMessage(hdlg_, id, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(txt.c_str()));
					if (!banner.empty())
					{
						std::wstring ws(banner.begin(), banner.end());
						::SendDlgItemMessage(hdlg_, id, EM_SETCUEBANNER, 1, reinterpret_cast<LPARAM>(ws.c_str()));
					}
				}
			if (!pDlg->m_create(pDlg, hdlg_, 0, 0))
				pDlg->CloseReturn(IDCANCEL);
		}
		else
			pDlg = reinterpret_cast<GUI::CustomDialog*>(::GetWindowLong(hdlg_, GWL_USERDATA));
		if (pDlg)
			pDlg->DlgProc(umsg_, wp_, lp_);
		return 0;
	}

};
