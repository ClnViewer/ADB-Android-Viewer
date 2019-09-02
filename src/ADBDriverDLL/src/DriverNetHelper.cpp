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

namespace GameDev
{
    ///
    /// ostream << helper operator to class TraceFunction::instance().args(...)
    ///
#   if (defined(_BUILD_TRACE) && (_BUILD_TRACE == 1))

    /// ADBSYNCPKT print helper
    std::ostream & operator << (std::ostream & os, const DriverNet::ADBSYNCPKT & pkt)
    {
        os << " { " << pkt.rsz <<"/" << pkt.asz;
        if (pkt.buff)
        {
            os << ", [";
            if (pkt.rsz >= 4)
            {
                os << pkt.buff[0] << "|" << pkt.buff[1] << "|" << pkt.buff[2] << "|" << pkt.buff[3];
                if ((pkt.asz) && ((pkt.asz + 4) <= pkt.rsz))
                    os << "|...|" << pkt.buff[(pkt.asz + 1)] << "|" << pkt.buff[(pkt.asz + 2)] << "|" << pkt.buff[(pkt.asz + 3)] << "|"  << pkt.buff[(pkt.rsz + 4)];
            }
            else
            {
                os << "size mismatch";
            }
            os << "]";
        }
        else
        {
            os << ", [null]";
        }
        os << " }";
        return os;
    }
    /// ADBSYNCDATA print helper
    std::ostream & operator << (std::ostream & os, const DriverNet::ADBSYNCDATA & data)
    {
        os << " { " << data.sz << ", [";
        os << data.id[0] << "|" << data.id[1] << "|" << data.id[2] << "|" << data.id[3] << "] }";
        return os;
    }
#   endif

}
