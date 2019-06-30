#pragma once

#include "../ADBViewer.h"

static inline uint32_t l_calctime(uint32_t sec, uint32_t msec)
{
    return ((msec) ? (msec * 10) : (sec * 100));
}

class AppTimer
{
private:
    //
    std::atomic<bool>               m_active = false;
    std::future<void>               m_task;
    //
    void sleeps(uint32_t sec)
    {
        do
        {
            if (!m_active.load()) return;
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        while (sec--);
    }

public:
    //
    AppTimer() {}
    ~AppTimer()
    {
        if (m_task.valid())
            m_task.wait();
    }

    void once(uint32_t sec, uint32_t msec, auto f)
    {
        if (m_active.load())
            return;

        if (m_task.valid())
            m_task.wait();

        uint32_t t = l_calctime(sec, msec);
        m_active = true;
        m_task = std::async(
                    std::launch::async,
                    [=](uint32_t s)
                    {
                        sleeps(s);

                        if (!m_active.load()) return;
                        f();
                        m_active = false;
                    },
                    t
            );
    }

    void loop(uint32_t cnt, uint32_t sec, uint32_t msec, auto f)
    {
        if (m_active.load())
            return;

        if (m_task.valid())
            m_task.wait();

        uint32_t t = l_calctime(sec, msec);
        m_active = true;
        m_task = std::async(
                    std::launch::async,
                    [=](uint32_t c, uint32_t s)
                    {
                        bool a = (c == 0U);
                        if (a)
                            c++;

                        do
                        {
                            sleeps(s);
                            if (!m_active.load()) return;
                            f();
                            if (!m_active.load()) return;
                            if (a)
                                c++;
                        }
                        while (c--);

                        m_active = false;
                    },
                    cnt,
                    t
            );
    }

    void stop()
    {
        if (!m_active.load())
            return;

        m_active = false;

        if (m_task.valid())
        {
            if (m_task.wait_for(std::chrono::milliseconds(0)) != std::future_status::deferred)
                while (m_task.wait_for(std::chrono::milliseconds(25)) != std::future_status::ready) {}

            m_task.wait();
        }
    }

    void wait()
    {
        if (m_task.valid())
            m_task.wait();
    }

    bool isactive() const
    {
        return m_active.load();
    }
};
