#pragma once
#include "INetCallBackModule.h"
#include "IClientNetActor.h"

class NetCallBackModule : public INetCallBackModule
{
private:
	std::shared_ptr<std::map<int, std::function<void(std::shared_ptr<IClientNetActor>)>>> m_ptr_callback_map; // frontend callback
	// m_receive_callBack; // backend callback
public:
	NetCallBackModule(std::shared_ptr<IPluginManager> ptr) : INetCallBackModule(ptr),
		m_ptr_callback_map(std::make_shared<std::map<int, std::function<void(std::shared_ptr<IClientNetActor>)>>>())
	{
	}

	virtual void AddReceiveCallBack(const int msgID, std::function<void(std::shared_ptr<IClientNetActor>)> call_func);
};

