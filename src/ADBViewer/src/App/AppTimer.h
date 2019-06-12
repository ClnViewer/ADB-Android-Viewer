#pragma once

#include "../ADBViewer.h"

class AppTimer
{
public:
    //
    AppTimer() {}
    ~AppTimer()
    {
        if (m_task.valid())
            m_task.wait();
    }

    void start(uint32_t sec, auto f)
    {
        if (m_active.load())
            return;

        if (m_task.valid())
            m_task.wait();

        m_active = true;
        m_task = std::async(
                    std::launch::async,
                    [=](uint32_t s)
                    {
                        s = (s * 100);
                        do
                        {
                            if (!m_active.load()) return;
                            std::this_thread::yield();
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            if (!m_active.load()) return;
                        }
                        while (s--);

                        if (!m_active.load()) return;
                        f();
                        m_active = false;
                    },
                    sec
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

    bool isactive() const
    {
        return m_active;
    }

private:
    //
    std::atomic<bool>               m_active;
    std::future<void>               m_task;
};
