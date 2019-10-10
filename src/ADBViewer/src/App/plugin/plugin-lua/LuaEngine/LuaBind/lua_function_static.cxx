
#   if defined(_BUILD_LUA_EDITOR)
    static bool f_image_fb_check(LuaEngine *le, std::string const & s)
    {
        if (le->m_imgdefault.empty())
        {
            std::string err_ = s;
            if (!le->loadimagedefault(err_))
            {
                if (!err_.empty())
                    LuaLint::print_traceT(LuaLint::g_lint_str_27, err_);
                return false;
            }
        }
        return true;
    }
#   else
    static bool f_image_fb_check(LuaEngine *le, std::string const & s)
    {
        if (le->m_imgdefault.empty())
                return false;
        return true;
    }
#   endif

#   if defined(_BUILD_LUA_EDITOR)
    static void f_image_show(
            int32_t idx,
            LuaEngine *le,
            std::string const & s,
            ImageLite::ImageRGBbuffer & img1,
            ImageLite::ImageRGBbuffer & img2)
    {
        switch (idx)
        {
            case 0:
                {
                    le->m_imgdraw.add(0, img1, s);
                    break;
                }
            case 1:
                {
                    le->m_imgdraw.add(1, img1);
                    break;
                }
            case 2:
                {
                    le->m_imgdraw.add(1, img1);
                    le->m_imgdraw.add(2, img2, s);
                    break;
                }
            default:
                return;
        }
        if (!le->m_imgdraw.isactive())
            le->m_imgdraw.show(s);
    }
#   endif

    static bool f_image_comparable(
            LuaEngine *le,
            std::string const & s,
            ImageLite::IRECT<int32_t> & r,
            ImageLite::ImageRGBbuffer & img,
            double & d)
    {
        d = 0.0;
        bool isrectenable = !((r.w == 0) || (r.h == 0));

        LINT_TRY__
        {
            img.load(s);
            if (img.empty())
                return false;

            if (!f_image_fb_check(le, std::string()))
                return false;

            if (isrectenable)
                d = le->m_imgdefault.icompare(img, r);
            else
                d = le->m_imgdefault.icompare(img);
            return true;
        }
        LINT_CATCH__
        return false;
    }

    static bool f_string_to_time(std::string const & s, std::tm & tms)
    {
        if (s.empty())
            return false;

        std::istringstream ss(s.c_str());
        ss >> std::get_time(&tms, "%H:%M:%S");
        if (ss.fail())
            return false;
        return true;
    }


