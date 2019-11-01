
case IDM_BTN_EXIT:
{
    if ((!ebc->issaved()) && (!MDIWin::Config::instance().issaved))
    {
        INFOBox(g_scedit_saved);
        MDIWin::Config::instance().issaved = true;
        break;
    }
    ::SendMessageA(hwnd, WM_CLOSE, wParam, lParam);
    break;
}
case IDM_BTN_SHOW_VERSION:
{
    //
    try
    {
        std::stringstream ss;
        ss << " -- Version: " << AVIEW_FULLVERSION_STRING;
        ss << ", build date: " << AVIEW_DATE "/" AVIEW_MONTH "/" AVIEW_YEAR;
        ss << ", builder: mingw-gcc " << __VERSION__;
        DEFAULTBox(ss.str());
    }
    catch (...) { GameDev::ExceptionPrn::parse(std::current_exception()); }
    //
    break;
}
case IDM_BTN_WINTOP:
{
    MDIWin::Config::instance().iswintop =
            !(MDIWin::Config::instance().iswintop.load());
    ::SetWindowPos(
            MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
            ((MDIWin::Config::instance().iswintop.load()) ? HWND_TOPMOST : HWND_NOTOPMOST),
            0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE
    );
    tbc->event(id, MDIWin::Config::instance().iswintop.load());
    {
        std::stringstream ss;
        ss << " -- set SCEdit ";
        if (MDIWin::Config::instance().iswintop.load())
            ss << "always top";
        else
            ss << "normal mode";
        DEFAULTBox(ss.str());
    }
    break;
}
case IDM_BTN_INFOCLEAR:
{
    DEFAULTBox("");
    break;
}
case IDM_EVENT_SET_CAPTION:
{
    const char *txt =
            reinterpret_cast<const char*>(lParam);
    if (!txt)
        break;

    std::stringstream ss;
    ss << MDIWin::Config::instance().appname.c_str();
    ss << ": " << txt;

    ::SetWindowTextA(
            MDIWin::Config::instance().gethandle<MDIWin::Config::HandleType::HWND_MAIN>(),
            ss.str().c_str()
        );
    break;
}
case IDM_BTN_PLUGIN_RESCAN:
case IDM_EVENT_PLUGIN_INIT:
{
    //
    m_luap.init();
    //
    auto vtbl = m_luap.getlist();
    if (!vtbl.size())
        break;
    //
    for (auto & item : vtbl)
    {
        std::stringstream ss;
        ss << " + plugin available: [" << std::get<0>(item);
        ss << "] "                     << std::get<2>(item);
        ss << ", \""                   << std::get<3>(item);
        ss << "\", type: "             << std::get<1>(item);
        DEFAULTBox(ss.str());
    }
    tbc->event(MenuBar::MainMenuId::MMID_Plugins, 2, vtbl);
    break;
}
