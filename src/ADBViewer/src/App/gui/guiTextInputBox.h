#pragma once

class guiTextInputBox : public guiStaticText
{
private:

    bool tinit(SDL_Texture**) override;
    void draw(std::string const&, SDL_Point&);

public:
    //
    std::string       m_prompt;
    SDL_Point         m_point;
    //

    guiTextInputBox();
    guiTextInputBox& operator=(guiTextInputBox const&) = delete;

    bool init(
            App*,
            SDL_Point&,
            std::string const&,
            ResManager::IndexFontResource,
            ResManager::IndexColorResource,
            ResManager::IndexColorResource
        );
    void begin();
    void begin(std::string const&);
    void end();
    bool isactive() const;
    bool isresult();
    std::string getresult();
    std::string getresult(std::string const&);
    bool eventcb(SDL_Event*);
    void setcord(SDL_Point&);
    void settext(std::string const&);

private:
    //
    uint32_t          m_history_idx = 0U;
    std::atomic<bool> m_active = false;
    std::string       m_str;
    std::string       m_history[10]; /// create history
    std::valarray<std::string> m_history_array;
    guiCursor         m_cursor;
    //
    void addhistory(std::string const&);
    bool gethistory(std::string&, int32_t);
    void drawstring(int32_t, std::string&, std::string const&);
    guiCursor::IndexTextType getencode(std::string const &);
};
