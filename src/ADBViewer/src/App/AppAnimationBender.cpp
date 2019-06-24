
#include "../ADBViewer.h"

bool AppAnimationBender::init(App *app)
{
    if (!AppConfig::instance().cnf_disp_bender)
        return guiAnimation::init(app);
    return guiAnimation::init(app, &m_color, ResManager::IndexSpriteResource::RES_SPRITE_BENDER);
}

void AppAnimationBender::stop()
{
    if (!AppConfig::instance().cnf_disp_bender)
        return;
    guiAnimation::stop();
}

void AppAnimationBender::run()
{
    if (!AppConfig::instance().cnf_disp_bender)
    {
        if (guiAnimation::isinit())
            guiAnimation::stop();
        return;
    }
    guiAnimation::run();
}

bool AppAnimationBender::event(SDL_Event *ev, const void *instance)
{
    AppAnimationBender *aab = static_cast<AppAnimationBender*>(
                const_cast<void*>(instance)
            );

    if (!aab)
        return false;

    if (
        (ev->type == AppConfig::instance().cnf_uevent) &&
        (ev->user.code == ID_CMD_POP_MENU15)
       )
    {
        if (AppConfig::instance().cnf_disp_bender.load())
        {
            if (!aab->guiAnimation::isinit())
                if (!aab->guiAnimation::init(
                            aab->guiAnimation::m_app,
                            &aab->m_color,
                            ResManager::IndexSpriteResource::RES_SPRITE_BENDER
                        )
                    )
                    AppConfig::instance().cnf_disp_bender = false;

            if (AppConfig::instance().cnf_disp_bender.load())
                aab->stop();
        }
        else if (aab->isinit())
            aab->stop();

        AddMessageQueue(
            ResManager::stringload(
                ((AppConfig::instance().cnf_disp_bender.load()) ?
                    ResManager::IndexStringResource::RES_STR_BENDER_ON :
                    ResManager::IndexStringResource::RES_STR_BENDER_OFF
                ),
                AppConfig::instance().cnf_lang
            ),
            10U,
            -1
        );
        return true;
    }

    if ((!AppConfig::instance().cnf_isstop) || (!aab->guiBase::IsActive()))
        return false;

    switch(ev->type)
    {
        case SDL_RENDER_TARGETS_RESET:
        case SDL_RENDER_DEVICE_RESET:
            {
                aab->guiBase::ActiveOff();
                aab->guiBase::gui.texture = nullptr;
                return false;
            }
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button == SDL_BUTTON_LEFT)
            {
                if (
                    (ev->motion.x >=  aab->guiBase::gui.rect.x) &&
                    (ev->motion.x <= (aab->guiBase::gui.rect.x + aab->guiBase::gui.rect.w)) &&
                    (ev->motion.y >=  aab->guiBase::gui.rect.y) &&
                    (ev->motion.y <= (aab->guiBase::gui.rect.y + aab->guiBase::gui.rect.h))
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
