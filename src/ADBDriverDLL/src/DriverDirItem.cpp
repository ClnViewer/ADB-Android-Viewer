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

/// Redefine to Android/POSIX value -> Type()
#define	ADB_IFMT	 0170000		/* type of file */
#define	ADB_IFIFO	 0010000		/* named pipe (fifo) */
#define	ADB_IFCHR	 0020000		/* character special */
#define	ADB_IFDIR	 0040000		/* directory */
#define	ADB_IFBLK	 0060000		/* block special */
#define	ADB_IFREG	 0100000		/* regular */
#define	ADB_IFLNK	 0120000		/* symbolic link */
#define	ADB_IFSOCK   0140000		/* socket */

static const char *l_strPerm[] =
{
    "---", // 0
    "--x", // 1
    "-w-", // 2
    "-wx", // 3
    "r--", // 4
    "r-x", // 5
    "rw-", // 6
    "rwx"  // 7
};

namespace GameDev
{
    ADBDriver::DirItem::DirItem()
            : type(DriverConst::FileType::FILETYPE_NONE),
              ctxt{ 255, 255, 255, 0 },
              cbg{ 0, 0, 0, 0 },
              permission{ 0U, 0U, 0U },
              pmode(0U) {}

    ADBDriver::DirItem::DirItem(void *ct_, void *cb_)
            : type(DriverConst::FileType::FILETYPE_NONE),
              permission{ 0U, 0U, 0U },
              pmode(0U)
        {
            ctxt = *static_cast<ADBDriver::DirItemColor*>(ct_);
            cbg  = *static_cast<ADBDriver::DirItemColor*>(cb_);
        }

    ADBDriver::DirItem::DirItem(std::string const & s_, void *ct_, void *cb_)
            : s(s_),
              type(DriverConst::FileType::FILETYPE_NONE),
              permission{ 0U, 0U, 0U },
              pmode(0U)
        {
            ctxt = *static_cast<ADBDriver::DirItemColor*>(ct_);
            cbg  = *static_cast<ADBDriver::DirItemColor*>(cb_);
        }

    ADBDriver::DirItem::DirItem(std::string const & s_, std::string const & m_, std::string const & d_, DriverConst::FileType f_, ADBDriver::DirItemColor const & c_)
            : s(s_),
              cmds(m_),
              desc(d_),
              type(f_),
              ctxt(c_),
              cbg{ 0, 0, 0, 0 },
              permission{ 0U, 0U, 0U },
              pmode(0U) {}

    void ADBDriver::DirItem::Permission(uint32_t mode_)
    {
        this->pmode = mode_;
        this->permission[0] = ((mode_ >> 6) & 7);
        this->permission[1] = ((mode_ >> 3) & 7);
        this->permission[2] =  (mode_ & 7);
    }

    std::string ADBDriver::DirItem::Permission()
    {
        std::stringstream ss;

        if (this->permission[0] > 7)
            ss << l_strPerm[0];
        else
            ss << l_strPerm[this->permission[0]];

        if (this->permission[1] > 7)
            ss << l_strPerm[0];
        else
            ss << l_strPerm[this->permission[1]];

        if (this->permission[2] > 7)
            ss << l_strPerm[0];
        else
            ss << l_strPerm[this->permission[2]];

        return ss.str();
    }

    void ADBDriver::DirItem::Type(uint32_t mode_)
    {
        switch (mode_ & ADB_IFMT)
        {
            case ADB_IFREG:
                this->type = DriverConst::FileType::FILETYPE_FILE; break;
            case ADB_IFLNK:
                this->type = DriverConst::FileType::FILETYPE_SYMLINK; break;
            case ADB_IFDIR:
                this->type = DriverConst::FileType::FILETYPE_DIR; break;
            case ADB_IFIFO:
                this->type = DriverConst::FileType::FILETYPE_FIFO; break;
            case ADB_IFCHR:
                this->type = DriverConst::FileType::FILETYPE_CHARDEV; break;
            case ADB_IFBLK:
                this->type = DriverConst::FileType::FILETYPE_BLKDEV; break;
            case ADB_IFSOCK:
                this->type = DriverConst::FileType::FILETYPE_SOCK; break;
            default:
                this->type = DriverConst::FileType::FILETYPE_NONE; break;
        }
    }

    DriverConst::FileType ADBDriver::DirItem::Type()
    {
        return type;
    }

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

