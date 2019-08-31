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
#include <errno.h>

namespace GameDev
{

static const char *synctag[] =
    {
        "OKAY",
        "FAIL",
        "DONE",
        "SEND",
        "RECV",
        "DATA",
        "LIST",
        "DENT",
        "\0\0\0\0"
    };

DriverNet::ADBSYNCDATA DriverNet::SyncDataMake(DriverNet::SyncTagType type, uint32_t sz)
    {
        DriverNet::ADBSYNCDATA data{};
        data.sz = sz;
        ::memcpy(data.id, synctag[type], 4);
        return data;
    }

DriverNet::ADBSYNCDATA DriverNet::SyncDataFromBuff(std::string const & s)
    {
        do
        {
            std::size_t sz = ((s.length() >= 8U) ? 8U :
                              ((s.length() >= 4U) ? 4U : 0U)
                            );
            if ((!sz) || (s.empty()))
                break;

            std::vector<char> data(s.begin(), s.begin() + sz);
            return SyncDataFromBuff(&data[0]);
        }
        while (0);
        return DriverNet::ADBSYNCDATA();
    }

DriverNet::ADBSYNCDATA DriverNet::SyncDataFromBuff(std::wstring const & s)
    {
        do
        {
            if ((s.empty()) || (s.length() < 4U))
                break;

            std::string ssub = string_to_utf8_sub(s, ((s.length() >= 8U) ? 8U : s.length()));
            std::size_t sz = ((ssub.length() >= 8U) ? 8U :
                              ((ssub.length() >= 4U) ? 4U :  0U)
                            );
            if (!sz)
                break;

            std::vector<char> data(ssub.begin(), ssub.begin() + sz);
            return SyncDataFromBuff(&data[0]);
        }
        while (0);
        return DriverNet::ADBSYNCDATA();
    }

DriverNet::ADBSYNCDATA DriverNet::SyncDataFromBuff(char *buf)
    {
        if (!buf)
            return DriverNet::ADBSYNCDATA();
        try
        {
            return *reinterpret_cast<DriverNet::ADBSYNCDATA*>(buf);
        }
        catch (...)
        {
            return DriverNet::ADBSYNCDATA();
        }
    }

template<typename T>
bool DriverNet::SyncCmdSend(SOCKET client, T const & cmd, DriverNet::SyncTagType type)
    {
        std::stringstream ss;

        if constexpr (std::is_same<T, std::string>::value)
        {
            ss << std::hex << std::setw(4) << std::setfill('0') << cmd.length() << cmd.c_str();
        }
        else if constexpr (std::is_same<T, std::wstring>::value)
        {
            std::string cmds = string_to_utf8(cmd);
            ss << std::hex << std::setw(4) << std::setfill('0') << cmds.length() << cmds.c_str();
        }
        return SyncDataSend(client, ss.str(), type);
    }
    template bool DriverNet::SyncCmdSend<std::wstring>(SOCKET, std::wstring const&, DriverNet::SyncTagType);
    template bool DriverNet::SyncCmdSend<std::string> (SOCKET, std::string const&,  DriverNet::SyncTagType);

bool DriverNet::SyncTargetSend(SOCKET client, std::wstring const & target)
{
    std::stringstream ss;
    if (!target.empty())
        ss << DriverConst::ls_host_transport << string_to_utf8(target);
    else
        ss << DriverConst::ls_host_transport_any;

    return SyncCmdSend<std::string>(client, ss.str());
}

template <typename T>
DriverNet::SyncTagType DriverNet::SyncDataType(T const & data)
    {
        for (uint32_t i = 0; (i < (__NELE(synctag) - 1U)); i++)
            if ((synctag[i][0] == data.id[0]) &&
                (synctag[i][1] == data.id[1]) &&
                (synctag[i][2] == data.id[2]) &&
                (synctag[i][3] == data.id[3]))
                return static_cast<DriverNet::SyncTagType>(i);

        return DriverNet::SyncTagType::SYNC_TAG_NONE;
    }
    template DriverNet::SyncTagType DriverNet::SyncDataType<DriverNet::ADBSYNCDATA>(DriverNet::ADBSYNCDATA const&);
    template DriverNet::SyncTagType DriverNet::SyncDataType<DriverNet::ADBSYNCDENT>(DriverNet::ADBSYNCDENT const&);


bool DriverNet::SyncDataSend(SOCKET client, std::string const & data, DriverNet::SyncTagType type)
    {
        if (data.empty())
            return false;
        return SyncDataSend(client, data.c_str(), data.length(), type);
    }

bool DriverNet::SyncDataSend(SOCKET client, const char *data, int32_t dsz, DriverNet::SyncTagType type)
    {
        if (::send(client, data, dsz, 0) == -1)
            return false;
        return SyncDataResponse(client, type);
    }

bool DriverNet::SyncDataResponse(SOCKET client, DriverNet::SyncTagType type)
    {
        DriverNet::ADBSYNCDATA req{};
        return SyncDataResponse(client, req, type);
    }

bool DriverNet::SyncDataResponse(SOCKET client, DriverNet::ADBSYNCDATA & req, DriverNet::SyncTagType type)
    {
        do
        {
            size_t total = 0U;
            std::vector<char> v(256);

            do
            {
                GetNetError(); /// flush network errors
                int32_t rsz;

                if ((rsz = ::recv(client, reinterpret_cast<char*>(&v[total]), 256, 0)) <= 0)
                {
                    if (!rsz)
                        break;
                    else if (rsz == SOCKET_ERROR)
                    {
                        if (!errno)
                            break;
                        else
                            return false;
                    }
                }
                total += static_cast<size_t>(rsz);
                if (total < 4)
                    continue;

                v.resize(total);
                break;
            }
            while (1);

            if (!v.size())
                break;

            req = SyncDataFromBuff(
                    reinterpret_cast<char*>(&v[0])
                );
            if (SyncDataType<DriverNet::ADBSYNCDATA>(req) != type)
                break;
            return true;
        }
        while (0);
        return false;
    }

bool DriverNet::SyncFileSend(SOCKET client, std::string const & src, std::string const & dst)
    {
        do
        {
            DriverNet::ADBSYNCDATA req;

            if (!SyncCmdSend<std::string>(client, DriverConst::ls_cmd_sunc))
                break;

            req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_SEND, dst.length());

            if ((::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req))) ||
                (::send(client, dst.data(), dst.length(), 0) < static_cast<int32_t>(dst.length())))
                break;

            int32_t rsz = 0;
            auto dbuf = std::make_unique<char[]>(64000);
            FILE __AUTO(__autofile) * fp = ::fopen(src.c_str(), "rb");
            if (!fp)
                break;

            do
            {
                if ((rsz = ::fread(dbuf.get(), 1, 64000, fp)) <= 0)
                    break;

                req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_DATA, static_cast<uint32_t>(rsz));
                if (::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req)))
                    break;

                if ((rsz = ::send(client, dbuf.get(), rsz, 0)) <= 0)
                    break;

            }
            while (rsz > 0);

            if (!rsz)
            {
                req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_DONE, static_cast<uint32_t>(::time(NULL)));
                if (!SyncDataSend(client, (const char*)&req, sizeof(req), DriverNet::SyncTagType::SYNC_TAG_OKAY))
                    break;
            }
            else if (rsz < 0)
                if (GetNetError())
                    break;

            return true;
        }
        while (0);
        return false;
    }

