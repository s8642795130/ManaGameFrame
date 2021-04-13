#pragma once
#include "../Server/IModule.h"
#include "../Server/ClientDescriptor.h"

class ServerControlModule : public IModule
{
private:
public:
	ServerControlModule(std::shared_ptr<IPlugin> ptr) : IModule(ptr)
	{
	}

	virtual void Init();
	virtual void AfterInit();
	virtual void ReadyExecute();
	virtual void Execute();

	//
	void OnServerOnlineCallback(ClientDescriptor* ptr_client);
};

