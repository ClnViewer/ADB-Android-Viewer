
#include "../ADBViewer.h"

AppMessageBar::AppMessageBar()
    {
    }

AppMessageBar::~AppMessageBar()
    {
    }

bool AppMessageBar::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;
        bool ret = guiStaticText::init(
                app,
                ResManager::IndexFontResource::RES_FONT_16704,
                ResManager::IndexColorResource::RES_COLOR_GREEN_BLACK
                );
        guiStaticText::ActiveOn();
        return ret;
}

void AppMessageBar::clear()
    {
        guiStaticText::clear();
        guiStaticText::ActiveOn();
    }

bool AppMessageBar::evresize(SDL_Texture**)
    {
        clear();
        return false;
    }

bool AppMessageBar::event(SDL_Event *ev, const void *instance)
{
    do
    {
        AppMessageBar *amb = static_cast<AppMessageBar*>(
                    const_cast<void*>(instance)
                );

        if (!amb)
            break;

        if (
            (ev->type == SDL_RENDER_TARGETS_RESET) ||
            (ev->type == SDL_RENDER_DEVICE_RESET)
            )
        {
            (void) amb->evresize(&amb->guiStaticText::gui.texture);
            break;
        }
        if (
            (ev->type != AppConfig::instance().cnf_uevent) ||
            (ev->user.code != ID_CMD_MSGBAR)
            )
            break;

        if (amb->m_app->m_appinput.isactive())
            break;

        if (!AppMessageQueue::instance().checkData())
            break;

        AppMessageQueue::DataMessageQueue dataq{};

        if (!AppMessageQueue::instance().getData(dataq))
            break;

        amb->guiStaticText::draw(dataq.s, &dataq.cord, dataq.id);
        amb->guiStaticText::guiBase::ActiveOn();

        if (amb->m_timer.isactive())
            amb->m_timer.stop();

        if (dataq.tm)
            amb->m_timer.once(
                dataq.tm,
                [=]()
                {
                    amb->clear();
                }
            );
    }
    while (0);

    return false;
}


