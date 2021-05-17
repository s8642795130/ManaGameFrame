#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "IConfigFile.h"

struct ServerData
{
public:
	std::string m_server_type;
	std::string m_server_name;
	std::string m_server_ip;
	int m_port;
};

class ConfigFile : public IConfigFile
{
private:
	std::map<std::string, std::vector<std::string>> m_plugin_config; // key: plugin name, value: server name
	std::map<std::string, std::shared_ptr<ServerData>> m_server_config; // key: server name, value: server data
	std::map<std::string, std::vector<std::string>> m_type_config; // key: server type, value: server name

	const std::string m_config_file_name{ "config.cfg" };
	std::vector<std::string> StrSplitBySpace(const std::string& s);

	std::string m_server_name;
protected:
	void AnalyseConfigStr(const std::vector<std::string>& config_str);
public:
	bool ReadServerConfigFile();
	void SetServerName(const std::string& server_name);
	virtual const std::shared_ptr<ServerData>& GetServerDataByName(const std::string& server_name);
	virtual const std::vector<std::string>& GetServersByPluginName(const std::string& plugin_name);
	virtual const std::vector<std::string>& GetServersByType(const std::string& server_name);
	virtual const std::string& GetMyServerName();
};

