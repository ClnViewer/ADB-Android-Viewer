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

#include <string>
#include <iostream>
#include <regex>
#include <filesystem>
#include "../../../ADBViewer/src/version.h"

namespace fs = std::filesystem;

const char *str[] =
{
    "class.function()",
    "class:function()",
    "function()",
    " -- 373  | 14  |  LOAD \"The table the script received has:\n\"",
    " -- 6    | 16  |  GET [LuaObject]",
    " -- 67   | 18  |  GET",
    " HYPERLINK \"THELP_APP\"Hot keys help\r" // "^\s+(HYPERLINK)\s+\"(\w+)\"(.*)\r$"

};

int main(int32_t argc, char *argv[])
{
    if (argc < 3)
    {
        fs::path l_exec{ argv[0] };
        std::cout << " * version: " << AVIEW_FULLVERSION_STRING << " - " <<  __DATE__ << std::endl;
        std::cout << " * using  : " << l_exec.filename().generic_string().c_str() << " <0-6> <ECMAScript regex>" << std::endl;
        std::cout << std::endl << " ! Bad arguments.." << std::endl;
        return 0;
    }
    try
    {
    int32_t idx = std::stoi(argv[1]);
    idx = ((idx > 6) ? 6 : idx);

    std::string s(str[idx]);
    std::string r(argv[2]);
    const std::regex re(
            r.c_str(),
            std::regex::ECMAScript
        );
    std::smatch m;
    bool b = regex_search(s, m, re);

    std::cout << "\n\tSearch: " << s.c_str() << std::endl;

    uint32_t cnt = 1U;
    for (auto & x : m)
        std::cout << "\t" << cnt++ << ": " << x <<"\n";

    std::cout << "\n\tFound: " << b << " - " << m.size() << std::endl;
    }
    catch (std::exception const & ex)
    {
        std::cout << "\n\tException: " << ex.what() << std::endl;
    }
    return 0;
}
