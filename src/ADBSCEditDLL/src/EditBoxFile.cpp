
#include "SCEditInternal.h"
#include "Scintilla.h"
#include "SciLexer.h"

namespace Editor
{
    static void f_SetupFileDialog(
                OPENFILENAMEA *sfn_,
                std::string & s_,
                DWORD flags_
            )
    {
        sfn_->lStructSize = sizeof(OPENFILENAMEW);
        sfn_->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags_;
        sfn_->lpstrFilter = static_cast<LPCSTR>("Lua script files (*.lua)\0*.lua\0");
        sfn_->lpstrDefExt = static_cast<LPCSTR>("lua");
        sfn_->lpstrInitialDir = static_cast<LPCSTR>(".\\");
        sfn_->nFilterIndex = 1;
        sfn_->hwndOwner = Config::instance().gethandle();
        sfn_->lpstrFile = static_cast<LPSTR>(
                            const_cast<char*>(s_.c_str())
                        );
        sfn_->nMaxFile = s_.size();
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
        catch (std::exception & _ex)
        {
            ALERTBox(_ex.what()); return;
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return;
        }
    }

    std::string EditBox::filesave(std::string const & s)
    {
        try
        {
            std::string fname;

            do
            {
                if (!s.empty())
                    fname.assign(s.c_str());
                else
                    fname.assign("plugin-lua.lua");
                fname.resize(512);

                if (s.empty())
                {
                    OPENFILENAMEA sfn{};
                    f_SetupFileDialog(
                        &sfn,
                        fname,
                        OFN_LONGNAMES | OFN_OVERWRITEPROMPT
                    );
                    if (!::GetSaveFileNameA(&sfn))
                        break;
                }

                std::vector<char> v;
                if (!gettext<std::vector<char>>(v))
                {
                    ALERTBox("not get saves text from editor");
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
            }
            while (0);
            return fname;
        }
        catch (std::exception & _ex)
        {
            ALERTBox(_ex.what()); return std::string();
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return std::string();
        }
    }

    std::string EditBox::fileopen()
    {
        try
        {
            std::string fname;

            do
            {
                fname.resize(512);

                OPENFILENAMEA sfn{};
                f_SetupFileDialog(
                    &sfn,
                    fname,
                    OFN_LONGNAMES | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST
                );
                if (!::GetSaveFileNameA(&sfn))
                    break;

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
                    ALERTBox("opening file is empty");
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
            }
            while (0);
            return fname;
        }
        catch (std::exception & _ex)
        {
            ALERTBox(_ex.what()); return std::string();
        }
        catch (...)
        {
            ALERTBox(g_scedit_error); return std::string();
        }
    }

};
