#include "MsgRouterModule.h"

void MsgRouterModule::Init()
{
	initRouterFunc();
}

void MsgRouterModule::initRouterFunc()
{
	m_router_func.emplace("hallPlugin", [](const int server_count, const IClientNetActor& client) -> int
		{
			auto str_uid = client.GetUid();
			auto uid = std::stoi(str_uid);
			return (uid % server_count);
		}
	);
}

int MsgRouterModule::DefaultRouter(const int server_count, const IClientNetActor& client)
{
	auto str_uid = client.GetUid();
	auto uid = std::stoi(str_uid);
	return (uid % server_count);
}

int MsgRouterModule::GetMsgRouterByClient(const std::string plugin_name, const int server_count, const IClientNetActor& client)
{
	int ret = 0;

	auto it = m_router_func.find(plugin_name);
	if (it != std::cend(m_router_func))
	{
		ret = m_router_func[plugin_name](server_count, client);
	}
	else
	{
		ret = DefaultRouter(server_count, client);
	}
	return ret;
}