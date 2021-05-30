#pragma once

#include <vector>

#include "../Server/IModule.h"
#include "../Server/ServerTypeDefine.h"
#include "../Server/CommonStruct.h"

struct ServerData;
struct PluginData;

class IConfigModule : public IModule
{
public:
	IConfigModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;

	// interface
	virtual const std::shared_ptr <ServerData>& GetServerDataByName(const std::string& server_name) = 0;
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByPluginName(const std::string& plugin_name) = 0;
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByType(const std::string& server_name) = 0;
	virtual const std::shared_ptr<ServerData>& GetMyServerInfo() = 0;
	virtual const std::vector<std::shared_ptr<PluginData>>& GetPluginsByServerName(const std::string& server_name) = 0;
	virtual const NetServerType::ServerType GetServerType() = 0;
};