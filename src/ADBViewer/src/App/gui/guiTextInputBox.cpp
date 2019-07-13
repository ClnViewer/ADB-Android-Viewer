/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include "../../ADBViewer.h"

#define DRAWLINE1(A) \
    { \
        draw(A, m_point); \
        m_cursor.setstring(getencode(A), A, A.length()); \
    }

#define DRAWLINE2(A,B) \
    { \
        draw(A, m_point); \
        m_cursor.setstring(getencode(A), A, B); \
    }

#define DRAWLINE3(A,B,C) \
    { \
        draw(B, m_point); \
        m_cursor.setstring(A, B, C); \
    }

static bool l_check_encode(std::string const & s, bool ischar)
    {
        if (!((ischar) ?
                    AppConfig::instance().IsUtf8c(s) :
                    AppConfig::instance().IsUtf8(s)
                ))
            return true;

        AddMessageQueue(
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ADB_LANG_NOT_SUPPORT,
                AppConfig::instance().cnf_lang
            ),
            10U,
            -1
        );
        return false;
    }

guiTextInputBox::guiTextInputBox()
    {
        m_history_array = std::valarray<std::string>(
                            m_history,
                            std::size(m_history)
                        );
    }

bool guiTextInputBox::init(
            App *app,
            SDL_Point & point,
            std::string const & prompt,
            ResManager::IndexFontResource ifx,
            ResManager::IndexColorResource icx,
            ResManager::IndexColorResource icc
        )
    {
        do
        {
            m_point = point;
            m_prompt = prompt;

            if (!guiStaticText::init(app, ifx, icx))
                break;

            if (!m_cursor.init(app, point, ifx, icc))
                break;

            return true;
        }
        while (0);

        return false;
    }

bool guiTextInputBox::tinit(SDL_Texture**)
    {
        return true;
    }

void guiTextInputBox::setcord(SDL_Point & p)
    {
        m_point = p;
        m_cursor.setcord(p);
    }

void guiTextInputBox::draw(std::string const & s, SDL_Point & point)
    {
        if (s.empty())
            return;

        guiStaticText::draw(s, &point, -1);
    }

void guiTextInputBox::begin()
    {
        begin(m_prompt);
    }

void guiTextInputBox::begin(std::string const & s)
    {
        if (m_active.load())
            return;

        m_active = true;
        SDL_StartTextInput();
        m_str.erase();
        DRAWLINE1(s);
        m_cursor.On();
    }

void guiTextInputBox::end()
    {
        if (!m_active.load())
            return;

        SDL_StopTextInput();
        m_active = false;
        m_cursor.Off();
        guiStaticText::clear();
    }

bool guiTextInputBox::isactive() const
    {
        return m_active.load();
    }

bool guiTextInputBox::isresult()
    {
        return (!m_str.empty());
    }

std::string guiTextInputBox::getresult(std::string const & prompt)
    {
        std::string s = getresult();
        DRAWLINE1(prompt);
        return s;
    }

std::string guiTextInputBox::getresult()
    {
        std::string s;

        if (!l_check_encode(m_str, true))
        {
            s = ResManager::stringload(
                    ResManager::IndexStringResource::RES_STR_ADB_LANG_NOT_SUPPORT_ENG,
                    AppConfig::instance().cnf_lang
                );
        }
        else
        {
            addhistory(m_str);
            s.assign(m_str.c_str());
        }
        m_str.erase();
        return s;
    }

guiCursor::IndexTextType guiTextInputBox::getencode(std::string const & s)
    {
        if (AppConfig::instance().IsUtf8(s))
            return guiCursor::IndexTextType::TEXT_UTF8;
        return guiCursor::IndexTextType::TEXT_ANSI;
    }

bool guiTextInputBox::gethistory(std::string & s, int32_t flag)
    {
        uint32_t i = m_history_idx,
                 asz = std::size(m_history),
                 isz = (asz - 1U);

        switch (flag)
        {
            case SDLK_PAGEUP:
            case SDLK_UP:
                {
                    i = ((!i) ? isz : (i - 1U));
                    for (; i >= 0U; i--)
                        if (!m_history_array[i].empty())
                            break;
                    break;
                }
            case SDLK_PAGEDOWN:
            case SDLK_DOWN:
                {
                    i = ((i >= isz) ? isz : (i + 1U));
                    for (; i <= isz; i++)
                        if (!m_history_array[i].empty())
                            break;

                    i = ((i == asz) ? 0U : i);
                    break;
                }
        }

        m_history_idx = i;

        if (m_history_array[m_history_idx].empty())
            return false;

        s.assign(m_history_array[m_history_idx]);
        return true;
    }

void guiTextInputBox::addhistory(std::string const & s)
    {
        if (s.empty())
            return;

        uint32_t i = 0U,
                 asz = std::size(m_history);

        for (; i < asz; i++)
            if (m_history_array[i].empty())
                break;

        m_history_idx = ((i < asz) ? i : (asz - 1U));

        if (m_history_idx == (asz - 1U))
            m_history_array = m_history_array.shift(-1);

        m_history_array[m_history_idx].assign(s.c_str());
    }

