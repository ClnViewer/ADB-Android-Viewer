
#pragma once

namespace Editor
{
    class MainWin : public MDIWin::Base
    {
        public:
            //
            MainWin();
            MDIWin::WinData        data() override;
            std::tuple<bool, bool> event_initBegin() override;
    };
};
