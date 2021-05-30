#include <memory>
#include <iostream>

#include "ServerNetPlugin.h"

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IPluginManager> ptr)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<ServerNetPlugin> plugin{ std::make_shared<ServerNetPlugin>(ptr) };
	return plugin;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}