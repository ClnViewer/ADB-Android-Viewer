
#include "DriverInternal.h"
#include "DriverConst.h"

using namespace std::placeholders;

namespace GameDev
{

enum l_enumScanCodeAndroid
{
#   define KEYDEF(A,B) A = B,
#   include "Keys/ScanCodeAndroid.h"
};
enum l_enumScanCodeSDL2
{
#   define KEYDEF(A,B) A = B,
#   include "Keys/ScanCodeSDL2.h"
};
enum l_enumScanCodeWINAPI
{
#   define KEYDEF(A,B) A = B,
#   include "Keys/ScanCodeWIAPI.h"
};

static inline const int32_t l_arrayScanCodeTranslate[][3] =
{
#   define KEYDEF(A,B,C) {A, B, C},
#   include "Keys/ScanCodeTranslate.h"
};

static int32_t f_arrayScanCodeSearch(int32_t idx, int32_t key)
{
    int32_t k = 0;
    for (uint32_t i = 0; i < __NELE(l_arrayScanCodeTranslate); i++)
    {
        if (key == l_arrayScanCodeTranslate[i][idx])
        {
            k = l_arrayScanCodeTranslate[i][0];
            break;
        }
    }
    return k;
}

void ADBDriver::Click(ADBDriver::Tap_s *p)
{
    std::stringstream ss;
    ss << "input tap " << p->x << " " << p->y;

    m_cmdasync.add<std::string>(
        ss.str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
}

void ADBDriver::Click(ADBDriver::Swipe_s *p)
{
    std::stringstream ss;
    ss << "input swipe ";
    ss << p->x0 << " " << p->y0 << " ";
    ss << p->x1 << " " << p->y1 << " " << p->t;

    m_cmdasync.add<std::string>(
        ss.str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
}

void ADBDriver::ClickDbl(ADBDriver::Tap_s *p)
{
    std::stringstream ss;
    ss << "input swipe ";
    ss << p->x << " " << p->y << " ";
    ss << p->x << " " << p->y << " " << 150U;

    m_cmdasync.add<std::string>(
        ss.str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
}

void ADBDriver::SendTextASCII(std::wstring const & s)
{
    std::wstring ws(s);
    std::wstringstream wss;

    string_replace<std::wstring>(ws, L" ", L"%s");
    string_replace<std::wstring>(ws, L"\"", L"'");
    wss << L"input text \"" << ws.c_str() << L"\"";

    ws.assign(wss.str());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cnv;

    m_cmdasync.add<std::string>(
        cnv.to_bytes(ws.c_str()),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
    SendSpecialKey(
        ADBDriver::KeysType::KEYS_ANDROID,
        GameDev::l_enumScanCodeAndroid::AKEYCODE_ENTER
        );
}

void ADBDriver::SendTextASCII(std::string const & s)
{
    std::string str(s);
    std::stringstream ss;

    string_replace<std::string>(str, " ", "%s");
    string_replace<std::string>(str, "\"", "'");
    ss << "input text \"" << str.c_str() << "\"";

    m_cmdasync.add<std::string>(
        ss.str().c_str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
    SendSpecialKey(
        ADBDriver::KeysType::KEYS_ANDROID,
        GameDev::l_enumScanCodeAndroid::AKEYCODE_ENTER
        );
}

void ADBDriver::SendSpecialKey(ADBDriver::KeysType t, int32_t key)
{
    switch (t)
    {
        case ADBDriver::KeysType::KEYS_ANDROID:
            {
                if (
                    (key < GameDev::l_enumScanCodeAndroid::AKEYCODE_UNKNOWN) ||
                    (key >= GameDev::l_enumScanCodeAndroid::AKEYCODE_END)
                   )
                    return;
                break;
            }
        case ADBDriver::KeysType::KEYS_SDL:
            {
                key = f_arrayScanCodeSearch(1, key);
                break;
            }
        case ADBDriver::KeysType::KEYS_WINAPI:
            {
                key = f_arrayScanCodeSearch(2, key);
                break;
            }
        default:
            return;
    }
    if (!key)
        return;

    std::stringstream ss;
    ss << "input keyevent " << key;

    m_cmdasync.add<std::string>(
        ss.str(),
        DriverConst::ls_cmd_shell,
        BIND_ASYNC(std::string)
        );
}

}
