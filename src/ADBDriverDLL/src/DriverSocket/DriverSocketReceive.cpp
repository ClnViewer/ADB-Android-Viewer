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
static bool f__SessionReceiveString(int stage, SOCKET client, T & cmd, T & result)
{
    result.clear();

    std::string cmds = f__MakeCmd<T>(cmd);
    if (send(client, cmds.c_str(), cmds.length(), 0) < 0)
        return false;

    int rsz;
    auto buf = std::make_unique<char[]>(1024);

    while ((rsz = recv(client, buf.get(), 1024, 0)) > 0)
    {
        result.append(buf.get(), buf.get() + rsz);
        if ((!stage) && (rsz == 4))
            break;
    }
    if (rsz < 0)
    {
        if (DriverNet::GetNetError())
            return false;
    }
    return true;
}

static bool f__SessionReceiveVector(SOCKET client, std::string const & cmd, std::vector<uint8_t> & v)
{
    if (!f__SessionSendCmd(client, cmd))
        return false;

    int32_t rsz;
    size_t  total = 0U;
    v.resize(4096);

    while ((rsz = recv(client, reinterpret_cast<char*>(&v[total]), 4096, 0)) > 0)
    {
        total += static_cast<size_t>(rsz);
        v.resize(total + 4096);
    }

    v.resize(total);

    if (rsz < 0)
    {
        if (DriverNet::GetNetError())
            return false;
    }
    return true;
}
