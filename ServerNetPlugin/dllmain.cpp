#include <memory>
#include <iostream>

// #include "MasterPlugin.h"
// #include "MasterActor.h"

extern "C" __attribute((visibility("default"))) std::shared_ptr<IPlugin> DllStartPlugin(std::shared_ptr<IApplication> m_ptr_manager)
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<MasterPlugin> ptr{ std::make_shared<MasterPlugin>(m_ptr_manager) };
	return ptr;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}