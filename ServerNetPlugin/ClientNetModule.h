#pragma once
#include <memory>

#include "HPSocket.h"

#include "../ActorPlugin/ThreadSafeMap.h"
#include "IClientNetModule.h"
#include "ClientPimpl.h"
#include "INetActor.h"

class ClientNetModule : public IClientNetModule
{
private:
	// module
	std::shared_ptr<IConfigModule> m_config_module;
	// member
	std::shared_ptr<ClientPimpl> m_client_pimpl;
	ThreadSafeMap<std::string, std::string> m_map_login_clients; // uid, uuid
public:
	ClientNetModule(std::shared_ptr<IPluginManager> ptr) : IClientNetModule(ptr)
	{
	}

	// life cycle
	virtual void Init() override;

	// interface
	virtual std::shared_ptr<INetActor> CreateHttpClientNet(IHttpServer* ptr_sender) override;
	virtual std::shared_ptr<INetActor> CreateWsClientNet(IHttpServer* ptr_sender) override;
	virtual std::shared_ptr<INetActor> CreateSocketClientNet(ITcpServer* ptr_sender) override;
	virtual std::shared_ptr<INetActor> CreateBackendClientNet(ITcpServer* ptr_sender);
	virtual std::shared_ptr<INetActor> CreateMasterNet(ITcpServer* ptr_sender) override;
	virtual std::shared_ptr<IPollClient> CreatePollMasterClient() override;
	virtual std::shared_ptr<IPollClient> CreatePollClient() override;
	// login client interface
	virtual void AddLoginClientToMap(const std::string& uid, const std::string& uuid);
	virtual const std::string GetLoginClient(const std::string& uid);
	virtual void RemoveLoginClientFromMap(const std::string& uid);
};

