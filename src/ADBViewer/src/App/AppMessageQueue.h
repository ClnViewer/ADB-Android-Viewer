#pragma once

class AppMessageQueue
{
public:
    //
    enum MessageQueueIdType
    {
        CNF_PLUGINS_ENABLE = 0,
        CNF_ADB_PATH,
        CNF_ADB_DEVICE,
        CNF_LANGUAGE
    };

    typedef struct
    {
        AppMessageQueue::MessageQueueIdType id;
        uint32_t                            timeot;
        std::string                         data;

    } DataMessageQueue;

private:
    //
    std::queue<AppMessageQueue::DataMessageQueue> m_queue;

    AppMessageQueue();
    AppMessageQueue(const AppMessageQueue&) = delete;
    AppMessageQueue& operator=(const AppMessageQueue&) = delete;

public:
    //
    static AppMessageQueue& instance();

    bool getData(AppMessageQueue::DataMessageQueue&);
    void setData(AppMessageQueue::DataMessageQueue&);
};