bool DriverNet::SyncFileReceive(SOCKET client, std::string const & src, std::string const & dst)
    {
        do
        {
            DriverNet::ADBSYNCDATA req;

            if (!SyncCmdSend<std::string>(client, DriverConst::ls_cmd_sunc))
                break;

            req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_RECV, src.length());

            if ((::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req))) ||
                (::send(client, src.data(), src.length(), 0) < static_cast<int32_t>(src.length())))
                break;

            bool    isbegin = true,
                    isend   = false;
            int32_t rsz     = 0,
                    asz     = 0;

            auto dbuf = std::make_unique<char[]>(64000);
            FILE __AUTO(__autofile) * fp = ::fopen(dst.c_str(), "wb");
            if (!fp)
                break;

            do
            {
                if ((rsz = ::recv(client, dbuf.get(), 64000, 0)) <= 0)
                {
                    if (rsz == SOCKET_ERROR)
                    {
                        if (!errno)
                            break;
                        return false;
                    }
                    else if (!rsz) /// detail test this!! (loop?)
                        continue;
                    break;
                }

                int32_t offset = 0;
                char *buff = dbuf.get();
                req = SyncDataFromBuff(buff);

                switch (SyncDataType<DriverNet::ADBSYNCDATA>(req))
                {
                    case DriverNet::SyncTagType::SYNC_TAG_DATA:
                        {
                            offset  = sizeof(DriverNet::ADBSYNCDATA);
                            asz     = static_cast<int32_t>(req.sz);
                            isbegin = false;

                            if (asz <= 0)
                            {
                                /// error is file empty
                                rsz = -1;
                            }
                            else if (asz < rsz)
                            {
                                req = SyncDataFromBuff(buff + asz + offset);
                                if (SyncDataType<DriverNet::ADBSYNCDATA>(req) == DriverNet::SyncTagType::SYNC_TAG_DONE)
                                    isend = true;
                                rsz = asz;
                            }
                            else
                                rsz -= offset;
                            break;
                        }
                    case DriverNet::SyncTagType::SYNC_TAG_DONE:
                        {
                            /// is file empty, no receive SYNC_TAG_DATA,
                            /// receive immediately SYNC_TAG_DONE
                            rsz = 0; isend = true;
                            break;
                        }
                    default:
                        {
                            if (isbegin)
                                return false;

                            if (asz < rsz)
                            {
                                req = SyncDataFromBuff(buff + asz);
                                if (SyncDataType<DriverNet::ADBSYNCDATA>(req) == DriverNet::SyncTagType::SYNC_TAG_DONE)
                                    isend = true;
                                rsz = asz;
                            }
                            break;
                        }
                }

                if ((!rsz) && (!isend))
                    continue;
                else if (rsz < 0)
                    break;
                else if (rsz > 0)
                {
                    ::fwrite(buff + offset, 1U, static_cast<size_t>(rsz), fp);
                    asz -= rsz;
                }
                if ((isend) || (asz <= 0))
                {
                    rsz = 0; break;
                }

                req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_OKAY, 0U);
                if (::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req)))
                    break;
            }
            while ((rsz >= 0) && (asz > 0));

            if (asz)
                break;

            if (rsz < 0)
                if (GetNetError())
                    break;
            return true;
        }
        while (0);
        return false;
    }

