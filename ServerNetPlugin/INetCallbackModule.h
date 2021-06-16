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
	virtual const std::map<int, std::string> GetGameMsgMap() = 0; // bug
	//
	virtual void AddReceiveCallback(const int msgID, std::function<void(IClientNetActor&)> call_func) = 0;
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallbackMap() = 0;
	virtual void AddBackendCallback(const int msgID, std::function<void(BackendClient&)> call_func) = 0;
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallbackMap() = 0;
	virtual void AddRPCCallback(const int msg_id, std::function<void(const int, const int, std::vector<char>&)> callback) = 0;
	virtual const std::map<int, std::function<void(const int, const int, std::vector<char>&)>>& GetRPCCallbackMap() = 0;
	virtual void AddHTTPCallback(const std::string& path, std::function<void(IClientNetActor&)> callback) = 0;
	virtual const std::map<std::string, std::function<void(IClientNetActor&)>>& GetHTTPCallbackMap() = 0;
	
};

