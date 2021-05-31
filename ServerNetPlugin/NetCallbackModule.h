#pragma once
#include "INetCallbackModule.h"
#include "IClientNetActor.h"

class NetCallbackModule : public INetCallbackModule
{
private:
	std::map<int, std::string> m_map_game_msg; // backend msg map // for router
	std::map<int, std::function<void(IClientNetActor&)>> m_receive_callback;
	// std::map<int, std::function<void(std::shared_ptr<IClientNetActor>)>> m_frontend_callback_map; // frontend callback
public:
	NetCallbackModule(std::shared_ptr<IPluginManager> ptr) : INetCallbackModule(ptr)
	{
	}

	// interface
	virtual void AddReceiveCallBack(const int msgID, std::function<void(IClientNetActor&)> call_func);
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallBackMap();
	virtual const std::map<int, std::string> GetGameMsgMap(); // for router
};

