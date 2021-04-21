#include <iostream>

#include <cstring>

#include "ConfigFile.h"
#include "Application.h"

#include "ActorMsg.h"
#include "IActor.h"

class A : public IActor
{
public:
	A() : IActor()
	{

	}

	virtual ~A()
	{

	}

	void SendMsg(const int a, const int b, const char* p, const int length)
	{
		std::cout << a << " " << b << " " << p << " " << length << std::endl;
	}
};

std::shared_ptr<IApplication> IApplication::m_app;

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	std::cout << "test !!!" << std::endl;

	// Application
	/*
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->LoadConfig();
	app->StartLoadAllLibrary();
	app->StartThreadPool();
	app->StartNetwork();
	app->ConnectMaster();
	*/

	A a;
	std::unique_ptr<IActorMsg> ptr{ std::make_unique<ActorMsg<void, A, const int, const int, const char*, const int>>("send", "recv", &A::SendMsg, 10, 20, "40", 30) };
	ptr->Act(&a);

	return 0;
}