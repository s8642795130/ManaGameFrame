#pragma once
#include "INetProccessModule.h"
#include "IMsgRouterModule.h"
#include "INetCallbackModule.h"
#include "IServerObjModule.h"
#include "../ConfigPlugin/IConfigModule.h"

class NetProccessModule : public INetProccessModule
{
private:
	std::shared_ptr<IMsgRouterModule> m_router_module;
	std::shared_ptr<INetCallbackModule> m_callback_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
	std::shared_ptr<IConfigModule> m_config_module;
public:
	NetProccessModule(std::shared_ptr<IPluginManager> ptr) : INetProccessModule(ptr)
	{
	}

	// life cycle function
	virtual void Init();

	// interface
	virtual void ProcessFrontendIO(IClientNetActor& client);
	virtual void ProcessBackendIO(IClientNetActor& client);
	virtual void ProcessFrontendUnknowMsg(IClientNetActor& client);
	virtual void ProcessServerBackendIO(IClientNetActor& client);
	virtual void ProcessRPCIO(IClientNetActor& client);
	virtual void ProcessMasterIO(IClientNetActor& client);
};

