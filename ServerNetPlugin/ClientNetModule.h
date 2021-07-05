#pragma once
#include <memory>

#include "IClientNetModule.h"
#include "ClientPimpl.h"
#include "INetActor.h"

class ClientNetModule : public IClientNetModule
{
private:
	std::map<int, std::shared_ptr<INetActor>> m_map_clients; // key: fd, value: clientNet
	std::map<std::string, std::shared_ptr<INetActor>> m_logged_in_clients; // key: uid, value: clientNet
	std::shared_ptr<ClientPimpl> m_client_pimpl;
	//
	std::shared_ptr<IConfigModule> m_config_module;
public:
	ClientNetModule(std::shared_ptr<IPluginManager> ptr) : IClientNetModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();

	// interface
	virtual std::shared_ptr<INetActor> CreateHttpClientNet(ITcpServer* ptr_sender);
	virtual std::shared_ptr<INetActor> CreateSocketClientNet(ITcpServer* ptr_sender);
	virtual void AddClientToMap(std::shared_ptr<INetActor>& ptr);
	virtual std::shared_ptr<INetActor> GetClientNet(const int& fd);
	virtual void RemoveClientFromMap(const int& fd);
	// login client interface
	virtual void AddLoginClientToMap(std::shared_ptr<INetActor>& ptr);
	virtual std::shared_ptr<INetActor> GetLoginClient(const std::string& uid);
	virtual void RemoveLoginClientFromMap(const std::string& uid);
};

