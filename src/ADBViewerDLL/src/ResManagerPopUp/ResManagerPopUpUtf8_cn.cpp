
#include "../../../ADBViewer/src/loader.h"
#include "../ResManager.h"


namespace Resources
{

#if defined(OS_WIN)
#  include "../Resources/ResPopUpWchar_cn.h"
#else
#  include "../Resources/ResPopUpUtf8_cn.h"
#endif

DECL_STRINGLOAD_N(cn)
{
    if ((uint32_t)idx >= __NELE(popup_strings_cn))
        return nullptr;
    return popup_strings_cn[idx];
}

}
