#pragma once
#include "IServerObjModule.h"
#include "IClientNetModule.h"

class ServerObjModule : public IServerObjModule
{
private:
	std::shared_ptr<IClientNetModule> m_client_net_module;
	//
	std::map<std::string, std::string> m_map_server; // key: server_name, value: uuid
public:
	ServerObjModule(std::shared_ptr<IPluginManager> ptr) : IServerObjModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();

	// interface
	virtual const std::map<std::string, std::string>& GetServerMap();
};

