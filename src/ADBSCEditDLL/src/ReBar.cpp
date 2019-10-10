
#include "SCEditInternal.h"
#include <commctrl.h>

namespace Editor
{
    struct RebarData
    {
        int32_t default_y, default_id, default_img_max, default_img;
        RebarData()
            : default_y(40),  default_id(ID_REBAR_BAND),
              default_img_max(0), default_img(0) {}
    };
    static RebarData rbd{};

    ReBar::~ReBar()
    {
        if (m_himl)
            ::ImageList_Destroy(m_himl);
        m_himl = nullptr;
    }

    bool ReBar::create(HWND hwnd_, HINSTANCE hinst_)
    {
        if (!WindowBase::create(
                hwnd_,
                hinst_,
                (HMENU)ID_REBAR,
                WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE |
                CCS_NODIVIDER |
                RBS_VARHEIGHT | RBS_BANDBORDERS,
                REBARCLASSNAME, ""
            ))
            return false;

        HBITMAP hbmp  = nullptr;
        HFONT   hfont = Editor::Config::instance().getfont();
        if (hfont)
            ::SendMessage(m_hwnd, WM_SETFONT, (LPARAM)hfont, 1);

        do
        {
            if (!(hbmp = ::LoadBitmap(hinst_, MAKEINTRESOURCE(ID_REBAR_BITMAP))))
            {
                HMODULE hmd;
                if ((::GetModuleHandleEx(
                        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                        reinterpret_cast<LPCTSTR>(RunEdit),
                        &hmd)) && (hmd != INVALID_HANDLE_VALUE))
                    if (!(hbmp = ::LoadBitmap(hmd, MAKEINTRESOURCE(ID_REBAR_BITMAP))))
                        break;
            }

            BITMAP bmp{};
            if (!::GetObject(hbmp, sizeof(bmp), &bmp))
                break;

            if ((!bmp.bmWidth) || (!bmp.bmHeight))
                break;

            rbd.default_img_max = (bmp.bmWidth / bmp.bmHeight);
            if (!rbd.default_img_max)
                break;

            m_himl = ::ImageList_Create(
                            bmp.bmHeight,
                            bmp.bmHeight,
                            ILC_COLOR16 | ILC_MASK,
                            rbd.default_img_max,
                            0
                    );
            if (!m_himl)
                break;

            ImageList_AddMasked(m_himl, hbmp, RGB(128,0,128));
            DeleteObject(hbmp); hbmp = nullptr;

            if (ImageList_GetImageCount(m_himl) < rbd.default_img_max)
                break;

            REBARINFO rbi{};
            rbi.cbSize = sizeof(rbi);
            rbi.fMask  = RBIM_IMAGELIST;
            rbi.himl   = m_himl;

            if (!::SendMessage(m_hwnd, RB_SETBARINFO, 0, reinterpret_cast<LPARAM>(&rbi)))
                break;

            return true;
        }
        while (0);

        if (hbmp)
            DeleteObject(hbmp);
        return false;
    }

    int32_t ReBar::addband(HWND hwnd_, int32_t y_)
    {
        REBARBANDINFO rbi{};
        rbi.cbSize = sizeof(rbi);
        rbi.fMask =
            RBBIM_ID    |
            RBBIM_STYLE |
            RBBIM_SIZE  |
            RBBIM_IMAGE |
            RBBIM_CHILD |
            RBBIM_CHILDSIZE;
        rbi.fStyle =
            // RBBS_BREAK  |
            RBBS_CHILDEDGE |
            RBBS_TOPALIGN  |
            RBBS_GRIPPERALWAYS;

        rbd.default_y  = ((y_) ? y_ : rbd.default_y);
        rbi.hwndChild  = hwnd_;
        rbi.cyMinChild = rbd.default_y;
        rbi.wID        = rbd.default_id;
        rbi.iImage     = rbd.default_img;

        if (rbd.default_img < (rbd.default_img_max - 1))
            rbd.default_img++;

        ::SendMessage(
                    m_hwnd,
                    RB_INSERTBAND,
                    WPARAM(-1),
                    reinterpret_cast<LPARAM>(&rbi)
                );
        return rbd.default_id++;
    }

    void ReBar::showband(int32_t id_)
    {
	int32_t idx = ::SendMessage(m_hwnd, RB_IDTOINDEX, WPARAM(ID_REBAR_BAND + id_), 0);
	if (idx != -1)
        ::SendMessage(m_hwnd, RB_MAXIMIZEBAND, WPARAM(idx), 0);
    }

};