bool guiTextInputBox::eventcb(SDL_Event *ev)
    {
        /// Enabled only system key:
        /// KMOD_NUM, KMOD_CAPS,
        /// KMOD_RSHIFT, KMOD_LSHIFT, KMOD_SHIFT,
        /// KMOD_CTRL*

        if (ev->type == SDL_KEYDOWN)
        {
            if (ev->key.keysym.mod != KMOD_NONE)
                for (auto & m : AppConfig::instance().cnf_keymod_alt)
                    if (ev->key.keysym.mod & m)
                        return false;

            switch (ev->key.keysym.sym)
            {
                case SDLK_v:
                    {
                        if ((SDL_GetModState() & KMOD_CTRL) && (SDL_HasClipboardText()))
                        {
                            char *tmp = nullptr;

                            do
                            {
                                if (!(tmp = SDL_GetClipboardText()))
                                    break;

                                //if (!l_check_encode(tmp, false))
                                    //break;

                                drawstring(SDLK_v, m_str, tmp);
                            }
                            while (0);

                            if (tmp)
                                SDL_free(tmp);
                        }
                        break;
                    }
                case SDLK_c:
                    {
                        if (SDL_GetModState() & KMOD_CTRL)
                            SDL_SetClipboardText(m_str.data());
                        break;
                    }
                case SDLK_ESCAPE:
                    {
                        return false;
                    }
                case SDLK_RETURN:
                case SDLK_RETURN2:
                    {
                        return false;
                    }
                case SDLK_PAGEDOWN:
                case SDLK_PAGEUP:
                    {
                        return false;
                    }
                case SDLK_BACKSPACE:
                    {
                        drawstring(SDLK_BACKSPACE, m_str, "");
                        break;
                    }
                case SDLK_LEFT:
                    {
                        drawstring(SDLK_LEFT, m_str, "");
                        break;
                    }
                case SDLK_RIGHT:
                    {
                        drawstring(SDLK_RIGHT, m_str, "");
                        break;
                    }
                case SDLK_DELETE:
                    {
                        drawstring(SDLK_DELETE, m_str, "");
                        break;
                    }
                case SDLK_UP:
                case SDLK_DOWN:
                    {
                        m_str.erase();
                        if (gethistory(m_str, ev->key.keysym.sym))
                        {
                            if (!m_str.empty())
                                DRAWLINE1(m_str);
                        }
                        break;
                    }
            }
        }
        else if (ev->type == SDL_TEXTINPUT)
        {
            if (l_check_encode(ev->text.text, true))
                drawstring(SDL_TEXTINPUT, m_str, ev->text.text);
        }
        else if (ev->type == SDL_MOUSEBUTTONDOWN)
        {
            return false;
        }
        return true;
    }

void guiTextInputBox::drawstring(
            int32_t idx, std::string & s, std::string const & sa
        )
    {
        std::stringstream ss;
        bool ischangepos = false;
        int32_t off = 1, pos = m_cursor.getpos();
        pos = std::max(0, pos);

        switch (idx)
        {
            case SDLK_v:
                {
                    s.assign(sa.c_str());
                    break;
                }
            case SDLK_DELETE:
                {
                    if ((!s.empty()) && (s.length() >= (size_t)pos))
                    {
                        AppConfig::instance().IsUtf8(s, &off);
                        off = std::max(1, off);

                        if (off > 1)
                            s.erase(((pos) ? (pos - (off - 1)) : pos), off);
                        else
                            s.erase(((pos) ? (pos - 1) : pos), 1);
                    }
                    ischangepos = true;
                    break;
                }
            case SDLK_BACKSPACE:
                {
                    if (!s.empty())
                    {
                        bool ispos = (s.length() > (size_t)pos);
                        AppConfig::instance().IsUtf8(s, &off);
                        off = std::max(1, off);

                        for (int32_t i = 0U; i < off; i++)
                        {
                            if (ispos)
                            {
                                if (off > 1)
                                {
                                    bool ispad = (pos >= off);
                                    s.erase(((ispad) ? (pos - off) : pos), off);
                                    pos = ((ispad) ? (pos - 1) : pos);
                                }
                                else
                                {
                                    bool ispad = (pos >= 2);
                                    s.erase(((ispad) ? (pos - 2) : pos), 1);
                                    pos = ((ispad) ? (pos - 1) : 1);
                                }
                            }
                            else
                                s.pop_back();
                        }
                        if (ispos)
                            ischangepos = true;
                    }
                    break;
                }
            case SDL_TEXTINPUT:
                {
                    int32_t len = s.length();

                    if (!len)
                    {
                        s.assign(sa.c_str());
                    }
                    else if (len > pos)
                    {
                        s.insert((pos - 1), sa.c_str());
                        ischangepos = true;
                        pos++;
                    }
                    else if (len < pos)
                    {
                        s.append(sa.c_str());
                    }
                    else if (len == pos)
                    {
                        s.append(sa.c_str());
                    }
                    else
                        return;
                }
        }

        switch (idx)
        {
            case SDLK_v:
            case SDLK_BACKSPACE:
            case SDL_TEXTINPUT:
                {
                    if (!ischangepos)
                        pos = s.length() + 2;
                    break;
                }
            case SDLK_LEFT:
            case SDLK_RIGHT:
                {
                    AppConfig::instance().IsUtf8(s, &off);
                    off = std::max(1, off);
                    break;
                }
        }

        ss << " " << s.c_str() << " ";

        switch (idx)
        {
            case SDLK_LEFT:
                {
                    pos = ((pos < off) ? 1 : (pos - off));
                    ischangepos = true;
                    break;
                }
            case SDLK_RIGHT:
                {
                    int32_t len = ss.str().length();
                    pos = ((pos > len) ? len : (pos + off));
                    ischangepos = true;
                    break;
                }
        }
        DRAWLINE2(ss.str(), ((!ischangepos) ? (pos - 1) : pos));
    }
