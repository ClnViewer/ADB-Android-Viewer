
#include "../ADBViewer.h"

static inline const LPCSTR l_openLuaFilter = "Lua script files (*.lua)\0*.lua\0";
static inline const LPCSTR l_openLuaExt = "lua";
static inline const LPCSTR l_openCurDir = ".\\";

AppEditor::AppEditor()
    : ppixel{}, scrtype(SCR_CHECK_AND_CLICK),
      m_app{nullptr}, m_active(false), m_target(false)
    {
        ppixel.x = ppixel.y = -1;
    }

AppEditor::~AppEditor()
    {
        gui.active = false;
    }

bool AppEditor::isactive() const
    {
        return m_active.load();
    }

bool AppEditor::isupdate() const
    {
        return ((m_active.load()) ?
                ((ppixel.x > -1) && (ppixel.y > -1)) : false);
    }

bool AppEditor::init(App *app)
    {
        if (!app)
            return false;

        m_app = app;

        gui.rect.h = 32;
        gui.rect.w = 32;
        gui.rect.x = 0;
        gui.rect.y = (m_app->m_appvideo.gui.rect.h - 32);
        gui.texture = nullptr;

        return initgui(app);
    }

void AppEditor::stop()
    {
        if ((!m_app) || (!m_active.load()))
            return;

        m_active = false;
        m_target = false;

        std::string fname(
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_LUAFILENAME
            )
        );
        if (!AppSysDialog::savefile(m_app->m_window, fname, l_openLuaFilter, l_openLuaExt, l_openCurDir))
            return;

        write_script(fname);
        gui_icon_off();
        m_app->m_appinfo.clear();
    }

void AppEditor::run()
    {
        if ((!m_app) || (m_active.load()))
            return;

        m_pixels.clear();
        m_ptarget.clear();
        ppixel.x = ppixel.y = -1;

        gui_icon_on();

        m_target = false;
        m_active = true;
        m_app->m_appinfo.clear();
    }

void AppEditor::cancel()
    {
        if ((!m_app) || (!m_active.load()))
            return;

        gui_icon_off();

        m_active = false;
        m_target = false;
        m_pixels.clear();
        m_ptarget.clear();
        m_app->m_appinfo.clear();
    }

void AppEditor::update(uint32_t w, uint32_t h, std::vector<uint8_t> & v)
    {
        if ((!m_app) || (!m_active.load()))
            return;

        do
        {
            if (!isupdate())
                break;

            do
            {
                int32_t x = ppixel.x,
                        y = ppixel.y,
                        p = ((m_target) ? 4 : 2),
                        z = ((m_target) ? 9 : 5);

                x = (((x + p) > (int32_t)w) ?
                     ((int32_t)w - z) :
                        (((x - p) < 0) ? 0 : (x - p))
                    );
                y = (((y + p) > (int32_t)h) ?
                     ((int32_t)h - z) :
                        (((y - p) < 0) ? 0 : (y - p))
                    );

                if (m_target)
                    m_ptarget.clear();

                for (int32_t xx = 0; xx < z; xx++)
                    for (int32_t yy = 0; yy < z; yy++)
                    {
                        AppEditor::_PIXELS pixel{};
                        pixel.x = (x + xx);
                        pixel.y = (y + yy);
                        pixel.pos = getpos(pixel.x, pixel.y, w, h);
                        if ((pixel.pos < 0) || ((size_t)pixel.pos >= v.size()))
                            continue;

                        if (m_target)
                        {
                            if (!foundpos(m_ptarget, pixel))
                                continue;
                        }
                        else
                        {
                            if (!foundpos(m_pixels, pixel))
                                continue;
                        }

                        AppEditor::_RGB *rgb = (AppEditor::_RGB*)&v[pixel.pos];
                        pixel.rgb.r = rgb->r;
                        pixel.rgb.g = rgb->g;
                        pixel.rgb.b = rgb->b;

                        if (m_target)
                            m_ptarget.push_back(pixel);
                        else
                            m_pixels.push_back(pixel);

                        /*
                        /// Nvidia crush!
                        if (yy == (z / 2))
                        {
                            std::stringstream ss;
                            ss << " I: "  << std::to_string(pixel.pos);
                            ss << " ( R:" << std::to_string(pixel.rgb.r);
                            ss << ", G:"  << std::to_string(pixel.rgb.g);
                            ss << ", B:"  << std::to_string(pixel.rgb.b) << " ) ";
                            SDL_Point offset = { (m_app->m_appmenubar.gui.rect.w + 1), 0 };
                            m_app->m_appinfo.draw(ss.str(), &offset, -1);
                        }
                        */
                    }
            }
            while (0);

            ppixel.x = ppixel.y = -1;
        }
        while (0);

        AppEditor::_RGB const *rgb_;

        switch (scrtype)
        {
            case AppEditorScriptType::SCR_CHECK_AND_CLICK:
                {
                    rgb_ = &m_redrgb;
                    break;
                }
            case AppEditorScriptType::SCR_CLICK_ONLY:
                {
                    rgb_ = &m_bluergb;
                    break;
                }
            default:
                {
                    rgb_ = &m_greenrgb;
                    break;
                }
        }

        for (auto &rgbs : m_pixels)
            ::memcpy(&v[rgbs.pos], rgb_, sizeof(m_redrgb));

        for (auto &rgbs : m_ptarget)
            ::memcpy(&v[rgbs.pos], &m_bluergb, sizeof(m_bluergb));
    }

int32_t AppEditor::getpad(uint32_t w)
    {
        int32_t pad = 0;
        while ((((int32_t)w + pad) % 4) != 0)
            pad++;
        return pad;
    }

