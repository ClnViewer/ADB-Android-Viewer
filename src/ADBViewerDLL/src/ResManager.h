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

            enum IndexImageResource
            {
                RES_IMG_LOGO,
                RES_IMG_MENU,
                RES_IMG_SCREEN
            };
            enum IndexStringResource
            {
                RES_STR_QUIT,
                RES_STR_START,
                RES_STR_STOP,
                RES_STR_ADBSET,
                RES_STR_SCALE,
                RES_STR_POSINFO,
                RES_STR_CAPTURE,
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
                RES_STR_ERR_APP,
                RES_STR_ERR_ADRV,
                RES_STR_ERR_UNKNOWN
            };

            ResManager();
            ~ResManager();

            static SDL_RWops   * fontload();
            static SDL_Surface * loadbmp(char const*);
            static SDL_Surface * imageload(ResManager::IndexImageResource);
            static const char  * stringload(ResManager::IndexStringResource);

        private:
            static SDL_Surface * imagedata(const ImageResource_s *res);

    };
}
