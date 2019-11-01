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
#include <Scintilla.h>
#include <SciLexer.h>

namespace Editor
{

#   include "EditBoxConstant.h"

    static void f_SetupFileDialog(
                OPENFILENAMEA *sfn_,
                std::vector<char> & v_,
                DWORD flags_
            )
    {
        sfn_->lStructSize = sizeof(OPENFILENAMEA);
        sfn_->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags_;
        sfn_->lpstrFilter = static_cast<LPCSTR>(l_ebFileDesc);
        sfn_->lpstrDefExt = static_cast<LPCSTR>(l_ebFileExt);
        sfn_->lpstrInitialDir = static_cast<LPCSTR>(l_ebFilePath);
        sfn_->nFilterIndex = 1;
        sfn_->hwndOwner = MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>();
        sfn_->lpstrFile = static_cast<LPSTR>(&v_[0]);
        sfn_->nMaxFile = MAX_PATH;
    }

    static void f_FileNameNotify()
    {
        ::SendMessageA(
                MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
                WM_COMMAND,
                IDM_EVENT_SET_CAPTION,
                reinterpret_cast<LPARAM>(
                        MDIWin::Config::instance().savepath.data()
                    )
            );
    }

    bool EditBox::filevalidext(std::string const & s)
    {
        std::size_t sp;
        if ((sp = s.find_last_of(".")) != std::string::npos)
            return (s.compare((sp + 1), 3, l_ebFileExt, 3) == 0);
        return false;
    }

    void EditBox::filedefault()
    {
        try
        {
            cmd(SCI_CLEARALL);
            cmd(SCI_SETTEXT, 0, (LPARAM)g_scedit_default_skeleton);
            cmd(SCI_SETUNDOCOLLECTION, 1);
            cmd(EM_EMPTYUNDOBUFFER);
            cmd(SCI_SETSAVEPOINT);
            cmd(SCI_GOTOPOS, 0);
            return;
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
    }

    bool EditBox::filesave(std::string const & s)
    {
        try
        {
            do
            {
                std::string fname;
                //
                if (s.empty())
                {
                    std::vector<char> v(MAX_PATH);
                    std::string tname(l_ebFileName);
                    v.assign(tname.begin(), tname.end());
                    v[tname.length()] = '\0';

                    OPENFILENAMEA sfn{};
                    //
                    f_SetupFileDialog(
                        &sfn,
                        v,
                        OFN_LONGNAMES | OFN_OVERWRITEPROMPT
                    );
                    if (!::GetSaveFileNameA(&sfn))
                    {
                        if (::GetLastError())
                            ERRORBox();
                        break;
                    }
                    fname = &v[0];
                    if (fname.empty())
                        break;
                }
                else
                    fname.assign(s.c_str());

                std::vector<char> v;
                if (!gettext<std::vector<char>>(v))
                {
                    ALERTBox(l_ebErrorGet);
                    break;
                }

                FILE __AUTO(__autofile) *fp = fopen(fname.c_str(), "wt");
                if (!fp)
                {
                    ERRORBox();
                    break;
                }
                ::fwrite(&v[0], 1,v.size(), fp);

                cmd(EM_EMPTYUNDOBUFFER);
                cmd(SCI_SETSAVEPOINT);
                //
                MDIWin::Config::instance().savepath = fname;
                f_FileNameNotify();
                return true;
            }
            while (0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return false;
    }

    bool EditBox::fileopen(std::string const & fname)
    {
        try
        {
            do
            {
                if (fname.empty())
                {
                    ALERTBox(l_ebErrorName);
                    break;
                }
                if (::_access(fname.c_str(), F_OK) < 0)
                {
                    ERRORBox();
                    break;
                }

                FILE __AUTO(__autofile) *fp = fopen(fname.c_str(), "rt");
                if (!fp)
                {
                    ERRORBox();
                    break;
                }
                ::fseek(fp, 0, FILE_END);
                int32_t sz = ::ftell(fp);
                ::fseek(fp, 0, FILE_BEGIN);
                if (!sz)
                {
                    ALERTBox(l_ebErrorSize);
                    break;
                }

                auto dbuf = std::make_unique<char[]>(++sz);
                ::fread(dbuf.get(), 1, sz, fp);

                cmd(SCI_CLEARALL);
                cmd(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(dbuf.get()));

                cmd(SCI_SETUNDOCOLLECTION, 1);
                cmd(EM_EMPTYUNDOBUFFER);
                cmd(SCI_SETSAVEPOINT);
                cmd(SCI_GOTOPOS, 0);
                //
                MDIWin::Config::instance().savepath = fname;
                f_FileNameNotify();
                return true;
            }
            while (0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return false;
    }

    bool EditBox::fileopen()
    {
        try
        {
            do
            {
                std::vector<char> v(MAX_PATH);
                v[0] = '\0';
                //
                OPENFILENAMEA sfn{};
                f_SetupFileDialog(
                    &sfn,
                    v,
                    OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST
                );
                if (!::GetSaveFileNameA(&sfn))
                {
                    if (::GetLastError())
                        ERRORBox();
                    break;
                }
                return fileopen(&v[0]);
            }
            while (0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return false;
    }

};
