#include <memory>
#include <iostream>

#include "ActorPlugin.h"

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IPluginManager> ptr)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<ActorPlugin> plugin{ std::make_shared<ActorPlugin>(ptr) };
	return plugin;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}