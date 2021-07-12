#pragma once
#include "../Server/IModule.h"
#include "INetActor.h"
#include "BindFunc.h"

class BackendClient;
class IPollClient;

class INetCallbackModule : public IModule
{
public:
	INetCallbackModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual const std::map<int, std::string> GetGameMsgMap() = 0; // bug
	//
	virtual void AddReceiveCallback(const int msg_id, std::shared_ptr<IBindFunc> bind_func) = 0;
	virtual const std::map<int, std::shared_ptr<IBindFunc>>& GetReceiveCallbackMap() = 0;
	virtual void AddMasterCallback(const int msg_id, std::function<void(IPollClient&)> callback) = 0;
	virtual const std::map<int, std::function<void(IPollClient&)>>& GetMasterCallbackMap() = 0;
	virtual void AddBackendCallback(const int msgID, std::function<void(BackendClient&)> call_func) = 0;
	virtual const std::map<int, std::function<void(BackendClient&)>>& GetBackendCallbackMap() = 0;
	virtual void AddRPCCallback(const int msg_id, std::function<void(const int, const int, std::vector<char>&)> callback) = 0;
	virtual const std::map<int, std::function<void(const int, const int, std::vector<char>&)>>& GetRPCCallbackMap() = 0;
	virtual void AddHTTPCallback(const std::string& path, std::shared_ptr<IBindFunc> callback) = 0;
	virtual const std::map<std::string, std::shared_ptr<IBindFunc>>& GetHTTPCallbackMap() = 0;
};

