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

#include <atomic>
#include <string>
#include <tuple>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cctype>
#include "../../src/Utils/extern/tixml2ex.h"
#include "../../../ADBViewer/src/version.h"

namespace fs = std::filesystem;

    std::atomic<uint32_t> cnt = 0;
    static uint32_t f_upcount()
    {
        cnt += 1;
        return cnt.load();
    }
    //
    static void f_string_replace(std::string & s, std::string const & f, std::string const & r)
	{
	    std::size_t pos = 0U;
	    for (;
              ((pos = s.find(f, pos)) != std::string::npos);
              pos += r.length()
            )
        {
            s.replace(pos, f.length(), r);
        }
	}
    //
    static bool f_isempty(std::string const & s)
    {
        for (auto c : s)
        {
            if ((c != ' ')  &&
                (c != '\t') &&
                (c != '\r') &&
                (c != '\v') &&
                (c != '\n'))
                return false;
        }
        return true;
    }
    //
    static std::tuple<std::string,std::string> f_parse_xml_item(const tinyxml2::XMLElement *root, std::string const & sn)
    {
        std::string d_;
        std::string r_;
        std::stringstream ss;
        for (auto item : tinyxml2::selection(root, "Overload"))
        {
            d_ = attribute_value(item, "descr");
            r_ = attribute_value(item, "retVal");
        }
        //
        if (!r_.empty())
            ss << r_.c_str() << " ";
        ss << sn.c_str() << "(";
        //
        {
            std::stringstream ssp;
            for (auto item : tinyxml2::selection(root, "Overload/Param"))
            {
                r_ = attribute_value(item, "name");
                if (!r_.empty())
                    ssp << r_.c_str() << ", ";
            }
            if (!ssp.str().empty())
            {
                r_ = ssp.str();
                r_.erase(
                    (r_.length() - 2), r_.length()
                );
                ss << r_.c_str();
            }
        }
        //
        ss << ")";
        //
        return { ss.str(), d_ };
    }
    //
    static void f_parse_xml_section(
        std::ofstream & of,
        const tinyxml2::XMLDocument & root,
        std::function<void(std::ofstream&, std::string const&, std::string const&, std::string const&)> f)
    {
        for (auto item : tinyxml2::selection(root, "NotepadPlus/AutoComplete/KeyWord"))
        {
            std::string f_ = attribute_value(item, "func");
            if ((f_.empty()) || (f_ == "no"))
                continue;

            std::string n_ = attribute_value(item, "name");
            if (n_.empty())
                continue;

            auto [ s_, d_ ] = f_parse_xml_item(item, n_);
            f(of, n_, s_, d_);
        }
    }
    //
    int main(int argc, char *argv[])
    {
        if (argc < 2)
        {
            fs::path l_exec{ argv[0] };
            std::cout << " * version: " << AVIEW_FULLVERSION_STRING << " - " <<  __DATE__ << std::endl;
            std::cout << " * using  : " << l_exec.filename().generic_string().c_str() << " <Input.xml> [<Output.h>]" << std::endl;
            return 0;
        }
        //
        try
        {
            fs::path l_root{ argv[1] };

            if (!fs::exists(l_root))
                throw std::runtime_error("xml file not found");
            if (!fs::is_regular_file(l_root))
                throw std::runtime_error("file not xml regular file");

            auto doc = tinyxml2::load_xmlfile(l_root.generic_string());
            //
            std::string fname = ((argc >= 3) ? argv[2] : "help.h");
            std::ofstream outf(fname);
            if (!outf.is_open())
                throw std::runtime_error("output file not open");

            outf << "HLP_LABEL(\"LUA FUNCTION:\")\n";

            f_parse_xml_section(
                outf,
                static_cast<const tinyxml2::XMLDocument&>(*doc),
                [=](std::ofstream & of, std::string const & name_, std::string const & sig_, std::string const & desc_)
                {
                    std::string        s_;
                    std::string        n_;
                    std::stringstream  ss;
                    std::istringstream iss(desc_);
                    bool               isbegin = true;
                    //
                    while (std::getline(iss, s_, '\n'))
                    {
                        if (s_.empty())
                        {
                            if (!isbegin)
                                ss << "\t\"\\n\" \\\n";
                            isbegin = false;
                            continue;
                        }
                        if (!f_isempty(s_))
                            ss << "\t\"" << s_.c_str() << "\\n\" \\\n";
                    }
                    if (!ss.str().empty())
                    {
                        s_ = ss.str();
                        s_.erase(
                            (s_.length() - 6), s_.length()
                        );
                        s_ += "\"\n";
                    }
                    else
                        s_ = "";

                    n_ = name_;
                    f_string_replace(n_, ":", ".");

                    cnt++;

                    std::cout << f_upcount() << "\t| name: " << name_.c_str() << " -> [" << sig_.c_str() << "]" << std::endl;
                    of << "HLP_ITEM(\n";
                    of << "\t\"" << n_.c_str() << "\",\n";
                    of << "\t\"" << sig_.c_str() << "\",\n";
                    of << s_.c_str() << ")\n";
                }
            );
            outf << "\n#undef HLP_LABEL\n#undef HLP_ITEM\n";
            outf.close();
        }
        catch (tinyxml2::XmlException & _ex)
        {
            std::cout << "XmlException: " << _ex.what() << std::endl;
        }
        catch (std::exception & _ex)
        {
            std::cout << "Exception: " << _ex.what() << std::endl;
        }
        return 0;
    }
