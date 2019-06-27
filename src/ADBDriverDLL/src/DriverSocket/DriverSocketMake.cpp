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

template<typename T>
static std::string f__MakeCmd(T const & cmd)
{
    string_sprintf<std::string> fmt{};
    if constexpr (std::is_same<T, std::wstring>::value)
        return fmt.go("%04x%ls", cmd.length(), cmd.c_str());
    else if constexpr (std::is_same<T, std::string>::value)
        return fmt.go("%04x%s", cmd.length(), cmd.c_str());
    else
        return std::string();
}

template<typename T>
static T f__MakeTarget(std::wstring const & dev)
{
    if constexpr (std::is_same<T, std::wstring>::value)
    {
        std::wstringstream wss;
        if (!dev.empty())
        {
            wss << DriverConst::lw_host_transport << dev;
            return wss.str();
        }
        wss << DriverConst::lw_host_transport_any;
        return wss.str();
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        std::stringstream ss;
        if (!dev.empty())
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cnv;
            ss << DriverConst::ls_host_transport << cnv.to_bytes(dev.c_str());
            return ss.str();
        }
        ss << DriverConst::ls_host_transport_any;
        return ss.str();
    }
    else
        return T();
}

template<typename T>
static bool f__MakeCheck(T & result, bool is8)
{
    if (result.length() < 4U)
        return false;

    if constexpr (std::is_same<T, std::wstring>::value)
    {
        if (wmemcmp(
            result.c_str(),
            DriverConst::lw_const_fail,
            4U) == 0)
            return false;

        if (wmemcmp(
            result.c_str(),
            DriverConst::lw_const_ok,
            4U) != 0)
            return false;
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        if (memcmp(
            result.c_str(),
            DriverConst::ls_const_fail,
            4U) == 0)
            return false;

        if (memcmp(
            result.c_str(),
            DriverConst::ls_const_ok,
            4U) != 0)
            return false;
    }
    else
        return false;

    size_t pos = (((is8) && (result.length() >= 8)) ? 8U :
            ((result.length() >= 4) ? 4U : 0U)
        );
    if (pos)
        result.erase(0, pos);

    return true;
}
