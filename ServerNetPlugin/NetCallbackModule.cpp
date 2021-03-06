#include "NetCallbackModule.h"

const std::map<int, std::string>& NetCallbackModule::GetGameMsgMap()
{
	return m_map_game_msg;
}

void NetCallbackModule::AddReceiveCallback(const int msg_id, std::shared_ptr<IBindFunc> bind_func)
{
	m_receive_callback.emplace(msg_id, bind_func);
}

const std::map<int, std::shared_ptr<IBindFunc>>& NetCallbackModule::GetReceiveCallbackMap()
{
	return m_receive_callback;
}

void NetCallbackModule::AddMasterCallback(const int msg_id, std::function<void(IPollClient&)> callback)
{
	m_master_callback.emplace(msg_id, callback);
}

const std::map<int, std::function<void(IPollClient&)>>& NetCallbackModule::GetMasterCallbackMap()
{
	return m_master_callback;
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
void NetCallbackModule::AddHttpCallback(const std::string& path, std::shared_ptr<IHttpBindFunc> bind_func)
{
	m_http_callback.emplace(path, bind_func);
}

const std::map<std::string, std::shared_ptr<IHttpBindFunc>>& NetCallbackModule::GetHttpCallbackMap()
{
	return m_http_callback;
}