
#include "SCEditInternal.h"
#include <commctrl.h>

namespace Editor
{
            WindowBase::~WindowBase()
            {
                if (m_hwnd)
                    ::DestroyWindow(m_hwnd);
                m_hwnd = nullptr;
            }

            HWND WindowBase::handle()
            {
                return m_hwnd;
            }

            void WindowBase::active(bool isfocus)
            {
                if (!m_hwnd)
                    return;

                ::ShowWindow(m_hwnd, SW_SHOW);
                if (isfocus)
                    ::SetFocus(m_hwnd);
            }

            void WindowBase::setsize(RECT const & r)
            {
                (void) ::SetWindowPos(m_hwnd, 0, r.left, r.top, r.right, r.bottom, 0);
            }

            void WindowBase::setsize(HWND hwnd_)
            {
                RECT rc{};
                ::GetClientRect(hwnd_, &rc);
                (void) ::SetWindowPos(hwnd_, 0, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), 0);
            }

            void WindowBase::styleclear(HWND hwnd)
            {
                long style;
                style  = ::GetWindowLong(hwnd, GWL_STYLE);
                style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
                ::SetWindowLong(hwnd, GWL_STYLE, style);

                style  = ::GetWindowLong(hwnd, GWL_EXSTYLE);
                style &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
                ::SetWindowLong(hwnd, GWL_EXSTYLE, style);
                ::SetWindowPos(
                        hwnd,
                        0,
                        0,0,0,0,
                        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER
                    );
            }

#           define HWND_arg        HWND hwnd_
#           define HINSTANCE_arg   HINSTANCE hinst_
#           define HMENU_arg       HMENU hmenu_
#           define RECT_arg        RECT const & rc_
#           define FLAG_arg        int32_t flag_
#           define CNAME_A_arg     std::string const & s_cname_
#           define WNAME_A_arg     std::string const & s_wname_
#           define CNAME_W_arg     std::wstring const & s_cname_
#           define WNAME_W_arg     std::wstring const & s_wname_
#           define CNAME_T_arg     T const & s_cname_
#           define WNAME_T_arg     T const & s_wname_
#           define VOID_arg        void * vobj_

            bool WindowBase::create(
                    HWND_arg, HINSTANCE_arg, FLAG_arg, CNAME_A_arg)
            {
                RECT rc{};
                m_hwnd = create_<std::string>(hwnd_, hinst_, 0, rc, flag_, s_cname_, std::string(), nullptr);
                return !(!m_hwnd);
            }

            bool WindowBase::create(
                    HWND_arg, HINSTANCE_arg, RECT_arg, FLAG_arg, CNAME_A_arg, WNAME_A_arg)
            {
                if ((!rc_.right) || (!rc_.bottom))
                    return false;

                m_hwnd = create_<std::string>(hwnd_, hinst_, 0, rc_, flag_, s_cname_, s_wname_, nullptr);
                return !(!m_hwnd);
            }

            bool WindowBase::create(
                    HWND_arg, HINSTANCE_arg, HMENU_arg, FLAG_arg, CNAME_A_arg, WNAME_A_arg)
            {
                RECT rc{};
                m_hwnd = create_<std::string>(hwnd_, hinst_, hmenu_, rc, flag_, s_cname_, s_wname_, nullptr);
                return !(!m_hwnd);
            }

            bool WindowBase::create(
                    HWND_arg, HINSTANCE_arg, HMENU_arg, RECT_arg, FLAG_arg, CNAME_A_arg, WNAME_A_arg)
            {
                if ((!rc_.right) || (!rc_.bottom))
                    return false;

                m_hwnd = create_<std::string>(hwnd_, hinst_, hmenu_, rc_, flag_, s_cname_, s_wname_, nullptr);
                return !(!m_hwnd);
            }

            template<typename T>
            HWND WindowBase::createEx(
                    HWND_arg, HINSTANCE_arg, HMENU_arg, RECT_arg, FLAG_arg, CNAME_T_arg, WNAME_T_arg, VOID_arg)
            {
                if ((!rc_.right) || (!rc_.bottom))
                    return nullptr;

                return create_<T>(hwnd_, hinst_, hmenu_, rc_, flag_, s_cname_, s_wname_, vobj_);
            }
            template HWND WindowBase::createEx<std::string> (HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::string const&,  std::string const&,  void*);
            template HWND WindowBase::createEx<std::wstring>(HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::wstring const&, std::wstring const&, void*);

            template<typename T>
            HWND WindowBase::create_(
                    HWND_arg, HINSTANCE_arg, HMENU_arg, RECT_arg, FLAG_arg, CNAME_T_arg, WNAME_T_arg, VOID_arg)
            {
                ::SetLastError(0);

                do {
                    if ((!hwnd_) || (!hinst_))
                        break;

                    HWND hwnd_child = nullptr;

                    if constexpr (std::is_same<T, std::string>::value)
                    {
                        hwnd_child = ::CreateWindowExA(
                                     WS_EX_CLIENTEDGE,
                                     static_cast<LPCSTR>(s_cname_.c_str()),
                                     static_cast<LPCSTR>(s_wname_.c_str()),
                                     flag_,
                                     rc_.left, rc_.top, rc_.right, rc_.bottom,
                                     hwnd_,
                                     hmenu_,
                                     hinst_,
                                     vobj_
                             );
                    }
                    else if constexpr (std::is_same<T, std::wstring>::value)
                    {
                        hwnd_child = ::CreateWindowExW(
                                     WS_EX_CLIENTEDGE,
                                     static_cast<LPCWSTR>(s_cname_.c_str()),
                                     static_cast<LPCWSTR>(s_wname_.c_str()),
                                     flag_,
                                     rc_.left, rc_.top, rc_.right, rc_.bottom,
                                     hwnd_,
                                     hmenu_,
                                     hinst_,
                                     vobj_
                             );
                    }
                    if (!hwnd_child)
                        break;

                    styleclear(hwnd_child);
                    return hwnd_child;
                }
                while (0);
                return nullptr;
            }
            template HWND WindowBase::create_<std::string> (HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::string const&,  std::string const&,  void*);
            template HWND WindowBase::create_<std::wstring>(HWND, HINSTANCE, HMENU, RECT const&, int32_t, std::wstring const&, std::wstring const&, void*);

};

