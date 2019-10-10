#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <atomic>
#include <functional>
#include <exception>
#include <stdexcept>
#include <thread>
#include <windows.h>
#include <commctrl.h>

#define _BUILD_IMAGELITE_DRAW 1
#include <ImageLite.h>

int main()
{
    try
    {
        /// test image not found
        //  ImageLite::ImageRGBbuffer img01("TestImage.raw", 1024, 600);

        ImageLite::ImageRGBbuffer img1("TestImageLite.raw", 1024, 600);
        if (img1.empty())
            throw std::length_error("main!1!empty");

        img1.save("__test1.png");
        img1.save("__test1.bmp", ImageLite::ImageType::IT_BMP);

        ///

        ImageLite::IRECT<int32_t> irect5 = { 17, 33, 139, 550 };
        ImageLite::ImageRGBbuffer img5(img1.getbuffer(), irect5);
        if (img5.empty())
            throw std::length_error("main!5!empty");

        img5.save("__test5.bmp", ImageLite::ImageType::IT_BMP);
        img5.save("__test5.png", ImageLite::ImageType::IT_PNG);

        ///

        double cmp15 = img1.icompare(img5.getbuffer(), irect5);
        std::cout << "compare region image1 -> image5: " << cmp15 << std::endl;

        ///

        ImageLite::ImageRGBbuffer img6("__test5.png");
        if (img1.empty())
            throw std::length_error("main!6!empty");

        img6.save("__test6.png");
        img6.save("__test6.bmp", ImageLite::ImageType::IT_BMP);

        ///

        ImageLite::ImageRGBbuffer img2("TestImageLite.png");
        if (img2.empty())
            throw std::length_error("main!2!empty");

        img2.save("__test2.png");
        img2.save("__test2.bmp", ImageLite::ImageType::IT_BMP);

        ImageLite::ImageRGBbuffer img3("__test2.bmp");
        if (img3.empty())
            throw std::length_error("main!3!empty");

        ImageLite::IRECT<int32_t> irect4  = { 24, 50, 1000, 550 };
        ImageLite::ImageRGBbuffer img4(img1.getbuffer(), irect4);
        if (img4.empty())
            throw std::length_error("main!4!empty");

        img4.save("__test4.bmp", ImageLite::ImageType::IT_BMP);

        ///

        double cmp12 = img1.icompare(img2.getbuffer());
        std::cout << "compare region image1 -> image2: " << cmp12 << std::endl;

        ///

        // 1:
        //ImageLite::Draw idraw(img1);

        // 2:
        ImageLite::Draw idraw;
        idraw.add(1,img1);

        // 3:
        idraw.show("my test windows this");

        std::cout << "i'm go close -> wait.. 90 sec." << std::endl;
        {
            auto s_start = std::chrono::high_resolution_clock::now();
            auto s_end = (s_start + std::chrono::seconds(90));

            while (std::chrono::high_resolution_clock::now() < s_end)
            {
                if (!idraw.isactive())
                    break;

                std::this_thread::yield();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        std::cout << "-> i'm close!" << std::endl;
        idraw.close();
        std::cout << "<- closed!" << std::endl;
    }
    catch (std::runtime_error const & ex_)
    {
        std::cout << " ! runtime_error ! - " << ex_.what() << std::endl;
        return -1;
    }
    catch (std::exception const & ex_)
    {
        std::cout << " ! exception ! - " << ex_.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        //auto eptr = std::current_exception();
        std::cout << " ! (UN) exception ! - " << std::endl;
        return -1;
    }
    return 0;
}

#       if 0
        {
            std::stringstream ss;
            ss << m_name.c_str() << "_" << m_test_count << "_image.raw";
            FILE *fp = fopen(ss.str().c_str(), "wb");
            if (fp)
            {
                fwrite(&v[0], 1U, v.size(), fp);
                fclose(fp);
            }
        }
#       endif
