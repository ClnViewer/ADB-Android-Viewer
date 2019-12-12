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
#include <shlobj.h>

namespace GUI
{
	//

	FileDialog::~FileDialog()
	{
	}

	std::string FileDialog::GetCtrlString()
	{
		return m_path;
	}

	void FileDialog::SetCtrlValue(FileDialog::CtrlValues_t const & var)
	{
		m_options = var;
	}

	void FileDialog::SetCtrlValue(HWND phwnd, FileDialog::CtrlValues_t const & var)
	{
		if (phwnd)
			m_phwnd = phwnd;
		m_options = var;
	}

	FileDialog::BuildValues_t FileDialog::BuildOptions()
	{
		auto & [ p, n, v ] = m_options;
		if (!v.size())
			return { false, std::string(), std::string(), std::string(), std::string() };

		std::stringstream sfil;
		std::stringstream sext;

		for (auto & [ f, e ] : v)
		{
			std::string ext;
			if (!e.empty())
				ext = e.c_str();
			else
				ext = "*";

			if (!f.empty())
				sfil << f.c_str() << " (*." << ext.c_str() << ")" << '\0' << "*." << ext.c_str() << '\0';
			sext << ext.c_str() << ";";
		}
		sfil << '\0' << '\0';

		if (p.empty())
			return { true, sfil.str(), sext.str(), ".\\", n };
		else
			return { true, sfil.str(), sext.str(), p, n };
	}

	bool FileDialog::MakeDialog(HWND phwnd, OPENFILENAMEA *ofn, DWORD flags, std::vector<char> & v)
	{
		m_path = "";
		if (phwnd)
			m_phwnd = phwnd;
		//
		auto [ b, f, e, p, n ] = BuildOptions();
		if (!b)
			return false;

        if (!n.empty())
        {
            v.assign(n.begin(), n.end());
            v[n.length()] = '\0';
        }
        else
            v[0] = '\0';
        //
        ofn->lStructSize = sizeof(OPENFILENAMEA);
        ofn->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags;
        ofn->lpstrFilter = static_cast<LPCSTR>(f.c_str());
        ofn->lpstrDefExt = static_cast<LPCSTR>(e.c_str());
        ofn->lpstrInitialDir = static_cast<LPCSTR>(p.c_str());
        ofn->nFilterIndex = 1;
        ofn->hwndOwner = m_phwnd;
        ofn->lpstrFile = static_cast<LPSTR>(&v[0]);
        ofn->nMaxFile = MAX_PATH;
        return true;
	}

	bool FileDialog::Open(HWND phwnd_)
	{
		OPENFILENAMEA ofn{};
		std::vector<char> v(MAX_PATH);

		if (!MakeDialog(phwnd_, &ofn, OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, v))
			return false;
		if (!::GetOpenFileNameA(&ofn))
			return false;

		m_path = &v[0];
		return true;
	}

	bool FileDialog::Save(HWND phwnd_)
	{
		OPENFILENAMEA ofn{};
		std::vector<char> v(MAX_PATH);

		if (!MakeDialog(phwnd_, &ofn, OFN_LONGNAMES | OFN_OVERWRITEPROMPT, v))
			return false;
		if (!::GetSaveFileNameA(&ofn))
			return false;

		m_path = &v[0];
		return true;
	}

	bool FileDialog::Folder(HWND phwnd_)
	{
		if (phwnd_)
			m_phwnd = phwnd_;

	    BROWSEINFO bi{};
	    ITEMIDLIST *items = nullptr;
	    std::vector<char> v(MAX_PATH);

        auto & [ p, n, w ] = m_options;
        if (!p.empty())
        {
            v.assign(p.begin(), p.end());
            v[p.length()] = '\0';
        }
        else
            v[0] = '\0';

        bi.hwndOwner = m_phwnd;
        bi.pidlRoot = 0;
        bi.pszDisplayName = static_cast<LPSTR>(&v[0]);
        bi.lpszTitle = static_cast<LPCSTR>(n.c_str());
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = 0;
        bi.lParam = 0;

        if (!(items = ::SHBrowseForFolderA(&bi)))
            return false;
        if (!::SHGetPathFromIDList(items, &v[0]))
            return false;

        m_path = &v[0];
        return true;
	}
};
