#pragma once
#include "IServerObjModule.h"
#include "IClientNetModule.h"
#include "IServerNetModule.h"
#include "../ConfigPlugin/IConfigModule.h"

class ServerObjModule : public IServerObjModule
{
private:
	std::shared_ptr<IClientNetModule> m_client_net_module;
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<IServerNetModule> m_server_net_module;
	//
	std::map<std::string, std::string> m_map_server; // key: server_name, value: uuid
protected:
	void SaveServerToMap(const std::string& server_name, const std::string& uuid);
public:
	ServerObjModule(std::shared_ptr<IPluginManager> ptr) : IServerObjModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();

	// interface
	virtual const std::map<std::string, std::string>& GetServerMap();

	// callback
	void OnServerOnlineCallback(IClientNetActor& ptr_client);
};
