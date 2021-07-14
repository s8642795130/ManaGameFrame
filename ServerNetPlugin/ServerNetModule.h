#pragma once
#include <sys/epoll.h>

#include "IServerNetModule.h"
#include "IClientNetModule.h"
#include "../ConfigPlugin/IConfigModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "../Server/DefineHeader.h"

#include "IListenerImpl.h"

class ServerNetModule : public IServerNetModule
{
private:
	// other module
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

	// listener
	std::shared_ptr<IListenerImpl> m_ptr_listener;
	std::shared_ptr<CTcpServerPtr> m_server;
protected:
public:
	ServerNetModule(std::shared_ptr<IPluginManager> ptr);
	virtual ~ServerNetModule();

	// life cycle function
	virtual void Init();
	virtual void AfterInit();
};

