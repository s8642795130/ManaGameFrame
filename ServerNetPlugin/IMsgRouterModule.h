#pragma once
#include "../Server/IModule.h"
#include "IClientNetActor.h"

class IMsgRouterModule : public IModule
{
public:
	IMsgRouterModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;

	// interface
	virtual int GetConnectorIndexByClient(const int server_count, const std::string& uid) = 0;
	virtual int GetMsgRouterByClient(const std::string plugin_name, const int server_count, const IClientNetActor& client) = 0;
	virtual int GetMsgRouterByString(const std::string plugin_name, const int server_count, const std::string& router) = 0;
};

