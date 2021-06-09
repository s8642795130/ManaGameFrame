#include "NetCallbackModule.h"

const std::map<int, std::string> NetCallbackModule::GetGameMsgMap()
{
	return m_map_game_msg;
}

void NetCallbackModule::AddReceiveCallBack(const int msg_id, std::function<void(IClientNetActor&)> callback)
{
	m_receive_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(IClientNetActor&)>>& NetCallbackModule::GetReceiveCallBackMap()
{
	return m_receive_callback;
}

void NetCallbackModule::AddBackendCallBack(const int msg_id, std::function<void(BackendClient&)> callback)
{
	m_map_backend_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(BackendClient&)>>& NetCallbackModule::GetBackendCallBackMap()
{
	return m_map_backend_callback;
}

void NetCallbackModule::AddRPCCallback(const int msg_id, std::function<void(std::vector<char>&, const std::string&, const std::string&)> callback)
{
	m_rpc_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(std::vector<char>&, const std::string&, const std::string&)>>& NetCallbackModule::GetRPCCallBackMap()
{
	return m_rpc_callback;
}