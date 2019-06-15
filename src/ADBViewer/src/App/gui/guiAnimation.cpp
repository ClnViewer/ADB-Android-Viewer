
#include "../../ADBViewer.h"

guiAnimation::guiAnimation()
{
}

guiAnimation::~guiAnimation()
{
    cleanup();
}

void guiAnimation::cleanup()
{
    guiBase::gui.active = false;

    if (m_timer.isactive())
        m_timer.stop();

    guiBase::gui.texture = nullptr;

    if ((!m_textures) || (!m_textures_sz))
        return;

    for (uint32_t i = 0U; i < m_textures_sz; i++)
        if (m_textures[i])
            SDL_DestroyTexture(m_textures[i]);

    delete [] m_textures;
    m_textures = nullptr;
}

bool guiAnimation::init(App *app, SDL_Color *bcolor, ResManager::IndexSpriteResource res)
{
    if (!app)
        return false;

    m_app = app;
    guiBase::gui.texture = nullptr;

    do
    {
        bool isvalid = true;
        SDL_Surface **l_sprites  = nullptr;

        if (
            (!(l_sprites = ResManager::spriteload(res, bcolor, &m_textures_sz))) ||
            (!m_textures_sz)
            )
            break;

        guiBase::gui.rect.w = l_sprites[0]->w;
        guiBase::gui.rect.h = l_sprites[0]->h;

        m_textures = new SDL_Texture*[m_textures_sz]{};

        for (uint32_t i = 0U; i < m_textures_sz; i++)
        {
            if ((isvalid) && (l_sprites[i]))
                if (!(m_textures[i] = SDL_CreateTextureFromSurface(
                                        m_app->m_renderer,
                                        l_sprites[i]
                    )))
                    isvalid = false;

            if (l_sprites[i])
                SDL_FreeSurface(l_sprites[i]);
            l_sprites[i] = nullptr;
        }
        delete [] l_sprites;

        if (!isvalid)
        {
            cleanup();
            return false;
        }
    }
    while (0);

    bool ret = guiBase::initgui(app);
    guiBase::gui.active = false;

    if (ret)
        run();

    return ret;
}

void guiAnimation::stop()
{
    if (m_timer.isactive())
        m_timer.stop();

    guiBase::gui.active = false;
}

void guiAnimation::run()
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
                        l_key_y = (m_app->m_appvideo.gui.rect.h - guiBase::gui.rect.h);
                guiBase::gui.rect.x = (pad + m_key_x);
                guiBase::gui.rect.y = l_key_y;
                pad += m_app->m_appvideo.gui.rect.w;

                for (uint32_t i = 0U; i < m_textures_sz; i++)
                {
                    if (i == (m_textures_sz - 1U))
                        i = 0U;

                    {
                        guiBase::gui.active  = false;
                        guiBase::gui.texture = m_textures[i];
                        guiBase::gui.rect.x += (5 + m_key_x);
                        guiBase::gui.rect.y = (m_key_y + l_key_y);
                    }

                    if (guiBase::gui.rect.x >= pad)
                        break;

                    if (!AppConfig::instance().cnf_isrun)
                        break;

                    guiBase::gui.active = true;
                    std::this_thread::yield();
                    std::this_thread::sleep_for(std::chrono::milliseconds(80));

                    if (!AppConfig::instance().cnf_isrun)
                        break;
                }
            }
            while (0);

            guiBase::gui.active = false;
            guiBase::gui.texture = nullptr;
        }
    );
}
