#pragma once
#include <memory>

#include "IClientNetModule.h"
#include "IClientNetActor.h"
#include "ClientPimpl.h"

class ClientNetModule : public IClientNetModule
{
private:
	std::map<int, std::shared_ptr<IClientNetActor>> m_map_clients; // key: fd, value: clientNet
	std::map<std::string, std::shared_ptr<IClientNetActor>> m_logged_in_clients; // key: uid, value: clientNet
	std::shared_ptr<ClientPimpl> m_client_pimpl;
public:
	ClientNetModule(std::shared_ptr<IPluginManager> ptr) : IClientNetModule(ptr)
	{
	}

	// life cycle
	virtual void Init();

	// interface
	virtual std::shared_ptr<IClientNetActor> CreateClientNet();
	virtual void AddClientToMap(std::shared_ptr<IClientNetActor>& ptr);
	virtual std::shared_ptr<IClientNetActor> GetClientNet(const int& fd);
	virtual void RemoveClientFromMap(const int& fd);
	// login client interface
	virtual void AddLoginClientToMap(std::shared_ptr<IClientNetActor>& ptr);
	virtual std::shared_ptr<IClientNetActor> GetLoginClient(const std::string& uid);
	virtual void RemoveLoginClientFromMap(const std::string& uid);
};

