#include <memory>
#include <iostream>

#include "MasterPlugin.h"
#include "MasterActor.h"

std::shared_ptr<IApplication> IApplication::m_app;

extern "C" __attribute((visibility("default"))) void DllStartPlugin()
{
	//CREATE_PLUGIN(pm, NFChatPlugin)
	std::shared_ptr<MasterPlugin> ptr{ std::make_shared<MasterPlugin>() };
	std::cout << "1 use_count: " << ptr.use_count() << std::endl;

	std::shared_ptr<MasterActor> test_ptr{ std::make_shared<MasterActor>() };
	std::cout << "2 use_count: " << test_ptr.use_count() << std::endl;
}

extern "C" __attribute((visibility("default"))) void DllStopPlugin()
{
	//DESTROY_PLUGIN(pm, NFChatPlugin)
}