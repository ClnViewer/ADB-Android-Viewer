
#include "../ADBViewer.h"

bool AppAnimationBender::init(App *app)
{
    return guiAnimation::init(app, &m_color, ResManager::IndexSpriteResource::RES_SPRITE_BENDER);
}

void AppAnimationBender::stop()
{
    guiAnimation::stop();
}

void AppAnimationBender::run()
{
    guiAnimation::run();
}

bool AppAnimationBender::event(SDL_Event *ev, const void *instance)
{
    AppAnimationBender *aab = static_cast<AppAnimationBender*>(
                const_cast<void*>(instance)
            );

    if ((!aab) || (!AppConfig::instance().cnf_isstop) || (!aab->guiBase::gui.active))
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button == SDL_BUTTON_LEFT)
            {
                if (
                    (ev->motion.x >=  guiBase::gui.rect.x) &&
                    (ev->motion.x <= (guiBase::gui.rect.x + guiBase::gui.rect.w)) &&
                    (ev->motion.y >=  guiBase::gui.rect.y) &&
                    (ev->motion.y <= (guiBase::gui.rect.y + guiBase::gui.rect.h))
                    )
                {
                    AddMessageQueue(
                        ResManager::speechrandom(),
                        10U,
                        (__LINE__ + ev->motion.x)
                    );
                    return true;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            switch (ev->key.keysym.sym)
            {
                case SDLK_UP:
                case SDLK_RIGHT:
                {
                    aab->guiAnimation::m_key_y -= 5;
                    return true;
                }
                case SDLK_DOWN:
                case SDLK_LEFT:
                {
                    aab->guiAnimation::m_key_y += 5;
                    return true;
                }
            }
        }
    }
    return false;
}
