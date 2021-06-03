#include "NetCallbackModule.h"

const std::map<int, std::string> NetCallbackModule::GetGameMsgMap()
{
	return m_map_game_msg;
}

void NetCallbackModule::AddReceiveCallBack(const int msgID, std::function<void(IClientNetActor&)> call_func)
{
	m_receive_callback.emplace(msgID, call_func);
}

const std::map<int, std::function<void(IClientNetActor&)>>& NetCallbackModule::GetReceiveCallBackMap()
{
	return m_receive_callback;
}

void NetCallbackModule::AddBackendCallBack(const int msgID, std::function<void(BackendClient&)> call_func)
{
	m_map_backend_callback.emplace(msgID, call_func);
}

const std::map<int, std::function<void(BackendClient&)>>& NetCallbackModule::GetBackendCallBackMap()
{
	return m_map_backend_callback;
}