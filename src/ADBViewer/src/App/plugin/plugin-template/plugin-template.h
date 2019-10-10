#ifndef __MAIN_PLUGIN_H__
#define __MAIN_PLUGIN_H__

# include "../plugin-base.h"

#ifdef __cplusplus

namespace Plugins
{
	class PluginTemplate : public AppIPlugin
	{
    private:
        bool     m_test_swap_click = false;
        uint32_t m_test_count      = 0U;

	public:
		PluginTemplate(const char*, const void*);
		~PluginTemplate() = default;
		void go(std::vector<uint8_t> const &, uint32_t, uint32_t)  override;
	};
}

#endif

#endif