bool DriverNet::SyncDirList(
        SOCKET client,
        std::string const & dst,
        void *v,
        std::function<dir_list_cb> fun)
    {
#       define LOOP_BREAK \
                { iscontinue = false; break; }
        do
        {
            DriverNet::ADBSYNCDATA req{};

            if (!SyncCmdSend<std::string>(client, DriverConst::ls_cmd_sunc))
                break;

            req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_LIST, dst.length());

            if ((::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req))) ||
                (::send(client, dst.data(), dst.length(), 0) < static_cast<int32_t>(dst.length())))
                break;

            int32_t rsz = 0;
            bool    iscomplette = false;
            auto    dbuf = std::make_unique<char[]>(8192);

            do
            {
                bool iscontinue = true;
                DriverNet::ADBSYNCDENT dent{};

                if ((rsz = ::recv(client, dbuf.get(), sizeof(dent), 0)) != sizeof(dent))
                    break;

                (void) ::memcpy(&dent, dbuf.get(), sizeof(dent));
                DriverNet::SyncTagType type = SyncDataType<DriverNet::ADBSYNCDENT>(dent);

                switch (type)
                {
                    case DriverNet::SyncTagType::SYNC_TAG_DENT:
                        {
                            if ((!dent.esize) || (dent.esize > (UINT_MAX - 1)))
                                LOOP_BREAK;
                            if ((rsz = ::recv(client, dbuf.get(), dent.esize, 0)) != static_cast<int32_t>(dent.esize))
                                LOOP_BREAK;

                            std::string entry(dbuf.get(), dbuf.get() + rsz);
                            if (!fun(dent, entry, v))
                                LOOP_BREAK;

                            req = SyncDataMake(DriverNet::SyncTagType::SYNC_TAG_OKAY, static_cast<uint32_t>(rsz));
                            if (::send(client, (const char*)&req, sizeof(req), 0) < static_cast<int32_t>(sizeof(req)))
                                LOOP_BREAK;
                            break;
                        }
                    case DriverNet::SyncTagType::SYNC_TAG_DONE:
                        {
                            iscomplette = true;
                            break;
                        }
                    default:
                        {
                            LOOP_BREAK;
                        }
                }
                if ((iscomplette) || (!iscontinue))
                    break;
            }
            while (rsz > 0);

            if (iscomplette)
                return true;
        }
        while (0);
        return false;
    }

template<typename T>
bool DriverNet::SyncVectorReceive(SOCKET client, T const & cmd, std::vector<uint8_t> & v)
    {
        do
        {
            if (!SyncCmdSend<T>(client, cmd))
                break;

            int32_t rsz;
            size_t  total = 0U;
            v.resize(4096);

            while ((rsz = ::recv(client, reinterpret_cast<char*>(&v[total]), 4096, 0)) > 0)
            {
                total += static_cast<size_t>(rsz);
                v.resize(total + 4096);
            }
            v.resize(total);

            if (rsz < 0)
                if (GetNetError())
                    break;

            return true;
        }
        while (0);
        return false;
    }
    template bool DriverNet::SyncVectorReceive<std::wstring>(SOCKET, std::wstring const&, std::vector<uint8_t>&);
    template bool DriverNet::SyncVectorReceive<std::string> (SOCKET, std::string const&,  std::vector<uint8_t>&);

template<typename T>
bool DriverNet::SyncStringReceive(SOCKET client, T const & cmd, T & result)
    {
        result.clear();

        do
        {
            if (!SyncCmdSend<T>(client, cmd))
                break;

            int32_t rsz;
            auto buf = std::make_unique<char[]>(1024);

            while ((rsz = ::recv(client, buf.get(), 1024, 0)) > 0)
                result.append(buf.get(), buf.get() + rsz);

            if (rsz < 0)
                if (GetNetError())
                    break;
            return true;
        }
        while (0);
        return false;
    }
    template bool DriverNet::SyncStringReceive<std::wstring>(SOCKET, std::wstring const&, std::wstring&);
    template bool DriverNet::SyncStringReceive<std::string> (SOCKET, std::string const&,  std::string&);
}
