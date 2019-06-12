
#include "../ADBViewer.h"

AppMessageQueue::AppMessageQueue()
    {
    }

AppMessageQueue& AppMessageQueue::instance()
    {
        static AppMessageQueue m_instance{};
        return m_instance;
    }

bool AppMessageQueue::getData(AppMessageQueue::DataMessageQueue & dmsg)
    {
        if (!m_queue.empty())
            return false;

        dmsg = m_queue.front();
        m_queue.pop();

        return true;
    }

void AppMessageQueue::setData(AppMessageQueue::DataMessageQueue& dmsg)
    {
        m_queue.push(dmsg);
    }
