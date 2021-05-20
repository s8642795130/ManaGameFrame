#include <iostream>
#include <cstring>
#include <functional>
#include <memory>

#include <unistd.h>

#include "ConfigFile.h"
#include "Application.h"
#include "GameMessage.h"
#include "ServerType.h"

std::map<int, std::string> NetMsgDefine::m_map_msg;
NetMessage::ServerType ServerType::m_server_type;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	// int getopt(int argc, char * const argv[], const char * optstring);
	std::cout << "hello world" << std::endl;
	std::string plugin_name;

	std::array<char, 260> path_buf;
	getcwd(path_buf.data(), 260);
	std::cout << path_buf.data() << std::endl;

	std::cout << "param count: " << argc << std::endl;
	if (argc > 1)
	{
		std::cout << "param is: " << argv[1] << std::endl;
		plugin_name = argv[1];
	}
	else
	{
		plugin_name = "master-1";
	}

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	IApplication::SetPtr(app);
	//
	app->LoadConfig(plugin_name);
	app->StartLoadAllLibrary();
	app->StartThreadPool();
	if (argc > 1)
	{
		app->StartConnectServer();
	}
	app->StartNetwork();
	app->LibInit();
	app->LibAfterInit();
	app->LibReadyExecute();
	app->LibExecute();
	app->StartNetEventLoop();

	return 0;
}