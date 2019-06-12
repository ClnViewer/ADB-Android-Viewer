
#include "../../../ADBViewer/src/loader.h"
#include "../ResManager.h"

/// this build use key  -finput-charset=cp1251

namespace Resources
{

#if defined(OS_WIN)
#  include "../Resources/ResPopUp1251_ru.h"

DECL_STRINGLOAD_N(ru)
{
    if ((uint32_t)idx >= __NELE(popup_strings_ru))
        return nullptr;

    return popup_strings_ru[idx];
}
#endif

}
