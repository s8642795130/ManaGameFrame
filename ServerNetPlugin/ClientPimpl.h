#pragma once
#include <memory>

#include "INetProccessModule.h"
#include "INetCallbackModule.h"
#include "IMsgRouterModule.h"
#include "IServerObjModule.h"
#include "../ConfigPlugin/IConfigModule.h"

class ClientPimpl
{
public:
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<INetCallbackModule> m_callback_module;
	std::shared_ptr<INetProccessModule> m_proccess_module;
	std::shared_ptr<IMsgRouterModule> m_router_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
};