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

#include "../../ADBViewer/src/loader.h"
#include "../src/ADBDriver.h"

using namespace std;

int main()
{
    GameDev::ADBDriver m_adb;

    /*
    std::wstring response;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_CPU_LIST);
    std::wcout << L"GetProperties: " << response << std::endl;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_VERSION_RELEASE);
    std::wcout << L"GetProperties: " << response << std::endl;

    response = m_adb.GetProperties(GameDev::ADBDriver::DeviceInfoType::DI_VERSION_SDK);
    std::wcout << L"GetProperties: " << response << std::endl;
    */

    //m_adb.InitRemote();
    bool ret;
    ret = m_adb.GetDeviceSetupUI();
    std::wcout << L"GetDeviceSetupUI: " << ret << std::endl;
    ret = m_adb.GetDeviceListUI();
    std::wcout << L"GetDeviceListUI: " << ret << L" - " << m_adb.GetDeviceID<std::wstring>() << L" - " << m_adb.IsDeviceID() << std::endl;

    return 0;
}

static uint32_t __width_D_ = 640;
static uint32_t __height_D_ = 400;

struct _RGB {
    uint8_t r_, g_, b_;
};

uint32_t getPad(uint32_t w)
    {
        uint32_t pad = 0U;
        while (((w + pad) % 4) != 0)
            pad++;
        return pad;
    }

uint32_t checkPos(uint32_t w, uint32_t h, uint32_t pad)
    {
        return ((((w * 3U) + pad) * h) - 3U);
    }

void getPixel(int x, int y, std::vector<uint8_t> const & buf)
{
    uint32_t pad = getPad(__width_D_);
    uint32_t pos = ((((__width_D_ * 3) + pad) * y) + (x * 3));
    if (pos > checkPos(__width_D_, __height_D_, pad))
    {
        printf("position overflow!!!\n");
        return;
    }
    _RGB *rgb = (_RGB*)&buf[pos];
    //std::cout << "position:" << pos << " r:"  << rgb->r_ << " g:" << rgb->g_ << " b:" << rgb->b_ << std::endl;
    printf("position (%d/%d):%u\t - r:%u  \tg:%u\t b:%u\n", x, y, pos, rgb->r_, rgb->g_, rgb->b_);
}

int y_main()
{
    FILE *fp = fopen("..\\..\\template.raw", "rb");
    if (!fp)
    {
        std::cout << "file not found" << std::endl;
        return 1;
    }

    std::vector<uint8_t> buf(768000);
    fread(&buf[0], 1, 768000, fp);
    fclose(fp);

    getPixel(0,0, buf);
    getPixel(0,1, buf);
    getPixel(0,10, buf);

    getPixel(0,0, buf);
    getPixel(22,0, buf);
    getPixel(23,0, buf);
    getPixel(24,0, buf);

    getPixel(3,218, buf);
    getPixel(8,218, buf);
    getPixel(8,216, buf);

    return 0;
}
