
template<typename T>
bool ADBDriver::AdbRawT(T cmd, T const & addtype, T & result)
{
    bool ret = false;
    SOCKET l_client = INVALID_SOCKET;
    result.clear();

    if ((l_client = m_net.Connect()) == INVALID_SOCKET)
        return false;

    do
    {
        if (!f__SessionSendTarget(l_client, m_deviceid))
            break;

        cmd.insert(0, addtype.c_str());

        if (!f__SessionReceiveString<T>(1, l_client, cmd, result))
            break;

        if (!f__MakeCheck<T>(result))
            break;

        ret = true;
    }
    while (0);

    m_net.Close(l_client);
    return ret;
}
template bool ADBDriver::AdbRawT<std::wstring>(std::wstring, std::wstring const &, std::wstring&);
template bool ADBDriver::AdbRawT<std::string>(std::string, std::string  const &, std::string&);

