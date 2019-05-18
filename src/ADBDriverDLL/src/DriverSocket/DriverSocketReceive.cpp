
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
