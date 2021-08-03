#pragma once
#include "../Server/IModule.h"

#include "SocketInterface.h"

class INetActor;
// class IHttpServer;
// class ITcpServer;
class IPollClient;

class IClientNetModule : public IModule
{
public:
	IClientNetModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	virtual std::shared_ptr<INetActor> CreateHttpClientNet(IHttpServer* ptr_sender) = 0;
	virtual std::shared_ptr<INetActor> CreateWsClientNet(IHttpServer* ptr_sender) = 0;
	virtual std::shared_ptr<INetActor> CreateSocketClientNet(ITcpServer* ptr_sender) = 0;
	virtual std::shared_ptr<INetActor> CreateBackendClientNet(ITcpServer* ptr_sender) = 0;
	virtual std::shared_ptr<INetActor> CreateMasterNet(ITcpServer* ptr_sender) = 0;
	virtual std::shared_ptr<IPollClient> CreatePollMasterClient() = 0;
	virtual std::shared_ptr<IPollClient> CreatePollClient() = 0;
	// login client interface
	virtual void AddLoginClientToMap(const std::string& uid, const std::string& uuid) = 0;
	virtual const std::string GetLoginClient(const std::string& uid) = 0;
	virtual void RemoveLoginClientFromMap(const std::string& uid) = 0;
};

