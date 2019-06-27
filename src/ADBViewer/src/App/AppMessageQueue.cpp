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

#include "../ADBViewer.h"

AppMessageQueue::AppMessageQueue()
    {
    }

AppMessageQueue& AppMessageQueue::instance()
    {
        static AppMessageQueue m_instance{};
        return m_instance;
    }

void AppMessageQueue::init()
    {
    }

bool AppMessageQueue::checkData() const
    {
        return !(m_queue.empty());
    }

bool AppMessageQueue::getData(AppMessageQueue::DataMessageQueue & dmsg)
    {
        if (m_queue.empty())
            return false;

        std::lock_guard<std::mutex> l_lock(m_lock);
        dmsg = m_queue.front();
        m_queue.pop();

        return true;
    }

void AppMessageQueue::setData(AppMessageQueue::DataMessageQueue& dmsg)
    {
        std::lock_guard<std::mutex> l_lock(m_lock);
        m_queue.push(dmsg);
    }

void AppMessageQueue::setData(std::string const & s, SDL_Point *cord, uint32_t tmo, int32_t id)
    {
        AppMessageQueue::DataMessageQueue dmsg{};
        dmsg.s = s;
        dmsg.tm = tmo;
        dmsg.id = id;
        memcpy(&dmsg.cord, cord, sizeof(SDL_Point));
        m_queue.push(dmsg);
    }

void AppMessageQueue::setData(std::string const & s, SDL_Point *cord, uint32_t tmo)
    {
        setData(s, cord, tmo, -1);
    }

void AppMessageQueue::setData(std::string const & s, SDL_Point *cord)
    {
        setData(s, cord, 0U, -1);
    }

void AppMessageQueue::setData(std::string const & s, uint32_t tmo, int32_t id)
    {
        SDL_Point cord = { 33, 0 };
        setData(s, &cord, tmo, id);
    }

void AppMessageQueue::setData(std::string const & s, int32_t id)
    {
        SDL_Point cord = { 33, 0 };
        setData(s, &cord, 0U, id);
    }

void AppMessageQueue::setData(std::string const & s)
    {
        SDL_Point cord = { 33, 0 };
        setData(s, &cord, 0U, -1);
    }
