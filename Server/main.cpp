#include <iostream>
#include <cstring>
#include <functional>
#include <memory>

#include <unistd.h>

#include "ConfigFile.h"
#include "Application.h"
#include "GameMessage.h"
#include "ClientDescriptor.h"

std::map<int, std::string> NetMsgDefine::m_map_msg;
NetMessage::ServerType ClientDescriptor::m_server_type;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	// int getopt(int argc, char * const argv[], const char * optstring);
	std::cout << "hello world" << std::endl;

	std::array<char, 260> path_buf;
	getcwd(path_buf.data(), 260);
	std::cout << path_buf.data() << std::endl;

	std::cout << "param count: " << argc << std::endl;
	int type = 0;
	if (argc > 1)
	{
		type = std::stoi(argv[1]);
		std::cout << "param is: " << type << std::endl;
	}

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	IApplication::SetPtr(app);
	//
	app->LoadConfig(argv[1]);
	app->StartLoadAllLibrary(type);
	app->StartThreadPool();
	app->StartConnectServer();
	app->StartNetwork(type);
	app->LibInit();
	app->LibExecute();
	app->StartNetEventLoop();

	return 0;
}