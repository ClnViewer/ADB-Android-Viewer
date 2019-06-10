#pragma once

#include <SDL2/SDL.h>

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

            enum IndexColorResource
            {
                RES_COLOR_GREEN_BLACK,
                RES_COLOR_BLACK_WHITE
            };
            enum IndexFontResource
            {
                RES_FONT_16704,
                RES_FONT_FREESANS
            };
            enum IndexImageResource
            {
                RES_IMG_LOGO,
                RES_IMG_MENU,
                RES_IMG_SCREEN,
                RES_IMG_RCORD
            };
            enum IndexStringResource
            {
                RES_STR_QUIT,
                RES_STR_START,
                RES_STR_STOP,
                RES_STR_ADBSET,
                RES_STR_SCALE,
                RES_STR_POSINFO,
                RES_STR_CAPTURE_D,
                RES_STR_CAPTURE_F,
                RES_STR_FULLSCREEN,
                RES_STR_APK,
                RES_STR_UNKNOWN,
                RES_STR_FILESAVE,
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
                RES_STR_ENTER_TEXT
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
                RES_STR_POPUP_NONE
            };

            ResManager();
            ~ResManager();

            static SDL_RWops   * fontload(ResManager::IndexFontResource);
            static SDL_Color   * colorload(ResManager::IndexColorResource);
            static SDL_Surface * loadbmp(char const*);
            static SDL_Surface * imageload(ResManager::IndexImageResource);
            static const char  * stringload(ResManager::IndexStringResource);

#           if defined(OS_WIN)
            static const wchar_t * stringpopup(ResManager::IndexStringPopUpMenu);
#           else
            static const char * stringpopup(ResManager::IndexStringPopUpMenu);
#           endif

        private:
            static SDL_Surface * imagedata(const ImageResource_s *res);

    };
}
