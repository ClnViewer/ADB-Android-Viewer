
#pragma once

namespace GameDev
{

class AdbMgrDialog
{
    private:
	HWND _hwnd;
	HINSTANCE _hinst;
	DWORD _rid;
	StyleDialog  _style;
	std::atomic<bool> _IsFuture;
	std::atomic<bool> _IsAdbStatus;
	std::future<std::wstring> _tfut;
	ADBDriver * _adb;

	INT_PTR __DialogProc(HWND, UINT, WPARAM, LPARAM);

	INT_PTR _OnWM_CHAR(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CLOSE(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_COMMAND(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CTLCOLORDLG(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_CTLCOLORSTATIC(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_DRAWITEM(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_INITDIALOG(HWND, UINT, WPARAM, LPARAM);
	INT_PTR _OnWM_NCHITTEST(HWND, UINT, WPARAM, LPARAM);

    public:
	static INT_PTR CALLBACK DialogRedirect(HWND, UINT, WPARAM, LPARAM);

	INT_PTR Show();
	INT_PTR Show(ADBDriver*);
	INT_PTR Show(ADBDriver&);

	AdbMgrDialog(DWORD);
	AdbMgrDialog(HINSTANCE, DWORD);
	AdbMgrDialog(HINSTANCE, DWORD, COLORREF, COLORREF);
	~AdbMgrDialog();
};

}
