#pragma once
#include <sys/epoll.h>

#include "IServerNetModule.h"
#include "IClientNetActor.h"
#include "IClientNetModule.h"
#include "../ConfigPlugin/IConfigModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "../Server/DefineHeader.h"

#include "FrontendListenerImpl.h"

class ServerNetModule : public IServerNetModule
{
private:
	// other module
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

	// listener
	CFrontendListenerImpl m_frontend_listener;
	std::shared_ptr<CTcpServerPtr> m_server;
protected:
	/*
	void CreateEpoll();
	void StartNetwork(uint16_t listen_port, uint32_t timeout_secs);
	bool SetNonblocking(int fd);
	bool HandleAccept();
	bool HandleClient(epoll_event& ev);
	void RemoveClient(std::shared_ptr<IClientNetActor>& ptr_client);
	*/
public:
	ServerNetModule(std::shared_ptr<IPluginManager> ptr);
	virtual ~ServerNetModule();

	// life cycle function
	virtual void Init();
	virtual void AfterInit();

	// function
	//virtual bool AddFD(std::shared_ptr<IClientNetActor> ptr_client);
	virtual void EventLoop();
};

