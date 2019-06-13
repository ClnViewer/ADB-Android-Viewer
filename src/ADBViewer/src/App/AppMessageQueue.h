
#pragma once

#define AddMessageQueue(A,B,C)                      \
    {                                               \
        AppMessageQueue::instance().setData(        \
            A,                                      \
            static_cast<uint32_t>(B),               \
            static_cast<int32_t>(C)                 \
        );                                          \
        SDL_Event ev{};                             \
        ev.type = AppConfig::instance().cnf_uevent; \
        ev.user.code = ID_CMD_MSGBAR;               \
        SDL_PushEvent(&ev);                         \
    }

class AppMessageQueue
{
public:
    //
    typedef struct
    {
        int32_t                             id;
        uint32_t                            tm;
        std::string                         s;
        SDL_Point                           cord;

    } DataMessageQueue;

private:
    //
    std::queue<AppMessageQueue::DataMessageQueue> m_queue;
    std::mutex                                    m_lock;

    AppMessageQueue();
    AppMessageQueue(const AppMessageQueue&) = delete;
    AppMessageQueue& operator=(const AppMessageQueue&) = delete;

public:
    //
    static AppMessageQueue& instance();

    void init();
    bool checkData() const;
    bool getData(AppMessageQueue::DataMessageQueue&);
    void setData(AppMessageQueue::DataMessageQueue&);
    void setData(std::string const&, SDL_Point*, uint32_t, int32_t);
    void setData(std::string const&, SDL_Point*, uint32_t);
    void setData(std::string const&, SDL_Point*);
    void setData(std::string const&, uint32_t, int32_t);
    void setData(std::string const&, int32_t);
    void setData(std::string const&);

};
