/*
  include in all plugins in plugin-stub.cpp for all
  define PLUGIN_CLASS_NAME = Plugins::PluginName
 */

static PLUGIN_CLASS_NAME * l_PluginInstance = nullptr;

__attribute__((destructor)) static void plugin_free_destructor(void)
{
    if (l_PluginInstance)
        delete l_PluginInstance;
    l_PluginInstance = nullptr;
}

extern "C" void * DLL_EXPORT CreatePlugin(const char *s, const void *vcb)
{
    assert(l_PluginInstance == nullptr);
    l_PluginInstance = new PLUGIN_CLASS_NAME(s, vcb);
    return (void*)l_PluginInstance;
}

extern "C" void DLL_EXPORT DestroyPlugin(void)
{
    assert(l_PluginInstance);
    delete l_PluginInstance;
    l_PluginInstance = nullptr;
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(
        HINSTANCE hinstDLL __ATTR_UNUSED, DWORD fdwReason, LPVOID pvReserved __ATTR_UNUSED)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

