#include <iostream>

#include <cstring>

#include "ConfigFile.h"
#include "Application.h"
#include "ServerNet.h"
#include "Client.h"
#include "MasterObj.h"

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	std::cout << "test !!!" << std::endl;

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->StartThreadPool();

	// Connect Master
    MasterObj* master_obj = new MasterObj();
    master_obj->ConnectMaster();

	// Actor Manager

	// Network
	ServerNet<Client> server("0.0.0.0", 3010, 30);
    server.AddFD(master_obj);
	server.EventLoop();

	return 0;
}