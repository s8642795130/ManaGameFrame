#pragma once
#include <string>

struct ServerData
{
	std::string m_server_type;
	std::string m_server_name;
	std::string m_server_ip;
	int m_port;
};

struct PluginData
{
	std::string m_plugin;
	std::string m_plugin_name;
};