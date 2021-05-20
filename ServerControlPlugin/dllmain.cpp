#include <memory>
#include <iostream>

#include "ServerControlPlugin.h"

std::shared_ptr<IApplication> IApplication::m_app;

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IApplication> app)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<ServerControlPlugin> ptr{ std::make_shared<ServerControlPlugin>(app) };
	return ptr;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}