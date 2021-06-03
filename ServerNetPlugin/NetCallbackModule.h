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
public:
	NetCallbackModule(std::shared_ptr<IPluginManager> ptr) : INetCallbackModule(ptr)
	{
	}

	// interface
	virtual void AddReceiveCallBack(const int msgID, std::function<void(IClientNetActor&)> call_func);
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallBackMap();
	virtual void AddBackendCallBack(const int msgID, std::function<void(BackendClient&)> call_func);
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallBackMap();
	virtual const std::map<int, std::string> GetGameMsgMap(); // for router
};

