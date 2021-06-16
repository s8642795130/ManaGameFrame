#include <memory>
#include <iostream>

#include "LoginPlugin.h"

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IPluginManager> ptr)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<LoginPlugin> plugin{ std::make_shared<LoginPlugin>(ptr) };
	return plugin;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}