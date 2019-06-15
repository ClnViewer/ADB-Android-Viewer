
#include "../../ADBViewer/src/loader.h"
#include "ResManager.h"
#include <cstdlib>
#include <ctime>

namespace Resources
{
#include "Resources/ResHelp_ru.h"
#include "Resources/ResHelp_en.h"
#include "Resources/ResHelp_dm.h"
#include "Resources/ResHelp_cn.h"
#include "Resources/ResLogo.h"
#include "Resources/ResMenu.h"
#include "Resources/ResScreen.h"
#include "Resources/ResRecord.h"
#include "Resources/Res16704font.h"
#include "Resources/ResFreeSansfont.h"
#include "Resources/clip/bender-anime-8/bender8sprite.h"
#include "Resources/clip/bender-anime-8/bender8speech.h"

static inline SDL_Color box_color[][2] =
{
    {
        { 191, 227, 103, 0 },
        { 2, 2, 2, 150 }
    },
    {
        { 5, 5, 5, 0 },
        { 250, 250, 250, 150 }
    }
};

__attribute__((constructor)) static void initialize_ResourcesRand()
{
    std::srand(unsigned(std::time(0)));
}

ResManager::ResManager() {}
ResManager::~ResManager() {}

const char * ResManager::stringload(ResManager::IndexStringResource idx, ResManager::IndexLanguageResource lang)
{
    if ((uint32_t)idx >= __NELE(help_strings_ru))
        return nullptr;

    switch(lang)
    {
        case ResManager::IndexLanguageResource::LANG_RU: return help_strings_ru[idx];
        case ResManager::IndexLanguageResource::LANG_EN: return help_strings_en[idx];
        case ResManager::IndexLanguageResource::LANG_DM: return help_strings_dm[idx];
        case ResManager::IndexLanguageResource::LANG_CN: return help_strings_cn[idx];
        default: return help_strings_ru[idx];
    }
}

const char * ResManager::speechrandom()
    {
        return txt_bender8speech[(std::rand() % __NELE(txt_bender8speech))];
    }

SDL_Surface ** ResManager::spriteload(ResManager::IndexSpriteResource idx, SDL_Color *transparent, uint32_t *psz)
    {
        do
        {
            if (psz)
                *psz = 0U;

            uint32_t i = 0U,
                     l_ssz = 0U;
            const ResManager::ImageResource_s **l_res = nullptr;

            switch (idx)
            {
                default:
                //case ResManager::IndexSpriteResource::RES_SPRITE_BENDER
                {
                    l_ssz = __NELE(img_bender8sprites);
                    l_res = img_bender8sprites;
                }
            }

            SDL_Surface **surf = new SDL_Surface*[l_ssz]{};
            if (!surf)
                break;

            for (; i < l_ssz; i++)
            {
                if (!(surf[i] = ResManager::imagedata(l_res[i])))
                    break;

                if (transparent)
                {
                    surf[i]->format->Amask = 0xFF000000;
                    surf[i]->format->Ashift = 24;

                    SDL_SetColorKey(
                        surf[i],
                        SDL_TRUE,
                        SDL_MapRGB(
                            surf[i]->format,
                            transparent->r,
                            transparent->g,
                            transparent->b
                        )
                    );
                }
            }

            if (i != l_ssz)
                break;

            if (psz)
                *psz = l_ssz;

            return surf;
        }
        while (0);

        return nullptr;
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
        case ResManager::IndexImageResource::RES_IMG_RCORD:
        {
            return imagedata(&img_record);
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

SDL_RWops   * ResManager::fontload(ResManager::IndexFontResource idx)
{
    switch (idx)
    {
        case ResManager::IndexFontResource::RES_FONT_16704:
            {
                return SDL_RWFromMem(&data_font16704[0], __NELE(data_font16704));
            }
        case ResManager::IndexFontResource::RES_FONT_FREESANS:
            {
                return SDL_RWFromMem(&data_fontFreeSans[0], __NELE(data_fontFreeSans));
            }
        default:
            {
                return nullptr;
            }
    }
}

SDL_Color * ResManager::colorload(ResManager::IndexColorResource idx)
{
    switch (idx)
    {
        case ResManager::IndexColorResource::RES_COLOR_GREEN_BLACK:
            {
                return box_color[0];
            }
        case ResManager::IndexColorResource::RES_COLOR_BLACK_WHITE:
            {
                return box_color[1];
            }
        default:
            {
                return nullptr;
            }
    }
}

}
