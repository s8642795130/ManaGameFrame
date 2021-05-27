#pragma once
#include <memory>

#include "IClientNetModule.h"
#include "IClientNetActor.h"

class ClientNetModule : public IClientNetModule
{
private:
	std::map<int, std::shared_ptr<IClientNetActor>> m_map_clients;
public:
	ClientNetModule(std::shared_ptr<IPluginManager> ptr) : IClientNetModule(ptr)
	{
	}

	virtual std::shared_ptr<IClientNetActor> CreateClientNet();
	virtual void AddClientToMap(std::shared_ptr<IClientNetActor> ptr);
	virtual void RemoveClientFromMap(const int& fd);
	virtual std::shared_ptr<IClientNetActor> GetClientNet(const int& fd);
};

