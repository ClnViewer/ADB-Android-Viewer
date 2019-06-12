
#include "../../../ADBViewer/src/loader.h"
#include "../ResManager.h"

/// this build as utf8 source

namespace Resources
{

#if !defined(OS_WIN)
#  include "../Resources/ResPopUpUtf8_dm.h"

DECL_STRINGLOAD_N(dm)
{
    if ((uint32_t)idx >= __NELE(popup_strings_dm))
        return nullptr;

    return popup_strings_dm[idx];
}
#endif

}
