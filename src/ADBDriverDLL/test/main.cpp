
#include "../../ADBViewer/src/loader.h"
#include "../src/ADBDriver.h"

using namespace std;

int _x_main()
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
    std::wcout << L"GetDeviceListUI: " << ret << L" - " << m_adb.GetDeviceID() << L" - " << m_adb.IsDeviceID() << std::endl;

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

int main()
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
}
