#pragma once
#include <map>
#include <string>

#include "../Server/IModule.h"

class INetActor;

class IServerObjModule : public IModule
{
public:
	IServerObjModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;
	virtual void AfterInit() = 0;

	// interface
	virtual const std::string GetServerUUIDByName(const std::string& server_name) = 0;
	virtual void SaveServerToMap(const std::string& server_name, const std::string& uuid) = 0;
	// callback
	virtual void OnServerOnlineCallback(INetActor& client) = 0;
};