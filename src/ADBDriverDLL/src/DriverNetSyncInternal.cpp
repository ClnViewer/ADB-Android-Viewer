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
#include "Utils/extTraceFunction.h"
#include <errno.h>

#define ADBSYNCDATA_SIZE  static_cast<int32_t>(sizeof(DriverNet::ADBSYNCDATA))
#define ADBSYNCID_SIZE    static_cast<int32_t>(sizeof(((DriverNet::ADBSYNCDATA*)0)->id))

#define TRACE_DBG_STR_0 "pkt.rsz = %d, pkt.asz = %d"
#define TRACE_DBG_STR_1 "ecode = %d, errno = %d, error: %s"
#define TRACE_DBG_STR_2 ", errno = %d, error: %s"

namespace GameDev
{

DriverNet::SyncTagType DriverNet::receiveData(SOCKET client, std::unique_ptr<char[]> & dbuf, DriverNet::ADBSYNCPKT & pkt)
    {
        tracer();

        struct timeval tms{};
        fd_set  fds{};

        FD_ZERO(&fds);
        FD_SET(client, &fds);
        tms.tv_sec = 5;
        tms.tv_usec = 0;

        errno = 0;
        pkt.buff = nullptr;

        switch (::select(client + 1, &fds, nullptr, nullptr, &tms))
        {
            case SOCKET_ERROR:
                {
                    if (DriverNet::GetNetBlock())
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_CONTINUE);

                    pkt.rsz = -2;
                    trace_info(
                        TRACE_DBG_STR_1,
                        pkt.rsz, errno,
                        ((errno) ? strerror(errno) : "-")
                    );
                    trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                }
            case 0: /// timeout
                {
                    if (!errno)
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_END);

                    pkt.rsz = -3;
                    trace_info(
                        TRACE_DBG_STR_1,
                        pkt.rsz, errno,
                        ((errno) ? strerror(errno) : "-")
                    );
                    trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                }
            default:
                {
                    switch ((pkt.rsz = ::recv(client, dbuf.get(), 65536, 0)))
                    {
                        case 0: /// disconnect
                            {
                                pkt.rsz = -4;
                                trace_info(
                                    TRACE_DBG_STR_1,
                                    pkt.rsz, errno,
                                    ((errno) ? strerror(errno) : "-")
                                );
                                trace_return(DriverNet::SyncTagType::SYNC_TAG_END);
                            }
                        case SOCKET_ERROR:
                            {
                                if (DriverNet::GetNetBlock())
                                    trace_return(DriverNet::SyncTagType::SYNC_TAG_CONTINUE);

                                pkt.rsz = -1;
                                trace_info(
                                    TRACE_DBG_STR_1,
                                    pkt.rsz, errno,
                                    ((errno) ? strerror(errno) : "-")
                                );
                                trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                            }
                        default:
                            {
                                if (pkt.rsz > 0)
                                {
                                    pkt.buff = dbuf.get();
                                    trace_return(DriverNet::SyncTagType::SYNC_TAG_OKAY);
                                }
                                trace_return(DriverNet::SyncTagType::SYNC_TAG_END);
                            }
                    }
                    break;
                }
        }
    }

bool DriverNet::receivedDataWrite(FILE *fpp, DriverNet::ADBSYNCPKT & pkt)
    {
        tracer();

        if (!pkt.buff)
            trace_return(false);

        errno = 0;
        size_t sz = static_cast<size_t>(pkt.rsz);

        if (::fwrite(pkt.buff, 1, sz, fpp) != sz)
            trace_return(false);

        pkt.asz -= pkt.rsz;
        trace_info(
            TRACE_DBG_STR_0 TRACE_DBG_STR_2,
            pkt.rsz, pkt.asz, errno,
            ((errno) ? strerror(errno) : "-")
        );
        trace_return(true);
    }

DriverNet::SyncTagType DriverNet::parseDataTag(DriverNet::ADBSYNCPKT & pkt, DriverNet::ADBSYNCDATA const & req)
    {
        tracer();

        do
        {
            if (req.sz >= (UINT_MAX - 1))
            {
                trace_break(pkt.rsz = -9);
            }

            pkt.asz = static_cast<int32_t>(req.sz);
            trace_info(
                TRACE_DBG_STR_0 ", req.sz = %u",
                pkt.rsz, pkt.asz, req.sz
            );

            if (pkt.asz <= 0)
            {
                trace_break(pkt.rsz = -5);
            }

            if (pkt.rsz < 0)
            {
                trace_break(pkt.rsz = -7);
            }
            else if (pkt.rsz == 0)
            {
                trace_return(DriverNet::SyncTagType::SYNC_TAG_END);
            }
            else if (pkt.rsz <= ADBSYNCDATA_SIZE)
                trace_return(DriverNet::SyncTagType::SYNC_TAG_CONTINUE);

            pkt.rsz  -= ADBSYNCDATA_SIZE;
            pkt.buff += ADBSYNCDATA_SIZE;

            trace_info(
                TRACE_DBG_STR_0 ", buffer offset = %d",
                pkt.rsz, pkt.asz, ADBSYNCDATA_SIZE
            );
            trace_return(DriverNet::SyncTagType::SYNC_TAG_OKAY);
        }
        while (0);
        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
    }

