
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"


namespace Resources
{

DECL_STRINGLOAD_M()
{
    switch(lang)
    {
        case ResManager::IndexLanguageResource::LANG_RU: return stringpopup_ru(idx);
        case ResManager::IndexLanguageResource::LANG_EN: return stringpopup_en(idx);
        case ResManager::IndexLanguageResource::LANG_DM: return stringpopup_dm(idx);
        case ResManager::IndexLanguageResource::LANG_CN: return stringpopup_cn(idx);
        default: return stringpopup_ru(idx);
    }
}

}
