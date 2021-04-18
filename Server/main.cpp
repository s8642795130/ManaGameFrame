#include <iostream>

#include <cstring>

#include "ConfigFile.h"
#include "Application.h"
#include "ServerNet.h"
#include "MasterObj.h"
#include "PluginManager.h"

std::shared_ptr<IApplication> IApplication::m_app;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	std::cout << "test !!!" << std::endl;

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->StartLoadAllLibrary();
	app->StartThreadPool();
	app->StartNetwork();

	// Connect Master
    MasterObj* master_obj = new MasterObj();
    master_obj->ConnectMaster();

	// Plugin Manager
	std::shared_ptr<PluginManager> pluginManager{ std::make_shared<PluginManager>() };
	pluginManager->LoadAllPluginLibrary();

	// Network
	app->StartNetwork();
	ServerNet<Client> server("0.0.0.0", 3010, 30);
    server.AddFD(master_obj);
	server.EventLoop();

	return 0;
}