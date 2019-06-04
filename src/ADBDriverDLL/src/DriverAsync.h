
#pragma once

namespace GameDev
{

class DriverAsync
{
private:
    std::thread                   m_thwatch;
    std::atomic<bool>             m_iswatch;
    std::queue<std::future<bool>> m_queue;

public:

    DriverAsync()
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

    ~DriverAsync()
    {
        m_iswatch = false;

        if (m_thwatch.joinable())
            m_thwatch.join();
    }

    template <typename T>
    void add(T cmd, T const & addtype, std::function<bool(T, T const&, T&, bool)> fun)
    {
        m_queue.push(
            std::async(
                std::launch::async,
                [=]()
                {
                    T result{};
                    return fun(cmd, addtype, result, true);
                }
            )
        );
    }
};

}
