
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
