#pragma once
#include <map>

#include "../Server/IModule.h"
#include "../Server/ClientDescriptor.h"
#include "../Server/ServerObj.h"

class ServerControlModule : public IModule
{
private:
	std::map<int, std::shared_ptr<ServerObj>> m_map_server_obj;
public:
	ServerControlModule(std::shared_ptr<IPlugin> ptr) : IModule(ptr)
	{
	}

	virtual void Init();
	virtual void AfterInit();
	virtual void Execute();

	// function
	void ConnectMaster();
	// callback
	void OnServerOnlineCallback(ClientDescriptor* ptr_client);
	void OnServerConnectCallback(ClientDescriptor* ptr_client);
	void OnFrontendMsgCallback(ClientDescriptor* ptr_client);
};

