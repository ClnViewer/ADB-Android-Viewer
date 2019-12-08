
#pragma once

namespace Editor
{
    class AppHelp
    {
        public:
            //
            enum HelpType
            {
                THELP_APP,
                THELP_LUAFUNC,
                THELP_LUAOBJECT,
                THELP_LUABYTECODE,
                THELP_LINK,
                THELP_URL,
                THELP_NONE
            };
            //
            struct HelpItems
            {
                uint32_t    flag;
                char const *title;
                char const *key;
                char const *desc;
                char const *info;
            };
            //
            using helparray_t = std::tuple<HelpItems*, uint32_t>;
            //
            HelpItems m_finditems[11]{};
            //
            AppHelp();
            //
            helparray_t get(HelpType);
            helparray_t get(std::string const&);
            HelpType    getid(std::string const&);
            HelpType    isurl(std::string const&);

        private:
            //
            std::regex  m_re[4];
            //
            std::tuple<HelpType, std::string> find_(std::string const&);
    };
};
