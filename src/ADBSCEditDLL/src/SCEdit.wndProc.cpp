
#include "SCEditInternal.h"
#include "Scintilla.h"
#include <windowsx.h>

#define break_error() { ERRORBox(); break; }

namespace Editor
{
    LRESULT SCEdit::wndProc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        try
        {
            switch(msg)
            {
                HANDLE_MSG (hwnd, WM_CLOSE,     _OnClose);
                HANDLE_MSG (hwnd, WM_CREATE,    _OnCreate);
                HANDLE_MSG (hwnd, WM_NOTIFY,    _OnNotify);
                HANDLE_MSG (hwnd, WM_MOVE,      _OnMove);
                HANDLE_MSG (hwnd, WM_SIZE,      _OnSize);
                HANDLE_MSG (hwnd, WM_NCHITTEST, _OnNCHitTest);
                case WM_COMMAND:
                    {
                        _OnCommand(hwnd, msg, wParam, lParam);
                        return 0;
                    }
                default: return DefWindowProc (hwnd, msg, wParam, lParam);
            }
        }
        catch (std::exception & ex_)
        {
            ALERTBox(ex_.what()); return 0;
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return 0;
        }
    }

    LRESULT CALLBACK SCEdit::wndRedirect_(HWND hwnd_, UINT umsg_, WPARAM wp_, LPARAM lp_)
    {
        if (umsg_ == WM_NCCREATE)
        {
            LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lp_);
            if (lpcs)
            {
                LPARAM lp = reinterpret_cast<LPARAM>(lpcs->lpCreateParams);
                ::SetWindowLongPtrA(hwnd_, GWLP_USERDATA, lp);
            }
        }
        Editor::SCEdit *sce =
            reinterpret_cast<Editor::SCEdit*>(GetWindowLongPtrA(hwnd_, GWLP_USERDATA));
        if (sce)
            return sce->wndProc_(hwnd_, umsg_, wp_, lp_);
        return ::DefWindowProc(hwnd_, umsg_, wp_, lp_);
    }
};
