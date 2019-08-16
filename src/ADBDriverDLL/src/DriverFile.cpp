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

#include "DriverInternal.h"
#include "DriverConst.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace GameDev
{

static const char *l_arraySize[] =
    {
        "Tb",
        "Gb",
        "Mb",
        "Kb",
        "Bytes"
    };

static std::string f_CalcSize(uintmax_t sz)
    {
        std::stringstream ss;
        double b = sz;
        double k = (sz    / 1024.0);
        double m = ((sz   / 1024.0) / 1024.0);
        double g = (((sz  / 1024.0) / 1024.0) / 1024.0);
        double t = ((((sz / 1024.0) / 1024.0) / 1024.0) / 1024.0);

        if (t > 1)
            ss << t << " " << l_arraySize[0];
        else if (g > 1)
            ss << g << " " << l_arraySize[1];
        else if (m > 1)
            ss << m << " " << l_arraySize[2];
        else if (k > 1)
            ss << k << " " << l_arraySize[3];
        else
            ss << b << " " << l_arraySize[4];

        return ss.str();
    }

static std::string f_CalcSize(std::string const & s)
    {
        try
        {
            uintmax_t sz = std::stoull(s);
            if (sz)
                return f_CalcSize(sz);
            return std::string();
        }
        catch (...)
        {
            return std::string();
        }
    }

std::string ADBDriver::permission(GameDev::ADBDriver::FilePermissionType fp)
    {
        return std::to_string(DriverConst::ls_arrayModeFile[static_cast<int32_t>(fp)]);
    }

std::string ADBDriver::PermissionFile(GameDev::ADBDriver::FilePermissionType fp)
    {
        std::stringstream ss;
        ss << "0" << permission(fp).c_str();
        return ss.str();
    }

std::string ADBDriver::SizeFile(uintmax_t sz)
    {
        return f_CalcSize(sz);
    }

std::string ADBDriver::SizeFile(std::string const & s)
    {
        return f_CalcSize(s);
    }

bool ADBDriver::SendFile(std::string const & src, std::string const & dst, GameDev::ADBDriver::FilePermissionType fp)
    {
        do
        {
            if (::_access(src.c_str(), F_OK) < 0)
                break;

            std::string fname = GameDev::filename<std::string>(src);
            if (fname.empty())
                break;

            std::stringstream ss;
            ss << dst.c_str() << "/" << fname.c_str();
            ss << ",0" << permission(fp).c_str();

            if (!sendfileraw(src, ss.str()))
                break;

            return true;
        }
        while (0);
        return false;
    }

bool ADBDriver::ChmodFile(std::string const & dst, GameDev::ADBDriver::FilePermissionType fp, std::string & rs)
    {
        std::stringstream ss;
        ss << DriverConst::ls_cmod_file;
        ss << permission(fp).c_str() << " ";
        ss << dst.c_str();
        bool ret = AdbRawT<std::string>(ss.str(), DriverConst::ls_cmd_shell, rs, false);
        if (!rs.empty())
            clearend(rs);
        return ret;
    }

};

