
#include "../ADBViewer.h"

AppVideoAnimation::AppVideoAnimation()
{
}

AppVideoAnimation::~AppVideoAnimation()
{
    clean();
}

void AppVideoAnimation::clean()
{
    gui.active = false;

    if (m_timer.isactive())
        m_timer.stop();

    gui.texture = nullptr;

    if ((!m_textures) || (!m_textures_sz))
        return;

    for (uint32_t i = 0U; i < m_textures_sz; i++)
        if (m_textures[i])
            SDL_DestroyTexture(m_textures[i]);

    delete [] m_textures;
    m_textures = nullptr;
}

bool AppVideoAnimation::init(App *app)
{
    if (!app)
        return false;

    m_app = app;
    gui.texture = nullptr;

    do
    {
        bool isvalid = true;
        SDL_Surface **l_sprites  = nullptr;

        if (
            (!(l_sprites = ResManager::spriteload(&m_color, &m_textures_sz))) ||
            (!m_textures_sz)
            )
            break;

        gui.rect.w = l_sprites[0]->w;
        gui.rect.h = l_sprites[0]->h;

        m_textures = new SDL_Texture*[m_textures_sz]{};

        for (uint32_t i = 0U; i < m_textures_sz; i++)
        {
            if ((isvalid) && (l_sprites[i]))
                if (!(m_textures[i] = SDL_CreateTextureFromSurface(
                                        m_app->m_renderer,
                                        l_sprites[i]
                    )))
                    isvalid = false;

            SDL_FreeSurface(l_sprites[i]);
            l_sprites[i] = nullptr;
        }
        delete [] l_sprites;

        if (!isvalid)
        {
            clean();
            return false;
        }
    }
    while (0);

    bool ret = initgui(app);
    gui.active = false;

    if (ret)
        run();

    return ret;
}

void AppVideoAnimation::stop()
{
    if (m_timer.isactive())
        m_timer.stop();

    gui.active = false;
}

void AppVideoAnimation::run()
{
    if ((!m_textures) || (!m_textures_sz) || (m_timer.isactive()))
        return;

    m_key_y = 0U;

    m_timer.loop(
        0U,
        30U,
        [=]()
        {
            do
            {
                if (!m_app)
                    break;

                int32_t pad = m_app->m_appmenubar.gui.rect.w,
                        l_key_y = (m_app->m_appvideo.gui.rect.h - gui.rect.h);
                gui.rect.x = pad;
                gui.rect.y = l_key_y;
                pad += m_app->m_appvideo.gui.rect.w;

                for (uint32_t i = 0U; i < m_textures_sz; i++)
                {
                    if (i == (m_textures_sz - 1U))
                        i = 0U;

                    {
                        gui.active  = false;
                        gui.texture = m_textures[i];
                        gui.rect.x += 5;
                        gui.rect.y = (m_key_y + l_key_y);
                    }

                    if (gui.rect.x >= pad)
                        break;

                    if (!AppConfig::instance().cnf_isrun)
                        break;

                    gui.active = true;
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(80));

                    if (!AppConfig::instance().cnf_isrun)
                        break;
                }
            }
            while (0);

            gui.active = false;
            gui.texture = nullptr;
        }
    );
}

bool AppVideoAnimation::event(SDL_Event *ev, const void *instance)
{
    AppVideoAnimation *apva = static_cast<AppVideoAnimation*>(
                const_cast<void*>(instance)
            );

    if ((!apva) || (!AppConfig::instance().cnf_isstop) || (!apva->gui.active))
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            if (ev->button.button == SDL_BUTTON_LEFT)
            {
                if (
                    (ev->motion.x >= gui.rect.x) &&
                    (ev->motion.x <= (gui.rect.x + gui.rect.w)) &&
                    (ev->motion.y >= gui.rect.y) &&
                    (ev->motion.y <= (gui.rect.y + gui.rect.h))
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
                    apva->m_key_y -= 5;
                    return true;
                }
                case SDLK_DOWN:
                case SDLK_LEFT:
                {
                    apva->m_key_y += 5;
                    return true;
                }
            }
        }
    }
    return false;
}
