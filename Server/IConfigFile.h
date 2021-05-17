#pragma once
#include <string>
#include <vector>

struct ServerData;

class IConfigFile
{
public:
	virtual const std::shared_ptr<ServerData>& GetServerDataByName(const std::string& server_name) = 0;
	virtual const std::vector<std::string>& GetServersByPluginName(const std::string& plugin_name) = 0;
	virtual const std::vector<std::string>& GetServersByType(const std::string& server_type) = 0;
	virtual const std::string& GetMyServerName() = 0;
};