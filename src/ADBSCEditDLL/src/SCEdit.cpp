
#include "SCEditInternal.h"
#include <fstream>
#include <streambuf>
#include <commctrl.h>

namespace Editor
{
    SCEdit::~SCEdit()
    {
        Editor::Config::instance().clear();

        if (m_haccel)
            ::DestroyAcceleratorTable(m_haccel);
        m_haccel = nullptr;

        if (m_hbgmain)
            ::DeleteObject(m_hbgmain);
        m_hbgmain = nullptr;
    }

    void SCEdit::Init(HINSTANCE hinst_, HICON hico_)
    {
        m_text = "";
        m_savepath = "";
        m_hinst = hinst_;
        m_hicon = hico_;
        Editor::Config::instance().init(hinst_);
    }

    void SCEdit::Init(std::string const & savepath_, std::string const & text_, HINSTANCE hinst_, HICON hico_)
    {
        m_text = text_;
        m_savepath = savepath_;
        m_hinst = hinst_;
        m_hicon = hico_;
        Editor::Config::instance().init(hinst_);
    }

    void SCEdit::Init(std::string const & openpath_, HINSTANCE hinst_, HICON hico_)
    {
        m_savepath = openpath_;
        m_hinst = hinst_;
        m_hicon = hico_;
        Editor::Config::instance().init(hinst_);

        if (openpath_.empty())
            return;

        try
        {
            std::ifstream ifs(openpath_.c_str());
            std::stringstream ss;
            ss << ifs.rdbuf();
            m_text = ss.str();
        }
        catch (std::exception const & ex_)
        {
            ALERTBox(ex_.what());
            return;
        }
        catch (...)
        {
            ALERTBox("read file error");
            return;
        }
    }

    std::string const SCEdit::getChildClassName()
    {
        return classNameChild;
    }

    std::string const SCEdit::getMenuClassName()
    {
        return classNameMenu;
    }

    bool SCEdit::Run(bool runtype)
    {
        try
        {
            std::thread thed
            {
                [=]()
                {
                    try         { run_(); }
                    catch (...) {}
                }
            };
            if (thed.joinable())
            {
                if (runtype)
                    thed.join();
                else
                    thed.detach();
            }
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    void SCEdit::run_()
    {
        do
        {
            if (!registerWin_(classNameMain, m_hicon))
                break;
            if (!registerWin_(classNameChild, m_hicon))
                break;
            if (!registerWin_(classNameMenu, m_hicon, ID_REBAR_MENU))
                break;
            if (!createWin_())
                break;

            (void) loopWin_();
            return;
        }
        while (0);
        ERRORBox();
    }

};

