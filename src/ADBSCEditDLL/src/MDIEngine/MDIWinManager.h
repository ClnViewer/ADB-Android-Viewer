
#pragma once

namespace MDIWin
{

    class DLL_EXPORT Manager
    {
        private:
            //
            uint32_t    m_count = 0;
            std::string m_class_main;
            std::string m_class_child;
            std::string m_class_control;
            std::map<uint32_t, std::shared_ptr<Base>> m_winmap;
            //

        public:
            //
            Manager();
            ~Manager();
            //
            //
            uint32_t       iswinid(HWND, BaseData::MDIWinType);
            uint32_t       getwinid(HWND);
            bool           init(std::string const&, void* = nullptr);
            void           moved(HWND, int32_t, int32_t);  // event WM_MOVE    (move window)
            void           closed(HWND);                   // event WM_DESTROY (close window)
            void           setsize();
            bool           remove(HWND);
            bool           remove(uint32_t, bool = false);
            MDIWin::Base * find(HWND);
            MDIWin::Base * find(uint32_t);
            //

            template <class Tclass>
            Base * add(HWND hwnd = nullptr, void *v = nullptr)
            {
                try
                {
                    std::string s;
                    uint32_t    id   = 0;
                    bool        isok = true;
                    auto        obj  = std::make_shared<Tclass>();

                    do
                    {
                        BaseData::MDIWinType g = obj->getgroup();

                        switch (g)
                        {
                            case BaseData::MDIWinType::MWTYPE_MAIN:
                                {
                                    hwnd = nullptr;
                                    s    = m_class_main;
                                    id   = static_cast<uint32_t>(g);
                                    break;
                                }
                            case BaseData::MDIWinType::MWTYPE_CLIENT:
                                {
                                    if (!hwnd)
                                        { isok = false; break; }
                                    s    = m_class_child;
                                    id   = static_cast<uint32_t>(g);
                                    break;
                                }
                            case BaseData::MDIWinType::MWTYPE_TOOLBAR:
                                {
                                    if (!hwnd)
                                        { isok = false; break; }
                                    id   = static_cast<uint32_t>(g);
                                    break;
                                }
                            case BaseData::MDIWinType::MWTYPE_CHILD:
                                {
                                    HWND hwndc = Config::instance().gethandle<Config::HandleType::HWND_CLIENT>();

                                    if (!hwndc)
                                        { isok = false; break; }

                                    hwnd = hwndc;
                                    s    = m_class_child;
                                    BaseData::MDIWinType t = obj->gettype();

                                    switch (t)
                                    {
                                        case BaseData::MDIWinType::MWTYPE_CHILD_EDIT:
                                        case BaseData::MDIWinType::MWTYPE_CHILD_INFO:
                                        case BaseData::MDIWinType::MWTYPE_CHILD_IMAGE:
                                        case BaseData::MDIWinType::MWTYPE_CHILD_TOOLS:
                                            {
                                                id = static_cast<uint32_t>(t);
                                                break;
                                            }
                                        case BaseData::MDIWinType::MWTYPE_CHILD_OTHER:
                                            {
                                                id = m_count;
                                                m_count++;
                                                break;
                                            }
                                        default:
                                            { isok = false; break; }
                                    }
                                    break;
                                }
                            default:
                                { isok = false; break; }
                        }
                        //
                        if (!isok)
                            break;
                        //
                        {
                            auto [it, result] = m_winmap.try_emplace(id, nullptr);
                            if (!result)
                            {
                                obj.reset();
                                return find(id);
                            }
                        }
                        //
                        if (!obj->init(hwnd, id, s, m_class_control, v))
                            break;

                        auto a = obj.get();
                        m_winmap[id] = std::move(obj);

                        if (Config::instance().gethandle<Config::HandleType::HWND_MAIN>())
                            setsize();
                        return a;
                    }
                    while (0);

                    obj.reset();
                    return nullptr;
                }
                catch (...) { return nullptr; }
            }
    };
};


