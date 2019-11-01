
#pragma once

namespace MDIWin
{

    class ClientWin : public Base
    {
        public:
            //
            ClientWin();
            MDIWin::WinData        data() override;
    };
};
