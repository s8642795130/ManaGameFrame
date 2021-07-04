#pragma once
#include "INetProccessModule.h"
#include "IMsgRouterModule.h"
#include "INetCallbackModule.h"
#include "IServerObjModule.h"
#include "IClientNetModule.h"
#include "../ConfigPlugin/IConfigModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"

class NetProccessModule : public INetProccessModule
{
private:
	std::shared_ptr<IMsgRouterModule> m_router_module;
	std::shared_ptr<INetCallbackModule> m_callback_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
public:
	NetProccessModule(std::shared_ptr<IPluginManager> ptr) : INetProccessModule(ptr)
	{
	}

	// life cycle function
	virtual void Init();

	// interface
	/*
	virtual void ProcessBackendIO(IClientNetActor& client);
	virtual void ProcessFrontendUnknowMsg(std::shared_ptr<IClientNetActor> client);
	virtual void ProcessServerBackendIO(IClientNetActor& client);
	virtual void ProcessBackendUnknowMsg(IClientNetActor& client);
	virtual void ProcessRPCIO(IClientNetActor& client);
	virtual void ProcessMasterIO(IClientNetActor& client);
	virtual void ProcessTempIO(std::shared_ptr<IClientNetActor> client);
	virtual void ProcessHttpIO(IClientNetActor& client);
	*/
};

