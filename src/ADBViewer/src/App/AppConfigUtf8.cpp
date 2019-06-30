/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

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

#include "../ADBViewer.h"

static inline bool l_IsUtf8Char(const uint8_t *bytes)
{
    if ((*bytes & 0x80) == 0x00)
    {
        if (
            ((*bytes & 0x7F) >= 0x0020) &&
            ((*bytes & 0x7F) <= 0x007F)
           )
            return false;
        return true;
    }
    else if (
        ((*bytes & 0xE0) == 0xC0) ||
        ((*bytes & 0xF0) == 0xE0) ||
        ((*bytes & 0xF8) == 0xF0)
             )
        return true;
    else
        return false;
}

static inline bool l_IsUtf8Chars(const uint8_t **ibytes, int32_t *pnum)
{
    uint32_t cp;
    int32_t  num;
    *pnum = 0;

    const uint8_t *bytes = *ibytes;

    if ((*bytes & 0x80) == 0x00)
    {
        // U+0000 to U+007F
        cp = (*bytes & 0x7F);
        num = 1;
    }
    else if ((*bytes & 0xE0) == 0xC0)
    {
        // U+0080 to U+07FF
        cp = (*bytes & 0x1F);
        num = 2;
    }
    else if ((*bytes & 0xF0) == 0xE0)
    {
        // U+0800 to U+FFFF
        cp = (*bytes & 0x0F);
        num = 3;
    }
    else if ((*bytes & 0xF8) == 0xF0)
    {
        // U+10000 to U+10FFFF
        cp = (*bytes & 0x07);
        num = 4;
    }
    else
        return false;

    bytes += 1;

    for (int i = 1; i < num; ++i)
    {
        if ((*bytes & 0xC0) != 0x80)
            return false;
        cp = (cp << 6) | (*bytes & 0x3F);
        bytes += 1;
    }

    if ((cp > 0x10FFFF)  ||
        ((cp >= 0xD800)  && (cp <= 0xDFFF)) ||
        ((cp <= 0x007F)  && (num != 1)) ||
        ((cp >= 0x0080)  && (cp <= 0x07FF)   && (num != 2)) ||
        ((cp >= 0x0800)  && (cp <= 0xFFFF)   && (num != 3)) ||
        ((cp >= 0x10000) && (cp <= 0x1FFFFF) && (num != 4)))
        return false;

    *ibytes = bytes;
    *pnum = num;
    return true;
}


bool AppConfig::IsUtf8c(std::string const & s)
{
    const uint8_t *bytes = reinterpret_cast<const uint8_t*>(s.data());
    return l_IsUtf8Char(bytes);
}

bool AppConfig::IsUtf8(std::string const & s)
{
    int32_t num;
    return IsUtf8(s, &num);
}

bool AppConfig::IsUtf8(std::string const & s, int32_t *sz)
{
    if (s.empty())
        return true;

    const uint8_t *bytes = reinterpret_cast<const uint8_t*>(s.data());
    int32_t num = 0, cnum = 0;
    if (sz)
        *sz = 0;

    while (*bytes != 0x00)
    {
        if (!l_IsUtf8Chars(&bytes, &cnum))
            return false;
        num = std::max(num, cnum);
    }
    if (sz)
        *sz = num;
    return true;
}
