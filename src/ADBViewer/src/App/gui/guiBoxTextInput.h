#pragma once

class guiBoxTextInput : public guiStaticText
{
private:

    void draw(std::string const & s, SDL_Point & point)
    {
        if (s.empty())
            return;

        guiStaticText::draw(s, &point, -1);
        SDL_SetTextInputRect(&gui.rect);
    }

public:

    void begin(std::string const & s)
    {
        if (m_active.load())
            return;

        SDL_StartTextInput();
        m_active = true;
        m_str.erase();
        draw(s, AppConfig::instance().cnf_point_input);
    }
    void end()
    {
        if (!m_active.load())
            return;

        SDL_StopTextInput();
        guiStaticText::clear();
    }
    bool isactive() const
    {
        return m_active.load();
    }
    bool isresult()
    {
        end();
        m_active = false;
        return (!m_str.empty());
    }
    std::string getresult()
    {
        end();
        m_active = false;
        return m_str;
    }
    bool event(SDL_Event *ev, const void *instance) override
    {
        guiBoxTextInput *bti = static_cast<guiBoxTextInput*>(
                const_cast<void*>(instance)
            );

        if ((AppConfig::instance().cnf_isstop) || (!bti->m_active.load()))
            return false;

        if (ev->type == SDL_KEYDOWN)
        {
            switch (ev->key.keysym.sym)
            {
                case SDLK_v:
                    {
                        if ((SDL_GetModState() & KMOD_CTRL) && (SDL_HasClipboardText()))
                        {
                            char *tmp;

                            if (!(tmp = SDL_GetClipboardText()))
                                break;
                            std::stringstream ss;
                            bti->m_str.assign(tmp);
                            SDL_free(tmp);
                            ss << " " << m_str.c_str() << " ";
                            draw(ss.str(), AppConfig::instance().cnf_point_input);
                        }
                        break;
                    }
                case SDLK_c:
                    {
                        if (SDL_GetModState() & KMOD_CTRL)
                            SDL_SetClipboardText(bti->m_str.data());
                        break;
                    }
                case SDLK_ESCAPE:
                    {
                        end();
                        bti->m_str.erase();
                        bti->m_active = false;
                        return true;
                    }
                case SDLK_RETURN:
                case SDLK_RETURN2:
                    {
                        break;
                    }
                case SDLK_BACKSPACE:
                    {
                        std::stringstream ss;
                        if (!bti->m_str.empty())
                            bti->m_str.pop_back();
                        ss << " " << bti->m_str.c_str() << " ";
                        draw(ss.str(), AppConfig::instance().cnf_point_input);
                        break;
                    }
                case SDLK_LEFT:
                    {
                        break;
                    }
                case SDLK_RIGHT:
                    {
                        break;
                    }
                case SDLK_UP:
                    {
                        break;
                    }
                case SDLK_DOWN:
                    {
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }
        else if (ev->type == SDL_TEXTINPUT)
        {
            std::stringstream ss;
            bti->m_str.append(ev->text.text);
            ss << " " << bti->m_str.c_str() << " ";
            draw(ss.str(), AppConfig::instance().cnf_point_input);
        }
        else if (ev->type == SDL_MOUSEBUTTONDOWN)
        {

        }
        return true;
    }

    guiBoxTextInput()
     : m_pos(0), m_active(false) {}
    ~guiBoxTextInput() {}

private:
    //
    int32_t           m_pos;
    std::atomic<bool> m_active;
    std::string       m_str;
    //
};
