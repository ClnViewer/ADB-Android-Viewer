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

#include "../SCEditInternal.h"
#include <richedit.h>

namespace Editor
{
    //
#   include "AppHelp.Static.cxx"
    //

    AppHelp::AppHelp()
    {
        m_re[0].assign("^(\\w+)\\(.*$", std::regex::ECMAScript);
        m_re[1].assign("^(\\w+)(?:[.:])(\\w+)\\(.*$", std::regex::ECMAScript);
        m_re[2].assign("^(\\d+)([\\s\\|]+)(\\d+)([\\s\\|]+)([\\w]+).*", std::regex::ECMAScript);
        m_re[3].assign("^\\s+(HYPERLINK)\\s+\"(\\w+)\"(.*)\\r$", std::regex::ECMAScript);
    }

    std::tuple<AppHelp::HelpType, std::string> AppHelp::find_(std::string const & s)
    {
        AppHelp::HelpType t = AppHelp::HelpType::THELP_NONE;
        std::string src;

        try
        {
            for (uint32_t i = 0U; i < __NELE(m_re); i++)
            {
                std::smatch m;
                if (!std::regex_search(s, m, m_re[i]))
                    continue;
                //
                switch (i)
                {
                    case 0:
                        {
                            if (m.size() != 2)
                                break;

                            src.assign(m.str(1));
                            t = AppHelp::HelpType::THELP_LUAFUNC;
                            break;
                        }
                    case 1:
                        {
                            if (m.size() != 3)
                                break;

                            src.assign(m.str(1));
                            src.append(".");
                            src.append(m.str(2));
                            if (src.compare(0, 9, "LuaObject", 9) == 0)
                                t = AppHelp::HelpType::THELP_LUAOBJECT;
                            else
                                t = AppHelp::HelpType::THELP_LUAFUNC;
                            break;
                        }
                    case 2:
                        {
                            if (m.size() != 6)
                                break;

                            src.assign(m.str(5));
                            t = AppHelp::HelpType::THELP_LUABYTECODE;
                            break;
                        }
                    default:
                        break;
                }
                if (!src.empty())
                    break;
            }
            if ((src.empty()) ||  (t == AppHelp::HelpType::THELP_NONE))
                return { AppHelp::HelpType::THELP_NONE, std::string() };
            return { t, src };
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return { AppHelp::HelpType::THELP_NONE, std::string() };
    }
    //
    AppHelp::helparray_t AppHelp::get(AppHelp::HelpType t)
    {
        switch (t)
        {
            case HelpType::THELP_APP:         return { l_items_THELP_APP,         __NELE(l_items_THELP_APP) };
            case HelpType::THELP_LUAFUNC:     return { l_items_THELP_LUAFUNC,     __NELE(l_items_THELP_LUAFUNC) };
            case HelpType::THELP_LUAOBJECT:   return { l_items_THELP_LUAOBJECT,   __NELE(l_items_THELP_LUAOBJECT) };
            case HelpType::THELP_LUABYTECODE: return { l_items_THELP_LUABYTECODE, __NELE(l_items_THELP_LUABYTECODE) };
            case HelpType::THELP_LINK:        return { l_items_THELP_LINK,        __NELE(l_items_THELP_LINK) };
            default:                          return { nullptr, 0 };
        }
    }
    //
    AppHelp::helparray_t AppHelp::get(std::string const & s)
    {
        try
        {
            auto [ t, src ] = find_(s);
            if (t == AppHelp::HelpType::THELP_NONE)
                return { nullptr, 0 };

            uint32_t idx = 0U;
            auto [ items, isz ] = get(t);
            for (uint32_t i = 0; i < isz; i++)
            {
                std::string s_;
                //
                if (items[i].flag == CFM_ALL2)
                {
                    if (!items[i].title)
                        continue;
                    s_ = items[i].title;
                }
                else if (items[i].flag == CFM_ALL)
                {
                    if (!items[i].key)
                        continue;
                    s_ = items[i].key;
                }
                else
                    continue;
                //
                if (src.compare(0, s_.length(), s_.c_str(), s_.length()) == 0)
                    m_finditems[idx++] = items[i];
                if (idx > 10)
                    break;
            }
            if (idx)
                return { m_finditems, idx };
            return { nullptr, 0 };
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return {};
    }

    AppHelp::HelpType AppHelp::isurl(std::string const & s)
    {
        if ((s.compare(0, 6, "ftp://", 6)   == 0) ||
            (s.compare(0, 7, "http://", 7)  == 0) ||
            (s.compare(0, 8, "https://", 8) == 0))
            return AppHelp::HelpType::THELP_URL;
        return AppHelp::HelpType::THELP_NONE;
    }

    AppHelp::HelpType AppHelp::getid(std::string const & s)
    {
        try
        {
            do
            {
                if (isurl(s) == AppHelp::HelpType::THELP_URL)
                    return AppHelp::HelpType::THELP_URL;
                //
                std::smatch m;
                if (!std::regex_search(s, m, m_re[3]))
                    break;

                if (m.size() != 4)
                    break;

                std::string src = m.str(2);
                /*
                    // Debug:
                    ALERTBox(src);
                 */
                for (uint32_t i = 0; i < __NELE(l_items_THELP_LINK); i++)
                {
                    AppHelp::HelpItems & item = l_items_THELP_LINK[i];
                    if (src.compare(item.key) == 0)
                        switch (i)
                        {
                            case 0:  return AppHelp::HelpType::THELP_APP;
                            case 1:  return AppHelp::HelpType::THELP_LUAFUNC;
                            case 2:  return AppHelp::HelpType::THELP_LUABYTECODE;
                            case 3:  return AppHelp::HelpType::THELP_LUAOBJECT;
                            default: return AppHelp::HelpType::THELP_NONE;
                        }
                }
            }
            while (0);
        }
        catch (...) { GameDev::ExceptionPrn::parse(std::current_exception(), g_scedit_error); }
        return AppHelp::HelpType::THELP_NONE;
    }

};
