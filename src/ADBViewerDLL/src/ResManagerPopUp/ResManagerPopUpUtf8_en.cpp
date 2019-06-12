
#include "../../../ADBViewer/src/loader.h"
#include "../ResManager.h"

/// this build as utf8 source

namespace Resources
{

#if !defined(OS_WIN)
#  include "../Resources/ResPopUpUtf8_en.h"

DECL_STRINGLOAD_N(en)
{
    if ((uint32_t)idx >= __NELE(popup_strings_en))
        return nullptr;

    return popup_strings_en[idx];
}
#endif

}
