
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"


namespace Resources
{

#if defined(OS_WIN)
#  include "Resources/ResPopUp1251.h"
   const wchar_t * ResManager::stringpopup(ResManager::IndexStringPopUpMenu idx)
#else
#  include "Resources/ResPopUpUtf8.h"
   const char * ResManager::stringpopup(ResManager::IndexStringPopUpMenu idx)
#endif
{
    if ((uint32_t)idx < __NELE(popup_strings))
        return popup_strings[idx];
    return nullptr;
}

}
