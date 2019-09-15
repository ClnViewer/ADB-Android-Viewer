#pragma once

namespace Editor
{
    class Config
    {
        private:
            //
            HINSTANCE            m_hinst   = nullptr;
            HWND                 m_hwnd    = nullptr;
            HFONT                m_hfont   = nullptr;
            //
            Config();
            ~Config();
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            //
            void clear();
            void init(HWND);
            void init(HINSTANCE, HWND = nullptr);
            static Config& instance();
            //
            HWND              gethandle();
            Editor::EditBox * geteditbox();
            HFONT             getfont();
            std::string       getfontname();
            //
    };
};
