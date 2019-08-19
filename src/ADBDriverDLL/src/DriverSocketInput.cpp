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

static inline int32_t f_arrayScanCodeSearch(int32_t idx, int32_t key)
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

#if defined(_BUILD_NOTASCII_STRING)
template<typename T>
static inline bool f_IsAsciiString(T & s)
    {
        return !(std::any_of(
                    s.begin(),
                    s.end(),
                    [](auto c)
                    {
                        if constexpr (std::is_same<T, std::wstring>::value)
                            return !(iswascii(static_cast<uint8_t>(c)));
                        else if constexpr (std::is_same<T, std::string>::value)
                            return !(isascii(static_cast<uint8_t>(c)));
                    }
                ));
    }
#else
template<typename T>
static inline bool f_ClearAsciiString(T & s)
    {
        s.erase(
            remove_if(
                s.begin(),
                s.end(),
                [](auto c)
                {
                    if constexpr (std::is_same<T, std::wstring>::value)
                        return !(iswascii(static_cast<uint8_t>(c)));
                    else if constexpr (std::is_same<T, std::string>::value)
                        return !(isascii(static_cast<uint8_t>(c)));
                }
            ),
            s.end()
        );
        if (s.empty())
            return false;

        return std::any_of(
                    s.begin(),
                    s.end(),
                    [](auto c)
                    {
                        if constexpr (std::is_same<T, std::wstring>::value)
                        {
                            return ((!iswpunct(static_cast<uint8_t>(c))) &&
                                    (!isblank(static_cast<int32_t>(c))));
                        }
                        else if constexpr (std::is_same<T, std::string>::value)
                        {
                            int32_t n = static_cast<int32_t>(c);
                            return ((!ispunct(n)) &&
                                    (!isblank(n)));
                        }
                    }
                );
    }
#endif

void ADBDriver::Click(ADBDriver::Tap_s *p)
    {
        std::stringstream ss;
        ss << DriverConst::ls_cmd_input << DriverConst::ls_arrayInput[0];
        ss << p->x << DriverConst::ls_punct_space << p->y;

        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
    }

void ADBDriver::Click(ADBDriver::Swipe_s *p)
    {
        std::stringstream ss;
        ss << DriverConst::ls_cmd_input << DriverConst::ls_arrayInput[1];
        ss << p->x0 << DriverConst::ls_punct_space << p->y0 << DriverConst::ls_punct_space;
        ss << p->x1 << DriverConst::ls_punct_space << p->y1 << DriverConst::ls_punct_space << p->t;

        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
    }

void ADBDriver::ClickDbl(ADBDriver::Tap_s *p)
    {
        std::stringstream ss;
        ss << DriverConst::ls_cmd_input << DriverConst::ls_arrayInput[1];
        ss << p->x << DriverConst::ls_punct_space << p->y << DriverConst::ls_punct_space;
        ss << p->x << DriverConst::ls_punct_space << p->y << DriverConst::ls_punct_space << 150U;

        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
    }

void ADBDriver::SendSpecialKey(DriverConst::KeysType t, int32_t key)
    {
        switch (t)
        {
            case DriverConst::KeysType::KEYS_ANDROID:
                {
                    if ((key < GameDev::l_enumScanCodeAndroid::AKEYCODE_UNKNOWN) ||
                        (key >= GameDev::l_enumScanCodeAndroid::AKEYCODE_END))
                        return;
                    break;
                }
            case DriverConst::KeysType::KEYS_SDL:
                {
                    key = f_arrayScanCodeSearch(1, key);
                    break;
                }
            case DriverConst::KeysType::KEYS_WINAPI:
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
        ss << DriverConst::ls_cmd_input << DriverConst::ls_arrayInput[2];
        ss << key;

        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
    }

bool ADBDriver::SendToShell(std::string const & s, std::string & sr, bool isclean, DriverConst::ClearType typec)
    {
        do
        {
            if (s.empty())
                break;
            if (!AdbRawT<std::string>(s, DriverConst::ls_cmd_shell, sr, typec))
                break;
            if ((isclean) && (!sr.empty()))
                string_clear_end<std::string>(sr);
            return true;
        }
        while (0);
        return false;
    }

template <typename T>
bool DLL_EXPORT ADBDriver::SendTextT(T const & s, bool isasciitext)
    {
        if (s.empty())
            return false;

        T ts(s);
        if (isasciitext)
#           if defined(_BUILD_NOTASCII_STRING)
            if (!f_IsAsciiString<T>(ts))
#           else
            if (!f_ClearAsciiString<T>(ts))
#           endif
                return false;

        std::stringstream ss;
        ss << DriverConst::ls_cmd_input << DriverConst::ls_arrayInput[3];

        if constexpr (std::is_same<T, std::wstring>::value)
        {
            string_replace<T>(ts, DriverConst::lw_punct_space, L"%s");
            string_replace<T>(ts, DriverConst::lw_punct_quote, L"\\\"");
            ss << DriverConst::ls_punct_quote << string_to_utf8(ts).c_str() << DriverConst::ls_punct_quote;
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            string_replace<T>(ts, DriverConst::ls_punct_space, "%s");
            string_replace<T>(ts, DriverConst::ls_punct_quote, "\\\"");
            ss << DriverConst::ls_punct_quote << ts.c_str() << DriverConst::ls_punct_quote;
        }
        m_cmdasync.add<std::string>(
            ss.str(),
            DriverConst::ls_cmd_shell,
            BIND_ASYNC(std::string)
        );
        /*
        SendSpecialKey(
            DriverConst::KeysType::KEYS_ANDROID,
            GameDev::l_enumScanCodeAndroid::AKEYCODE_ENTER
        );
        */
        return true;
    }
    template DLL_EXPORT bool ADBDriver::SendTextT<std::wstring>(std::wstring const&, bool);
    template DLL_EXPORT bool ADBDriver::SendTextT<std::string> (std::string const&,  bool);

}
