
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"


namespace Resources
{

#include "Resources/ResHelp.h"
#include "Resources/ResLogo.h"
#include "Resources/ResMenu.h"
#include "Resources/ResScreen.h"
#include "Resources/Res16704font.h"

ResManager::ResManager() {}
ResManager::~ResManager() {}

const char * ResManager::stringload(ResManager::IndexStringResource idx)
{
    return help_strings[idx];
}

SDL_Surface * ResManager::imageload(ResManager::IndexImageResource idx)
{
    switch(idx)
    {
        case ResManager::IndexImageResource::RES_IMG_LOGO:
        {
            return imagedata(&img_logo);
        }
        case ResManager::IndexImageResource::RES_IMG_MENU:
        {
            return imagedata(&img_menu0);
        }
        case ResManager::IndexImageResource::RES_IMG_SCREEN:
        {
            return imagedata(&img_screen);
        }
        default:
            return nullptr;
    }

}

SDL_Surface * ResManager::imagedata(const ResManager::ImageResource_s *res)
{
    if (!res)
        return nullptr;

    uint32_t rmask, gmask, bmask, amask;
#   if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (res->bpp == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#   else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (res->bpp == 3) ? 0 : 0xff000000;
#   endif

    return SDL_CreateRGBSurfaceFrom(
                static_cast<void*>(res->data),
                res->w,
                res->h,
                (res->bpp * 8),
                (res->bpp * res->w),
                rmask, gmask, bmask, amask
            );
}

SDL_Surface * ResManager::loadbmp(char const *path)
{
    SDL_Surface *image_surface = SDL_LoadBMP(path);

    if(!image_surface)
        return nullptr;

    return image_surface;
}

SDL_RWops   * ResManager::fontload()
{
    return SDL_RWFromMem(&data_font[0], __NELE(data_font));
}

}
