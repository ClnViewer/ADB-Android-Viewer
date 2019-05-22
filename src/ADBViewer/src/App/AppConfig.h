#pragma once

class AppConfig
{
private:

    AppConfig()
        : cnf_isrun(false), cnf_isstop(true), cnf_ispos(false),
          cnf_isfullscreen(false), cnf_scale(2U), cnf_compress(9) {};

    AppConfig(const AppConfig& apc) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

public:
    //
    std::atomic<bool>     cnf_isrun;
    std::atomic<bool>     cnf_isstop;
    std::atomic<bool>     cnf_ispos;
    std::atomic<bool>     cnf_isfullscreen;
    std::atomic<uint32_t> cnf_scale;
    std::atomic<uint32_t> cnf_compress;

    ~AppConfig() {}

    static AppConfig& instance()
    {
        static AppConfig m_instance{};
        return m_instance;
    }
};
