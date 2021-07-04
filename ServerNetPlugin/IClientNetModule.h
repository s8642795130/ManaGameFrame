#pragma once
#include "../Server/IModule.h"

class INetActor;

class IClientNetModule : public IModule
{
public:
	IClientNetModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	virtual std::shared_ptr<INetActor> CreateHttpClientNet() = 0;
	virtual std::shared_ptr<INetActor> CreateSocketClientNet() = 0;
	virtual void AddClientToMap(std::shared_ptr<INetActor>& ptr) = 0;
	virtual std::shared_ptr<INetActor> GetClientNet(const int& fd) = 0;
	virtual void RemoveClientFromMap(const int& fd) = 0;
	// login client interface
	virtual void AddLoginClientToMap(std::shared_ptr<INetActor>& ptr) = 0;
	virtual std::shared_ptr<INetActor> GetLoginClient(const std::string& uid) = 0;
	virtual void RemoveLoginClientFromMap(const std::string& uid) = 0;
};

