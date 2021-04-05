#pragma once
#include <string>
#include <vector>
#include <map>

class ConfigFile
{
private:
	std::map<std::string, int> m_plugin_config;
	std::map<std::string, int> m_server_config;
	const std::string m_config_file_name{ "config.cfg" };
	std::vector<std::string> StrSplitBySpace(const std::string& s);
protected:
	void AnalyseConfigStr(const std::vector<std::string>& config_str);
public:
	bool ReadServerConfigFile();
	std::vector<int> GetServersByPluginName(const std::string& plugin_name);
	std::vector<int> GetPluginsByPluginName(const std::string& plugin_name);
};