int32_t AppEditor::checkpos(uint32_t w, uint32_t h, int32_t pad)
    {
        return (((((int32_t)w * 3) + pad) * (int32_t)h) - 3);
    }

int32_t AppEditor::getpos(int32_t x, int32_t y, uint32_t w, uint32_t h)
    {
        int32_t pad = getpad(w),
                pos = (((((int32_t)w * 3) + pad) * y) + (x * 3));
        if (pos > checkpos(w, h, pad))
            return -1;
        return pos;
    }

bool AppEditor::foundpos(std::vector<AppEditor::_PIXELS> & v, AppEditor::_PIXELS & p)
    {
        auto a = find_if(
                    v.begin(),
                    v.end(),
                    [=](AppEditor::_PIXELS pix)
                    {
                        return pix.pos == p.pos;
                    }
            );
        if (a != v.end())
        {
            v.erase(a);
            return false;
        }
        return true;
    }

bool AppEditor::event(SDL_Event *ev, const void *instance)
    {
    AppEditor *ape = static_cast<AppEditor*>(
                const_cast<void*>(instance)
            );

    if (
        (!ape) ||
        (AppConfig::instance().cnf_isstop)  ||
        (ape->m_app->m_appinput.isactive())
       )
        return false;

    if (ev->type == ape->m_app->m_uevent)
    {
        switch(ev->user.code)
        {
            case ID_CMD_POP_MENU6:
                {
                    ///  (tiger)
                    if (ape->isactive())
                        /// stop and write LUA script
                        ape->stop();
                    else
                        /// start edit pixels 5x5 for check && identify
                        ape->run();
                    return true;
                }
            case ID_CMD_POP_MENU7:
                {
                    if (ape->isactive())
                        /// cancel and clear pixels list
                        ape->cancel();
                    return true;
                }
            case ID_CMD_POP_MENU8:
                {
                    ///  (tiger)
                    /// add endpoint action
                    m_target = !m_target;
                    return true;
                }
            default:
                break;
        }
    }

    if (!ape->isactive())
        return false;

    switch(ev->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        {
            switch (ev->button.button)
            {
                case SDL_BUTTON_LEFT:
                {
                    if (
                        (ev->motion.x < ape->gui.rect.w) &&
                        (ev->motion.y > ape->gui.rect.y)
                       )
                    {
                        ape->stop();
                        return true;
                    }

                    if (ev->motion.x <= ape->gui.rect.w)
                        break;

                    if (ape->isupdate())
                        break;

                    ape->ppixel.x = (ev->motion.x - ape->m_app->m_appmenubar.gui.rect.w);
                    ape->ppixel.y = ev->motion.y;
                    return true;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return false;
    }

void AppEditor::write_script(std::string const & fname)
    {
        if (fname.empty())
            return;

        int32_t sc = static_cast<int32_t>(AppConfig::instance().cnf_scale);
        std::stringstream ss;
        ss << "\n\nfunction main (stateOld)\n";

        switch (scrtype)
        {
            case AppEditorScriptType::SCR_CHECK_AND_CLICK:
                {
                    ss << "\tlocal tbl_screen01 = {\n";

                    for (auto &rgbs : m_pixels)
                        ss << "\t\t{" << (int)rgbs.pos << "," << (int)rgbs.rgb.r << "," << (int)rgbs.rgb.g << "," << (int)rgbs.rgb.b << "},\n";
                    ss << "\t}\n";

                    if (m_ptarget.size())
                    {
                        int32_t pos = (m_ptarget.size() / 2);
                        ss << "\ttLuaObject:adbClick(";
                        ss << (m_ptarget[pos].x * sc) << "," ;
                        ss << (m_ptarget[pos].y * sc) << ")\n";
                    }
                    ss << "\n\tLuaObject:stateSleep(10)\n";
                    break;
                }
            case AppEditorScriptType::SCR_CLICK_ONLY:
                {
                    int32_t cnt = 0;
                    for (auto &rgbs : m_pixels)
                    {
                        if (!cnt)
                            ss << "\t";
                        else
                            ss << "\telse";

                        ss << "if stateOld == " << cnt << "then LuaObject:adbClick(";
                        ss << (rgbs.x * sc) << "," ;
                        ss << (rgbs.y * sc) << ") end\n";
                    }
                    ss << "\n\tLuaObject:stateSleep(1)\n";
                    break;
                }
            case AppEditorScriptType::SCR_NONE:
                {
                    break;
                }
        }
        ss << "\tLuaObject:stateSet(stateOld + 1)\n";
        ss << "end\n\n";

        if (m_ptarget.size())
            m_ptarget.clear();

        if (m_pixels.size())
            m_pixels.clear();

        FILE __AUTO(__autofile) *fp = fopen(fname.c_str(), "wt");
        if (!fp)
            return;
        fwrite(ss.str().c_str(), 1, ss.str().length(), fp);
    }

void AppEditor::gui_icon_on()
    {
        SDL_Surface *l_image_surface = nullptr;

        do
        {
            gui.texture = SDL_CreateTexture(
                m_app->m_renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                gui.rect.w, gui.rect.h
            );

            if (!gui.texture)
                break;

            if (!(l_image_surface = ResManager::imageload(
                        ResManager::IndexImageResource::RES_IMG_RCORD
                )))
                break;

            SDL_UpdateTexture(gui.texture, nullptr, l_image_surface->pixels, l_image_surface->pitch);
            SDL_FreeSurface(l_image_surface);
            return;
        }
        while (0);

        if (l_image_surface)
            SDL_FreeSurface(l_image_surface);

        gui_icon_off();
    }

void AppEditor::gui_icon_off()
    {
        if (gui.texture)
            SDL_DestroyTexture(gui.texture);

        gui.texture = nullptr;
    }


