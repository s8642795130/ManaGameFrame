#include <iostream>

#include <cstring>

#include "ConfigFile.h"
#include "Application.h"

std::shared_ptr<IApplication> IApplication::m_app;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	std::cout << "test !!!" << std::endl;

	// Application
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->LoadConfig();
	app->StartLoadAllLibrary();
	app->StartThreadPool();
	app->StartNetwork();
	app->ConnectMaster();

	return 0;
}