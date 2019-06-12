
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"


namespace Resources
{

#if defined(OS_WIN)
#  include "Resources/ResPopUp1251_ru.h"
#  include "Resources/ResPopUp437_en.h"
   const wchar_t * ResManager::stringpopup(
                    ResManager::IndexStringPopUpMenu idx, ResManager::IndexLanguageResource lang
            )
#else
#  include "Resources/ResPopUpUtf8_ru.h"
#  include "Resources/ResPopUpUtf8_en.h"
   const char * ResManager::stringpopup(
                    ResManager::IndexStringPopUpMenu idx, ResManager::IndexLanguageResource lang
            )
#endif
{
    if ((uint32_t)idx >= __NELE(popup_strings_ru))
        return nullptr;

    switch(lang)
    {
        case ResManager::IndexLanguageResource::LANG_RU: return popup_strings_ru[idx];
        case ResManager::IndexLanguageResource::LANG_EN: return popup_strings_en[idx];
        case ResManager::IndexLanguageResource::LANG_CN: return stringpopup_cn(idx);
        default: return popup_strings_ru[idx];
    }
}

}
