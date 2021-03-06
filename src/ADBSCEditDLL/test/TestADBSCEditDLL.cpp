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

#include <iostream>
#include <thread>
#include <chrono>
#include "../src/SCEditDll.h"
#if defined(_DEBUG)
#  include <exchndl.h>
#endif

const char *textlua0 =
    "   --* comment\n" \
    "function main (stateOld)\n" \
    "   if stateOld == 0 then LuaObject:adbClick(378,404)\n" \
    "   elseif stateOld == 1 then LuaObject:adbClick(460,412)\n" \
    "   elseif stateOld == 2 then LuaObject:adbClick(477,474)\n" \
    "   elseif stateOld == 3 then LuaObject:adbClick(208,405)\n" \
    "   elseif stateOld == 4 then LuaObject:adbClick(378,342)\n" \
    "   elseif stateOld == 5 then LuaObject:adbClick(564,467)\n" \
    "   elseif stateOld == 6 then LuaObject:adbClick(497,536)\n" \
    "   elseif stateOld == 7 then LuaObject:adbClick(394,469)\n" \
    "   elseif stateOld == 8 then LuaObject:adbClick(482,464)\n" \
    "   elseif stateOld == 9 then LuaObject:adbClick(463,401)\n" \
    "   elseif stateOld == 10 then LuaObject:adbClick(900,459)\n" \
    "   elseif stateOld == 11 then LuaObject:adbClick(725,525)\n" \
    "   elseif stateOld == 12 then stateOld = -1 end\n" \
    "   \n\n\n" \
    "    LuaObject:checkPixelsByCord\n" \
    "   \n\n\n" \
    "       entry\n" \
    "       {\n" \
    "           title = \"Tecgraf\",\n" \
    "           org = \"Computer Graphics Technology Group, PUC-Rio\",\n" \
    "           url = \"http://www.tecgraf.puc-rio.br/\",\n" \
    "           contact = \"Waldemar Celes\",\n" \
    "           description = [[\n" \
    "               TeCGraf is the result of a partnership between PUC-Rio,\n" \
    "               the Pontifical Catholic University of Rio de Janeiro,\n" \
    "               and <A HREF=\"http://www.petrobras.com.br/\">PETROBRAS</A>,\n" \
    "               the Brazilian Oil Company.\n" \
    "               TeCGraf is Lua's birthplace,\n" \
    "               and the language has been used there since 1993.\n" \
    "               Currently, more than thirty programmers in TeCGraf use\n" \
    "               Lua regularly; they have written more than two hundred\n" \
    "               thousand lines of code, distributed among dozens of\n" \
    "               final products.]]\n" \
    "       }\n\n\n" \
    "   var aaa = \"gggggg\"\n" \
    "   var bbb = '\\n'\n\n" \
    "       math.atan2(0)\n" \
    "       utf8.codepoint(\"gfgfggfggf\")\n" \
    "       pcall(abc)\n" \
    "       io.close(f)\n" \
    "       string.byte()\n" \
    "       coroutine.resume()\n" \
    "       coroutine c = 0\n" \
    "       math.log(8765)\n" \
    "       bit32.bnot(987654321)\n" \
    "       string.len(\"llll\")\n\n" \
    "   LuaObject:stateSleep(1)\n" \
    "   LuaObject:stateSet(stateOld + 1)\n" \
    "end\n";


const char *textlua1 =
    "\n\n" \
    "-- script.lua\n" \
    "-- Receives a table, returns the sum of its components.\n" \
    "\n\n" \
    "function main(n)\n" \
    "   print(\"main = \", n)\n" \
    "   n = n + 1\n" \
    "   LuaObject:stateSet(n)\n" \
    "   LuaObject:imageShow(\"TestImageLite.png\")\n" \
    "end\n\n" \
    "print(\"The table the script received has:\\n\")\n\n" \
    "LuaObject:checkPixelsByCord(11,22,33,44,55)\n\n" \
    "x = 0\n" \
    "print(\"Returning data back to C = \", x)\n\n" \
    "main(110)\n\n" \
    "return x\n";

int main(int argc, char *[])
{
#   if defined(_DEBUG)
    static const char *l_dlog = "TestADBSCEdit.rpt";
    ExcHndlInit();
    ExcHndlSetLogFileNameA(l_dlog);
#   endif

    std::string p("myfile.txt");
    std::string s = ((argc > 1) ? textlua1 : textlua0);
    int32_t ret = RunEdit(p, s, true, nullptr);

    std::cout << "Run: " << ret << std::endl;
    return 0;
}
