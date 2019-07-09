#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#if defined(OS_WIN)
# define DECL_STRINGLOAD_M() \
   const wchar_t * ResManager::stringpopup ( \
                    ResManager::IndexStringPopUpMenu idx, ResManager::IndexLanguageResource lang \
            )
# define DECL_STRINGLOAD_N(A) \
   const wchar_t * ResManager::stringpopup_ ## A ( \
                    ResManager::IndexStringPopUpMenu idx \
            )
#else
# define DECL_STRINGLOAD_M() \
   const char * ResManager::stringpopup ( \
                    ResManager::IndexStringPopUpMenu idx, ResManager::IndexLanguageResource lang \
            )
# define DECL_STRINGLOAD_N(A) \
   const char * ResManager::stringpopup_ ## A ( \
                    ResManager::IndexStringPopUpMenu idx \
            )
#endif

namespace Resources
{
    class DLL_EXPORT ResManager
    {
        public:

            typedef struct {
                uint32_t w;
                uint32_t h;
                uint32_t bpp;
                uint8_t *data;
            } ImageResource_s;

            enum IndexLanguageResource
            {
                LANG_RU = 0,
                LANG_EN,
                LANG_DM,
                LANG_CN,
                LANG_DEFAULT = LANG_RU
            };
            enum IndexColorResource
            {
                RES_COLOR_GREEN_BLACK,
                RES_COLOR_BLACK_WHITE,
                RES_COLOR_GREEN_BG,
                RES_COLOR_GREEN_CURSOR
            };
            enum IndexFontResource
            {
                RES_FONT_16704,
                RES_FONT_FREESANS,
                RES_FONT_CONSOLAS,
                RES_FONT_DEFAULT = RES_FONT_FREESANS
            };
            enum IndexImageResource
            {
                RES_IMG_LOGO,
                RES_IMG_MENU_ACTIVE,
                RES_IMG_MENU_DISABLED,
                RES_IMG_SCREEN,
                RES_IMG_RCORD,
                RES_IMG_TERMCLOSE,
                RES_IMG_EDITMENU
            };
            enum IndexSpriteResource
            {
                RES_SPRITE_BENDER,
                RES_SPRITE_DEFAULT = RES_SPRITE_BENDER
            };
            enum IndexStringResource
            {
                RES_STR_QUIT,
                RES_STR_START,
                RES_STR_STOP,
                RES_STR_ADBSET,
                RES_STR_SCALE,
                RES_STR_POSINFO,
                RES_STR_CAPTURE_C,
                RES_STR_CAPTURE_D,
                RES_STR_CAPTURE_F,
                RES_STR_FULLSCREEN,
                RES_STR_APK,
                RES_STR_UNKNOWN,
                RES_STR_FILESAVE,
                RES_STR_CLIPBOARD,
                RES_STR_ADBCONNECT,
                RES_STR_ADBDISCONNECT,
                RES_STR_ADBCONNECTED,
                RES_STR_ADBDISCONNECTED,
                RES_STR_ADBINSTALLBIN,
                RES_STR_APPFULLNAME,
                RES_STR_APPTITLENAME,
                RES_STR_CAPFILENAME,
                RES_STR_LUAFILENAME,
                RES_STR_ERR_APP,
                RES_STR_ERR_ADRV,
                RES_STR_ERR_UNKNOWN,
                RES_STR_ENTER_TEXT,
                RES_STR_BENDER_ON,
                RES_STR_BENDER_OFF,
                RES_STR_ADB_LANG_NOT_SUPPORT,
                RES_STR_ADB_LANG_NOT_SUPPORT_ENG,
                RES_STR_SCRIPTEDIT_END,
                RES_STR_CLOSE_TERM
            };
            enum IndexStringPopUpMenu
            {
                RES_STR_POPUP_0,
                RES_STR_POPUP_1,
                RES_STR_POPUP_2,
                RES_STR_POPUP_3,
                RES_STR_POPUP_4,
                RES_STR_POPUP_5,
                RES_STR_POPUP_6,
                RES_STR_POPUP_7,
                RES_STR_POPUP_8,
                RES_STR_POPUP_9,
                RES_STR_POPUP_10,
                RES_STR_POPUP_11,
                RES_STR_POPUP_12,
                RES_STR_POPUP_13,
                RES_STR_POPUP_14,
                RES_STR_POPUP_15,
                RES_STR_POPUP_16,
                RES_STR_POPUP_17,
                RES_STR_POPUP_18,
                RES_STR_POPUP_19,
                RES_STR_POPUP_20,
                RES_STR_POPUP_21,
                RES_STR_POPUP_22,
                RES_STR_POPUP_23,
                RES_STR_POPUP_24,
                RES_STR_POPUP_25,
                RES_STR_POPUP_26,
                RES_STR_POPUP_27,
                RES_STR_POPUP_28,
                RES_STR_POPUP_29,
                RES_STR_POPUP_30,
                RES_STR_POPUP_31,
                RES_STR_POPUP_32,
                RES_STR_POPUP_33,
                RES_STR_POPUP_34,
                RES_STR_POPUP_35,
                RES_STR_POPUP_36,
                RES_STR_POPUP_37,
                RES_STR_POPUP_38,
                RES_STR_POPUP_39,
                RES_STR_POPUP_40,
                RES_STR_POPUP_41,
                RES_STR_POPUP_42,
                RES_STR_POPUP_43,
                RES_STR_POPUP_44,
                RES_STR_POPUP_45,
                RES_STR_POPUP_46,
                RES_STR_POPUP_47,
                RES_STR_POPUP_NONE
            };

            ResManager();
            ~ResManager();

            static TTF_Font    * fontload(ResManager::IndexFontResource);
            static void          fontcachefree();
            static void          fontcachefree(ResManager::IndexFontResource);
            static SDL_Color   * colorload(ResManager::IndexColorResource);
            static SDL_Surface * loadbmp(char const*);
            static SDL_Surface * imageload(ResManager::IndexImageResource);
            static SDL_Surface **spriteload(ResManager::IndexSpriteResource, SDL_Color*, uint32_t*);
            static const char  * speechrandom();
            static const char  * stringload(ResManager::IndexStringResource, ResManager::IndexLanguageResource);

#           if defined(OS_WIN)
            static const wchar_t * stringpopup(ResManager::IndexStringPopUpMenu, ResManager::IndexLanguageResource);
#           else
            static const char * stringpopup(ResManager::IndexStringPopUpMenu, ResManager::IndexLanguageResource);
#           endif

        private:
            static SDL_Surface * imagedata(const ImageResource_s*);
            static SDL_RWops   * fontloadraw(ResManager::IndexFontResource);

#           if defined(OS_WIN)
            static const wchar_t * stringpopup_ru(ResManager::IndexStringPopUpMenu);
            static const wchar_t * stringpopup_en(ResManager::IndexStringPopUpMenu);
            static const wchar_t * stringpopup_dm(ResManager::IndexStringPopUpMenu);
            static const wchar_t * stringpopup_cn(ResManager::IndexStringPopUpMenu);
#           else
            static const char * stringpopup_ru(ResManager::IndexStringPopUpMenu);
            static const char * stringpopup_en(ResManager::IndexStringPopUpMenu);
            static const char * stringpopup_dm(ResManager::IndexStringPopUpMenu);
            static const char * stringpopup_cn(ResManager::IndexStringPopUpMenu);
#           endif
    };
}
