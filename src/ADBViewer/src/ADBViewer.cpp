
#include "ADBViewer.h"

int main( int argc, char * argv[])
{
#   if defined(_DEBUG)
    static const char *CrashLogFileName = "ADBViewer.rpt";
    ExcHndlInit();
    ExcHndlSetLogFileNameA(CrashLogFileName);
#   endif

    try
    {
        App app;
        app.loop();
    }
    catch(std::exception const & _ex)
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
            ),
            _ex.what(),
            nullptr
        );
    }
    catch(...)
    {
        SDL_ShowSimpleMessageBox(
            0,
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_APP,
                AppConfig::instance().cnf_lang
            ),
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN,
                AppConfig::instance().cnf_lang
            ),
            nullptr
        );
    }
    return 0;
}
