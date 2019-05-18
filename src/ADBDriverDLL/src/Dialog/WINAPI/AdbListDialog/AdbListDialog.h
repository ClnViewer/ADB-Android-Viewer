//#pragma once

namespace GameDev
{

class AdbListDialog
{
    private:
	POINT _p;
	HWND _hwnd;
	HINSTANCE _hinst;
	DWORD _rid;
	std::atomic<int32_t> _sel;
	std::atomic<bool> _isreverse;
	SelectedList _dict;
	StyleDialog  _style;
	std::wstring _key;
	std::wstring _val;

	INT_PTR __DialogProc(HWND, UINT, WPARAM, LPARAM);

	INT_PTR _OnWM_CHAR(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CLOSE(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_COMMAND(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CTLCOLORDLG(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CTLCOLORSTATIC(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_DRAWITEM(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_INITDIALOG(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_NOTIFY(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_NCHITTEST(HWND, UINT, WPARAM, LPARAM);

    public:
	static INT_PTR CALLBACK DialogRedirect(HWND, UINT, WPARAM, LPARAM);

	INT_PTR Show(SelectedList&, int32_t, bool);
	INT_PTR Show(SelectedList&, int32_t);
	INT_PTR Show(SelectedList&);

	std::wstring SelectedKey() const;
	std::wstring SelectedValue() const;

	AdbListDialog(DWORD);
	AdbListDialog(HINSTANCE, DWORD);
	AdbListDialog(HINSTANCE, DWORD, COLORREF, COLORREF);
	~AdbListDialog();
};

}
