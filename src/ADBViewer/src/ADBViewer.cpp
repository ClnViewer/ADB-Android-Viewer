
#include "ADBViewer.h"

int main( int argc, char * argv[])
{
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
                ResManager::IndexStringResource::RES_STR_ERR_APP
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
                ResManager::IndexStringResource::RES_STR_ERR_APP
            ),
            ResManager::stringload(
                ResManager::IndexStringResource::RES_STR_ERR_UNKNOWN
            ),
            nullptr
        );
    }
    return 0;
}
