#pragma once
#include <string>
#include <map>
#include <functional>

class ClientNet;

class MsgRouter
{
private:
	static std::map<std::string, std::function<int(const int, const ClientNet&)>> m_router_func;
	static std::function<int(const int, const ClientNet&)> m_defalut_router_func;
public:
	static void initRouterFunc()
	{
		/*
		m_router_func.emplace("hallPlugin", [](const int server_count, const ClientNet& client) -> int
			{
				auto str_uid = client.GetUid();
				auto uid = std::stoi(str_uid);
				return (uid % server_count);
			}
		);
		*/
	}

	static void SetDefaultRouter(std::function<int(const int, const ClientNet&)> defalut_router_func)
	{
		m_defalut_router_func = defalut_router_func;
	}

	/*
	static int DefaultRouter(const int server_count, const ClientNet& client)
	{
		auto str_uid = client.GetUid();
		auto uid = std::stoi(str_uid);
		return (uid % server_count);
	}
	*/

	static int GetMsgRouterByClient(const std::string plugin_name, const int server_count, const ClientNet& client)
	{
		int ret = 0;

		auto it = m_router_func.find(plugin_name);
		if (it != std::cend(m_router_func))
		{
			ret = m_router_func[plugin_name](server_count, client);
		}
		else
		{
			ret = m_defalut_router_func(server_count, client);
		}
		return ret;
	}
};