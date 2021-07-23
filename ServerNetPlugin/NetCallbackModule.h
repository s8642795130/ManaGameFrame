#pragma once
#include "INetCallbackModule.h"
#include "IPollClient.h"
#include "BackendClient.h"

class NetCallbackModule : public INetCallbackModule
{
private:
	std::map<int, std::string> m_map_game_msg; // backend msg map // for router
	std::map<int, std::shared_ptr<IBindFunc>> m_receive_callback;
	std::map<int, std::function<void(IPollClient&)>> m_master_callback;
	std::map<int, std::function<void(BackendClient&)>> m_map_backend_callback;
	std::map<int, std::function<void(const int, const int, std::vector<char>&)>> m_rpc_callback;
	std::map<std::string, std::shared_ptr<IBindFunc>> m_http_callback;
public:
	NetCallbackModule(std::shared_ptr<IPluginManager> ptr) : INetCallbackModule(ptr)
	{
	}

	// interface
	virtual const std::map<int, std::string>& GetGameMsgMap(); // for router // bug
	//
	virtual void AddReceiveCallback(const int msg_id, std::shared_ptr<IBindFunc> bind_func);
	virtual const std::map<int, std::shared_ptr<IBindFunc>>& GetReceiveCallbackMap();
	virtual void AddMasterCallback(const int msg_id, std::function<void(IPollClient&)> callback);
	virtual const std::map<int, std::function<void(IPollClient&)>>& GetMasterCallbackMap();
	virtual void AddBackendCallback(const int msg_id, std::function<void(BackendClient&)> callback);
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallbackMap();
	virtual void AddRPCCallback(const int msg_id, std::function<void(const int, const int, std::vector<char>&)> callback);
	virtual const std::map<int, std::function<void(const int, const int, std::vector<char>&)>>& GetRPCCallbackMap();
	virtual void AddHTTPCallback(const std::string& path, std::shared_ptr<IBindFunc> bind_func);
	virtual const std::map<std::string, std::shared_ptr<IBindFunc>>& GetHTTPCallbackMap();
};

