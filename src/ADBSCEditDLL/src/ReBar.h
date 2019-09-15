
#pragma once

namespace Editor
{
    class ReBar : public WindowBase
    {
        public:
            //
            ~ReBar();
            //
            bool                    create(HWND, HINSTANCE);
            int32_t                 addband(HWND, int32_t);
            void                    showband(int32_t);

        private:
            //
            HIMAGELIST              m_himl = nullptr;
    };
};
#define ID_REBAR_BAND 3000

