
#include "../../../ADBViewer/src/loader.h"
#include "../ResManager.h"

/// this build use key  -finput-charset=cp850

namespace Resources
{

#if defined(OS_WIN)
#  include "../Resources/ResPopUp850_dm.h"

DECL_STRINGLOAD_N(dm)
{
    if ((uint32_t)idx >= __NELE(popup_strings_dm))
        return nullptr;

    return popup_strings_dm[idx];
}
#endif

}
