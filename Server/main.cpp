#include <iostream>
#include <cstring>
#include <functional>
#include <memory>

#include <unistd.h>

#include "StringDefine.h"
#include "ConfigFile.h"
#include "PluginManager.h"
#include "../ServerNetPlugin/IPollModule.h"

void PrintLogo()
{
	// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << "\n" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << "*                                              *" << std::endl;
	std::cout << "*                 ManaFrame                    *" << std::endl;
	std::cout << "*                                              *" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << "\n" << std::endl;
}

const std::string GetServerNameFromParam(int argc, char* argv[])
{
	// get work dir
	std::array<char, 260> path_buf;
	getcwd(path_buf.data(), 260);
	std::cout << path_buf.data() << std::endl;

	// server name
	std::string server_name;

	//
	std::cout << "param count: " << argc << std::endl;
	if (argc > 1)
	{
		std::cout << "param is: " << argv[1] << std::endl;
		server_name = argv[1];
	}
	else
	{
		server_name = "master";
	}

	return server_name;
}

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	// int getopt(int argc, char * const argv[], const char * optstring);
	PrintLogo();

	// get server name from main param
	const auto server_name = GetServerNameFromParam(argc, argv);

	// server log
	std::cout << server_name << IS_RUNNING << std::endl;

	// config
	std::shared_ptr<ConfigFile> config_file{ std::make_shared<ConfigFile>() };
	config_file->SetServerName(server_name);
	config_file->ReadServerConfigFile();

	// manager
	std::shared_ptr<PluginManager> manager{ std::make_shared<PluginManager>() };

	// set server name
	manager->SetServerName(server_name);

	// load plugin
	auto plugin_list = config_file->GetPluginsByServerName(config_file->GetMyServerInfo()->m_server_name);
	manager->LoadAllPluginLibrary(plugin_list);

	// life cycle
	manager->Init();
	manager->AfterInit();
	manager->ReadyExecute();
	manager->Execute();

	// start network loop
	auto poll_module = manager->GetModule<IPollModule>();
	poll_module->EventLoop();

	return 0;
}