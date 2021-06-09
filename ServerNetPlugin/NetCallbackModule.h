#pragma once
#include "INetCallbackModule.h"
#include "IClientNetActor.h"
#include "BackendClient.h"

class NetCallbackModule : public INetCallbackModule
{
private:
	std::map<int, std::string> m_map_game_msg; // backend msg map // for router
	std::map<int, std::function<void(IClientNetActor&)>> m_receive_callback;
	std::map<int, std::function<void(BackendClient&)>> m_map_backend_callback;
	std::map<int, std::function<void(std::vector<char>&, const std::string&, const std::string&)>> m_rpc_callback;
public:
	NetCallbackModule(std::shared_ptr<IPluginManager> ptr) : INetCallbackModule(ptr)
	{
	}

	// interface
	virtual const std::map<int, std::string> GetGameMsgMap(); // for router // bug
	//
	virtual void AddReceiveCallBack(const int msg_id, std::function<void(IClientNetActor&)> callback);
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallBackMap();
	virtual void AddBackendCallBack(const int msg_id, std::function<void(BackendClient&)> callback);
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallBackMap();
	virtual void AddRPCCallback(const int msg_id, std::function<void(std::vector<char>&, const std::string&, const std::string&)> callback);
	virtual const std::map<int, std::function<void(std::vector<char>&, const std::string&, const std::string&)>>& NetCallbackModule::GetRPCCallBackMap();
};

