#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class guiBoxTextInput
{
private:

    void draw(std::string const & s, SDL_Point *offset)
    {
        if (s.empty())
            return;

        m_guitext.draw(s, offset, -1);
        SDL_SetTextInputRect(&m_guitext.gui.rect);
    }

public:
    //
    bool init(
            guiMain *gm,
            ResManager::IndexFontResource ifx,
            ResManager::IndexColorResource icx
        ) noexcept
    {
        return m_guitext.init(gm, ifx, icx);
    }
    void begin(std::string const & s, SDL_Point *offset)
    {
        if (m_active.load())
            return;

        SDL_StartTextInput();
        m_active = true;
        m_str.erase();
        draw(s, offset);
    }
    void end()
    {
        if (!m_active.load())
            return;

        SDL_StopTextInput();
        m_guitext.clear();
    }
    bool status() const
    {
        return m_active.load();
    }
    bool isresult()
    {
        m_active = false;
        return (!m_str.empty());
    }
    std::string getresult()
    {
        m_active = false;
        return m_str;
    }
    bool event(SDL_Event *ev, SDL_Point *offset)
    {
        if (!m_active.load())
            return false;

        if (ev->type == SDL_KEYDOWN)
        {
            switch (ev->key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    {
                        end();
                        m_str.erase();
                        m_active = false;
                        return false;
                    }
                case SDLK_RETURN:
                case SDLK_RETURN2:
                    {
                        end();
                        return false;
                    }
                case SDLK_BACKSPACE:
                    {
                        std::stringstream ss;
                        if (!m_str.empty())
                            m_str.pop_back();
                        ss << " " << m_str.c_str() << " ";
                        draw(ss.str(), offset);
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
            m_str.append(ev->text.text);
            ss << " " << m_str.c_str() << " ";
            draw(ss.str(), offset);
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
    guiStaticText     m_guitext;
    std::atomic<bool> m_active;
    std::string       m_str;
    //
};
