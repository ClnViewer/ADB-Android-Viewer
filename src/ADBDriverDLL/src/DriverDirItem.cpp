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
    ADBDriver::DirItem::DirItem()
            : type(ADBDriver::FileType::FILETYPE_NONE),
              ctxt{ 255, 255, 255, 0 },
              cbg{ 0, 0, 0, 0 },
              permission(0) {}

    ADBDriver::DirItem::DirItem(void *ct_, void *cb_)
            : type(ADBDriver::FileType::FILETYPE_NONE),
              permission(0)
        {
            ctxt = *static_cast<ADBDriver::DirItemColor*>(ct_);
            cbg  = *static_cast<ADBDriver::DirItemColor*>(cb_);
        }

    ADBDriver::DirItem::DirItem(std::string const & s_, void *ct_, void *cb_)
            : s(s_),
              type(ADBDriver::FileType::FILETYPE_NONE),
              permission(0)
        {
            ctxt = *static_cast<ADBDriver::DirItemColor*>(ct_);
            cbg  = *static_cast<ADBDriver::DirItemColor*>(cb_);
        }

    ADBDriver::DirItem::DirItem(std::string const & s_, std::string const & m_, std::string const & d_, ADBDriver::FileType f_, ADBDriver::DirItemColor const & c_)
            : s(s_),
              cmds(m_),
              desc(d_),
              type(f_),
              ctxt(c_),
              cbg{ 0, 0, 0, 0 },
              permission(0) {}

    void ADBDriver::DirItem::Description(
            std::string  const & as_,
            std::string  const & bs_,
            std::string  const & cs_,
            ADBDriver::DirItemColor const & color_)
        {
            std::stringstream ss;
            if (!as_.empty())
                ss << as_ << " ";
            if (!bs_.empty())
                ss << bs_ << " ";
            if (!cs_.empty())
                ss << "(" << cs_ << ")";

            this->ctxt = color_;
            this->desc = ss.str();
        }
};

