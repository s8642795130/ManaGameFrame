#include <memory>
#include <iostream>

#include "ConfigPlugin.h"

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IPluginManager> ptr)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<ConfigPlugin> plugin{ std::make_shared<ConfigPlugin>(ptr) };
	return plugin;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}