#pragma once
#include "INetCallbackModule.h"
#include "IClientNetActor.h"

class NetCallbackModule : public INetCallbackModule
{
private:
	std::shared_ptr<std::map<int, std::function<void(std::shared_ptr<IClientNetActor>)>>> m_ptr_callback_map; // frontend callback
	// m_receive_callBack; // backend callback
public:
	NetCallbackModule(std::shared_ptr<IPluginManager> ptr) : INetCallbackModule(ptr),
		m_ptr_callback_map(std::make_shared<std::map<int, std::function<void(std::shared_ptr<IClientNetActor>)>>>())
	{
	}

	virtual void AddReceiveCallBack(const int msgID, std::function<void(std::shared_ptr<IClientNetActor>)> call_func);
};

