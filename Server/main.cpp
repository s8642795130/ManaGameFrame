#include <iostream>
#include <cstring>
#include <functional>
#include <memory>

#include "ConfigFile.h"
#include "Application.h"

std::shared_ptr<IApplication> IApplication::m_app;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	// int getopt(int argc, char * const argv[], const char * optstring);
	std::cout << "hello world" << std::endl;

	std::cout << "param count: " << argc << std::endl;
	int type = 0;
	if (argc > 1)
	{
		type = std::stoi(argv[1]);
		std::cout << "param is: " << type << std::endl;
	}
	type = 1;

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	IApplication::SetPtr(app);
	//
	app->LoadConfig();
	// app->StartLoadAllLibrary(type);
	// app->LibInit();
	// app->LibAfterInit();
	app->StartThreadPool();
	app->StartNetwork(type);
	// app->LibReadyExecute();
	// app->LibExecute();
	app->StartNetEventLoop();

	return 0;
}