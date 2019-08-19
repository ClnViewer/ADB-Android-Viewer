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

DriverAsync::DriverAsync()
        : m_iswatch(true)
    {
        std::thread thw
        {
            [=]()
            {
                while (m_iswatch.load() || !m_queue.empty())
                {
                    if (!m_queue.empty())
                    {
                        std::future<bool> & f = m_queue.front();
                        auto a = f.get(); (void)a;
                        m_queue.pop();
                    }
                    else
                    {
                        std::this_thread::yield();
                        if (!m_iswatch.load())
                            break;
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }
                }
            }
        };
        m_thwatch = std::move(thw);
    }

DriverAsync::~DriverAsync()
    {
        m_iswatch = false;

        if (m_thwatch.joinable())
            m_thwatch.join();
    }

template <typename T>
void DriverAsync::add(T cmd, T const & addtype, std::function<bool(T, T const&, T&, DriverConst::ClearType)> fun)
    {
        m_queue.push(
            std::async(
                std::launch::async,
                [=]()
                {
                    T result{};
                    return fun(cmd, addtype, result, DriverConst::ClearType::CLEARTYPE_NONE);
                }
            )
        );
    }
    template void DriverAsync::add<std::wstring>(std::wstring, std::wstring const&, std::function<bool(std::wstring, std::wstring const&, std::wstring&, DriverConst::ClearType)>);
    template void DriverAsync::add<std::string> (std::string,  std::string  const&, std::function<bool(std::string,  std::string  const&, std::string&,  DriverConst::ClearType)>);

}
