
#include "DriverInternal.h"

namespace GameDev
{

std::wstring ADBDriver::Manager(const std::wstring & cmd)
{
    HANDLE hOutR, hOutW;
    STARTUPINFOW si;
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&sa, sizeof(sa));

    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (
        (!CreatePipe(&hOutR, &hOutW, &sa, 0U)) ||
        (!SetHandleInformation(hOutR, HANDLE_FLAG_INHERIT, 0))
    )
        return std::wstring();

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    si.cb = sizeof(si);
    si.hStdInput = NULL;
    si.hStdOutput = hOutW;
    si.hStdError = hOutW;
    si.dwFlags = STARTF_USESTDHANDLES;

    std::wstring adbcmd = m_exepath + L" " + cmd;

    BOOL ret = CreateProcessW(
                   m_exepath.c_str(),
                   const_cast<wchar_t*>(adbcmd.c_str()),
                   NULL,
                   NULL,
                   TRUE,
                   DETACHED_PROCESS | CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW,
                   NULL,
                   NULL,
                   &si,
                   &pi
               );

    CloseHandle(hOutW);
    std::wstring data;

    do
    {
        if (!ret)
            break;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        DWORD br;
        auto buf = std::make_unique<char[]>(256);

        while (
               (ReadFile(hOutR, buf.get(), 256U, &br, NULL)) && (br > 0U)
              )
        {
            std::string _data(buf.get(), br);
            data.append(_data.begin(), _data.end());
        }
    }
    while (0);

    CloseHandle(hOutR);

    if (!ret)
        return std::wstring();

    return data;
}

}