DriverNet::SyncTagType DriverNet::parseData(SOCKET client, FILE *fpp, DriverNet::ADBSYNCPKT & pkt, bool isrecursive)
    {
        tracer();

        DriverNet::ADBSYNCDATA req{};
        DriverNet::SyncTagType type;

        if (!pkt.buff)
            trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);

        if (pkt.rsz < ADBSYNCDATA_SIZE)
            type = DriverNet::SyncTagType::SYNC_TAG_NONE;
        else
        {
            req  = SyncDataFromBuff(pkt.buff);
            type = trace_call(SyncDataType<DriverNet::ADBSYNCDATA>, req);
        }

        trace_info(
            "switch -> " TRACE_DBG_STR_0 ", case = %s",
            pkt.rsz, pkt.asz,
            enumtostring(type)
        );

        switch (type)
        {
            case DriverNet::SyncTagType::SYNC_TAG_DATA:
                {
                    if ((!isrecursive) && (!sendOkay(client)))
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);

                    type = trace_call(parseDataTag, pkt, req);
                    if (type == DriverNet::SyncTagType::SYNC_TAG_OKAY)
                        trace_break(0);

                    trace_info(
                        "warning: parseDataTag return (%s), no tagged",
                        enumtostring(type)
                    );
                    if (isrecursive)
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                    trace_return(type);
                }
            case DriverNet::SyncTagType::SYNC_TAG_DONE:
                {
                    pkt.rsz = pkt.asz = 0;
                    trace_return(DriverNet::SyncTagType::SYNC_TAG_END);
                }
            case DriverNet::SyncTagType::SYNC_TAG_NONE:
                {
                    if (pkt.asz <= 0)
                    {
                        pkt.rsz = -6;
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                    }
                    if (pkt.rsz < 0)
                    {
                        pkt.rsz = -9;
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                    }
                    else if (pkt.rsz == 0)
                    {
                        trace_return(DriverNet::SyncTagType::SYNC_TAG_CONTINUE);
                    }
                    break;
                }
            default:
                {
                    trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
                }
        }

        trace_info(
            "switch -> " TRACE_DBG_STR_0 ", calc = %d",
            pkt.rsz, pkt.asz, (pkt.rsz - pkt.asz - ADBSYNCID_SIZE)
        );

        if (pkt.rsz <= pkt.asz)
        {
            if (!receivedDataWrite(fpp, pkt))
                trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
            trace_return(DriverNet::SyncTagType::SYNC_TAG_CONTINUE);
        }

        int32_t asz = pkt.asz,
                rsz = (pkt.rsz - asz);
        pkt.rsz = asz;

        if (!receivedDataWrite(fpp, pkt))
            trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);

        if ((rsz - ADBSYNCDATA_SIZE) >= 0)
        {
            pkt.rsz = rsz;
            pkt.asz = rsz;
            pkt.buff += asz;

            /*
                /// Debug last block:
                std::string fname("__SyncFileReceive_bin_");
                fname += std::to_string(cnt++);
                FILE __AUTO(__autofile) * fp = ::fopen(fname.c_str(), "ab+");
                fwrite(pkt.buff, 1, pkt.rsz, fp);
            */

            return trace_call(parseData, client, fpp, pkt, true);
        }
        trace_return(DriverNet::SyncTagType::SYNC_TAG_ERROR);
    }

bool DriverNet::sendOkay(SOCKET client)
    {
        tracer();

        DriverNet::ADBSYNCDATA req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_OKAY, 0U);
        bool ret = (::send(client, (const char*)&req, sizeof(req), 0) == static_cast<int32_t>(sizeof(req)));
        trace_return(ret);
    }

bool DriverNet::receiveErrors(DriverNet::ADBSYNCPKT & pkt, std::string & rs)
    {
#       if defined(_BUILD_WARNING_0)
        switch (pkt.rsz)
        {
            case -1:
                rs = DriverNet::GetNetError(__LINE__); break;
            case -2:
                rs.assign(DriverConst::ls_errorSFR2); break;
            case -3:
                rs.assign(DriverConst::ls_errorSFR3); break;
            case -4:
                rs.assign(DriverConst::ls_errorSFR4); break;
            case -5:
                rs.assign(DriverConst::ls_errorSFR5); break;
            case -6:
                rs.assign(DriverConst::ls_errorSFR6); break;
            case -7:
                rs.assign(DriverConst::ls_errorSFR7); break;
            case -8:
                rs.assign(DriverConst::ls_errorSFR8); break;
            case -9:
                rs.assign(DriverConst::ls_errorSFR9); break;
            case -10:
                rs.assign(DriverConst::ls_errorSFR10); break;
            case 0:
                {
                    if (pkt.asz)
                        rs.assign(DriverConst::ls_errorSFR00);
                    else
                        rs.assign(DriverConst::ls_errorSFR0);
                    break;
                }
        }
#       endif

        return !((pkt.rsz < 0) || (pkt.asz));
    }

}
