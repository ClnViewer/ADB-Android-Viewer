_CMD_BUILD(IDM_MAIN_SHOW_VERSION,

    std::stringstream ss;
    ss << " -- Version: " << AVIEW_FULLVERSION_STRING;
    ss << ", build date: " << AVIEW_DATE "/" AVIEW_MONTH "/" AVIEW_YEAR;
    ss << ", builder: mingw-gcc " << __VERSION__;
    DEFAULTBox(ss.str());
)
