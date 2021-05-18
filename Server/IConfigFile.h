#pragma once
#include <string>
#include <vector>

struct ServerData;

class IConfigFile
{
public:
	virtual const std::shared_ptr <ServerData>& GetServerDataByName(const std::string& server_name) = 0;
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByPluginName(const std::string& plugin_name) = 0;
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByType(const std::string& server_name) = 0;
	virtual const std::shared_ptr<ServerData>& GetMyServerInfo() = 0;
	virtual const std::vector<std::shared_ptr<PluginData>>& GetPluginsByServerName(const std::string& server_name) = 0;
};