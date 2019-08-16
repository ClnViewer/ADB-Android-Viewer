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

std::vector<uint8_t> ADBDriver::Capture()
{
    return Capture(CapType::CAP_RAW, 0U, 0U);
}

std::vector<uint8_t> ADBDriver::Capture(CapType cap, uint32_t capratio, uint32_t caprotate)
{
    bool isAcap = false;
    SOCKET l_client = INVALID_SOCKET;
    std::vector<uint8_t> v;

    if ((l_client = m_net.Connect()) == INVALID_SOCKET)
        return v;

    do
    {
        if (!f__SessionSendTarget(l_client, m_deviceid))
            break;

        std::stringstream ss{};

        switch (cap)
        {
            case CAP_RAW:
            case CAP_PNG:
                {
                    ss << DriverConst::ls_cmd_exec << DriverConst::ls_name_screencap;
                    break;
                }
            case CAP_ACAP_BMP:
            case CAP_ACAP_STREAM:
                {
                    ss << DriverConst::ls_cmd_exec << DriverConst::ls_path_dir << "/" << DriverConst::ls_name_ascreencap;
                    if (capratio)
                        ss << DriverConst::ls_cmd_ascreencap_ratio << std::to_string(capratio);
                    if (caprotate)
                        ss << DriverConst::ls_cmd_ascreencap_rotate << std::to_string(caprotate);
                    isAcap = true;
                    break;
                }
        }
        switch (cap)
        {
            case CAP_RAW:
                {
                    break;
                }
            case CAP_PNG:
                {
                    ss << DriverConst::ls_cmd_screencap_jpeg;
                    break;
                }
            case CAP_ACAP_BMP:
                {
                    ss << DriverConst::ls_cmd_ascreencap_stdout;
                    break;
                }
            case CAP_ACAP_STREAM:
                {
                    ss << DriverConst::ls_cmd_ascreencap_stream;
                    break;
                }
        }
        if (!f__SessionReceiveVector(l_client, ss.str(), v))
            v.clear();
    }
    while (0);

    if (!isAcap)
    {
        if (v.size() > 12)
        {
            m_display[0] = ((m_display[0] != *(uint32_t*)&v[0]) ? *(uint32_t*)&v[0] : m_display[0]);
            m_display[1] = ((m_display[1] != *(uint32_t*)&v[4]) ? *(uint32_t*)&v[4] : m_display[1]);
            m_display[2] = ((m_display[2] != *(uint32_t*)&v[8]) ? *(uint32_t*)&v[8] : m_display[2]);
            m_display[3] = static_cast<uint32_t>(m_display[0] * m_display[1] * 4 + 12);

            if (v.size() != m_display[3])
                v.clear();
            else
                v.erase(v.begin(), v.begin()+12);
        }
        else
        {
            v.clear();
        }
    }

    m_net.Close(l_client);
    return v;
}

bool ADBDriver::Video(
        uint32_t capratio, uint32_t cappack, std::function<bool(std::vector<uint8_t>&, uint32_t, uint32_t)> fun
    )
{
    return Video(capratio, 0U, cappack, fun);
}

bool ADBDriver::Video(
        uint32_t capratio, uint32_t caprotate, uint32_t cappack, std::function<bool(std::vector<uint8_t>&, uint32_t, uint32_t)> fun
    )
{
    SOCKET l_client = INVALID_SOCKET;

    do
    {
        std::string cmds;
        std::stringstream ss;

        if ((l_client = m_net.Connect()) == INVALID_SOCKET)
            break;

        if (!f__SessionSendTarget(l_client, m_deviceid))
            break;

        ss << DriverConst::ls_cmd_exec << DriverConst::ls_path_dir << "/" << DriverConst::ls_name_ascreencap;
        ss << DriverConst::ls_cmd_ascreencap_stream;

        if (capratio)
            ss << DriverConst::ls_cmd_ascreencap_ratio << std::to_string(capratio);
        if (caprotate)
            ss << DriverConst::ls_cmd_ascreencap_rotate << std::to_string(caprotate);
        if (cappack)
            ss << DriverConst::ls_cmd_ascreencap_pack << std::to_string(cappack);

        cmds = f__MakeCmd<std::string>(ss.str());
        if (send(l_client, cmds.c_str(), cmds.length(), 0) == -1)
            break;

        bool loop = true;
        int32_t rsz,
                hsz = sizeof(DriverNet::STREAMHEADER);
        auto hbuf = std::make_unique<char[]>(sizeof(DriverNet::STREAMHEADER));

        if ((rsz = recv(l_client, hbuf.get(), 4, 0)) < 4)
            break;

        std::string data(hbuf.get(), rsz);
        if (memcmp(
                data.c_str(),
                DriverConst::ls_const_fail,
                4U) == 0)
            break;

        do
        {
            if ((rsz = recv(l_client, hbuf.get(), hsz, 0)) != hsz)
                break;

            DriverNet::STREAMHEADER *sh = reinterpret_cast<DriverNet::STREAMHEADER*>(hbuf.get());

            if (
                (sh->id != BMZ_MAGIC) ||
                (!sh->usz) ||
                (!sh->csz) ||
                (!sh->w) || (!sh->h)
               )
                break;

            size_t total = 0;
            std::vector<uint8_t> v(sh->csz);

            while ((rsz = recv(l_client, reinterpret_cast<char*>(&v[total]), (sh->csz - total), 0)) > 0)
            {
                total += static_cast<size_t>(rsz);
                if (total >= sh->csz)
                    break;
            }

            if (total != sh->csz)
                break;

            auto tbuf = std::make_unique<uint8_t[]>(sh->usz + 1U);

            if ((rsz = LZ4_decompress_safe(
                        reinterpret_cast<const char*>(&v[0]),
                        reinterpret_cast<char*>(tbuf.get()),
                        static_cast<int>(v.size()),
                        static_cast<int>(sh->usz + 1U)
                        )) <= 0)
                break;

            v.clear();
            v.assign(tbuf.get(), tbuf.get() + sh->usz);

            loop = fun(v, sh->w, sh->h);

            v.clear();
        }
        while (loop);

        m_net.Close(l_client);
        return !(loop);

    }
    while (0);

    m_net.Close(l_client);
    return false;
}

