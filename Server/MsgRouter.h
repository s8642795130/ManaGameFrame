#pragma once
#include <string>
#include <map>
#include <functional>

#include "ClientNet.h"

class MsgRouter
{
private:
	static std::map<std::string, std::function<int(const int, const ClientNet&)>> m_router_func;
public:
	static void initRouterFunc()
	{
		m_router_func.emplace("hallPlugin", [](const int server_count, const ClientNet& client) -> int
			{
				auto str_uid = client.GetUid();
				auto uid = std::stoi(str_uid);
				return (uid % server_count);
			}
		);
	}

	static int DefaultRouter(const int server_count, const ClientNet& client)
	{
		auto str_uid = client.GetUid();
		auto uid = std::stoi(str_uid);
		return (uid % server_count);
	}

	static int GetMsgRouterByClient(const std::string plugin_name, const int server_count, const ClientNet& client)
	{
		m_router_func.find(plugin_name);
		return m_router_func[];
	}
};