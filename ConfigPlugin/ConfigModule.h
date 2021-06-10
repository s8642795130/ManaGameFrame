#pragma once
#include "IConfigModule.h"
#include "../Server/ServerEnumDefine.h"

class ConfigModule : public IConfigModule
{
private:
	std::vector<std::shared_ptr<ServerData>> m_vec_server;
	std::vector<std::shared_ptr<PluginData>> m_vec_plugin;

	std::map<std::string, std::vector<std::shared_ptr<ServerData>>> m_plugin_config; // key: plugin name, value: server data list
	std::map<std::string, std::shared_ptr<ServerData>> m_server_config; // key: server name, value: server data
	std::map<std::string, std::vector<std::shared_ptr<ServerData>>> m_type_config; // key: server type, value: server data (find connector from router)
	std::map<std::string, std::vector<std::shared_ptr<PluginData>>> m_plugin_list; // key: server name, value: plugin data

	const std::string m_config_file_name{ "config.cfg" };
	void StringSplit(const std::string& str, const char split, std::vector<std::string>& res);

	// my server name
	std::string m_server_name;
	// my server type
	EnumDefine::ServerType m_server_type;
protected:
	void AnalyseConfigStr(const std::vector<std::string>& config_str);
	void AnalysePluginList();
	bool ReadServerConfigFile();
	void SetServerName(const std::string& server_name);
	void SetServerType();
	const EnumDefine::ServerType StrTypeToEnumType(const std::string& server_type);
public:
	ConfigModule(std::shared_ptr<IPluginManager> ptr) : IConfigModule(ptr)
	{
	}

	// life cycle
	virtual void Init();

	// interface
	virtual const std::shared_ptr <ServerData>& GetServerDataByName(const std::string& server_name);
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByPluginName(const std::string& plugin_name);
	virtual const std::vector<std::shared_ptr<ServerData>>& GetServersByType(const std::string& server_type); // type: master, connector
	virtual const std::shared_ptr<ServerData>& GetMyServerInfo();
	virtual const std::vector<std::shared_ptr<PluginData>>& GetPluginsByServerName(const std::string& server_name);
	virtual const EnumDefine::ServerType GetTypeByServerName(const std::string& server_name);
	virtual const EnumDefine::ServerType GetServerType();
};

