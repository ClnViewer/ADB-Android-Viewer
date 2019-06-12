
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"


namespace Resources
{

#if defined(OS_WIN)
#  include "Resources/ResPopUpWchar_cn.h"
   const wchar_t * ResManager::stringpopup_cn(
                    ResManager::IndexStringPopUpMenu idx
            )
#else
#  include "Resources/ResPopUpUtf8_cn.h"
   const char * ResManager::stringpopup_cn(
                    ResManager::IndexStringPopUpMenu idx
            )
#endif
{
    if ((uint32_t)idx >= __NELE(popup_strings_cn))
        return nullptr;
    return popup_strings_cn[idx];
}

}
