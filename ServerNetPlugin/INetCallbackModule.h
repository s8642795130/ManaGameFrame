#pragma once
#include "../Server/IModule.h"
#include "IClientNetActor.h"

class BackendClient;

class INetCallbackModule : public IModule
{
public:
	INetCallbackModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual void AddReceiveCallBack(const int msgID, std::function<void(IClientNetActor&)> call_func) = 0;
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallBackMap() = 0;
	virtual void AddBackendCallBack(const int msgID, std::function<void(BackendClient&)> call_func) = 0;
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallBackMap() = 0;
	virtual const std::map<int, std::string> GetGameMsgMap() = 0;
};

