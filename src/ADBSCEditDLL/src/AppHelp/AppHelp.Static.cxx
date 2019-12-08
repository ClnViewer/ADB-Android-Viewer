
#   define RTF_LINK(A,B) "{\\rtf1{\\field{\\*\\fldinst{ HYPERLINK \"" A "\"}}{\\fldrslt{" B "}\\par}}}."
#   define STR_SIZE(A) (__NELE(A) - 1)

    // CFM_ALL  - search by .key; print only titles, if titles possible
    // CFM_ALL2 - search by .title; print all fields
    //
    static AppHelp::HelpItems l_items_THELP_APP[] =
    {
#       define HLP_LABEL(A)    { CFM_ALL, A, nullptr, nullptr, nullptr },
#       define HLP_ITEM(A,B,C) { CFM_ALL, nullptr, A, B, C "\n" },
#       include "THELP_APP_Items.h"
    };
    //
    static AppHelp::HelpItems l_items_THELP_LUAOBJECT[] =
    {
        { CFM_ALL, "Plugin API - LuaObject:", nullptr, nullptr, nullptr },
#       define LFUN__(S,A,H1,H2,...) { CFM_ALL2, "LuaObject." #A, H1, H2, nullptr },
#       include <LuaPluginFunction.h>
    };
    //
    static AppHelp::HelpItems l_items_THELP_LUAFUNC[] =
    {
#       define HLP_LABEL(A)    { CFM_ALL,  A, nullptr, nullptr, nullptr },
#       define HLP_ITEM(A,B,C) { CFM_ALL2, A, B, C, nullptr },
#       include "THELP_LUAFUNC_Items.h"
    };
    //
    static AppHelp::HelpItems l_items_THELP_LUABYTECODE[] =
    {
#       define HLP_LABEL(A)    { CFM_ALL,  A, nullptr, nullptr, nullptr },
#       define HLP_ITEM(A,B,C) { CFM_ALL2, A, "* instruction: " B, C, nullptr },
#       include "THELP_LUABYTECODE_Items.h"
    };
    //
    static AppHelp::HelpItems l_items_THELP_LINK[] =
    {
#       define HLP_LABEL(A)
#       define HLP_ITEM(A,B) { CFM_LINK, RTF_LINK(A,B), A, nullptr, nullptr },
#       include "THELP_LINK_Items.h"
    };
    //
