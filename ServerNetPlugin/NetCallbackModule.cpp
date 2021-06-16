#include "NetCallbackModule.h"

const std::map<int, std::string> NetCallbackModule::GetGameMsgMap()
{
	return m_map_game_msg;
}

void NetCallbackModule::AddReceiveCallback(const int msg_id, std::function<void(IClientNetActor&)> callback)
{
	m_receive_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(IClientNetActor&)>>& NetCallbackModule::GetReceiveCallbackMap()
{
	return m_receive_callback;
}

void NetCallbackModule::AddBackendCallback(const int msg_id, std::function<void(BackendClient&)> callback)
{
	m_map_backend_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(BackendClient&)>>& NetCallbackModule::GetBackendCallbackMap()
{
	return m_map_backend_callback;
}

void NetCallbackModule::AddRPCCallback(const int msg_id, std::function<void(const int, const int, std::vector<char>&)> callback)
{
	m_rpc_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(const int, const int, std::vector<char>&)>>& NetCallbackModule::GetRPCCallbackMap()
{
	return m_rpc_callback;
}

//
void NetCallbackModule::AddHTTPCallback(const std::string& path, std::function<void(IClientNetActor&)> callback)
{
	m_http_callback.emplace(path, callback);
}

const std::map<std::string, std::function<void(IClientNetActor&)>>& NetCallbackModule::GetHTTPCallbackMap()
{
	return m_http_callback;
}