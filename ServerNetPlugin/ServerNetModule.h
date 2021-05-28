#pragma once
#include <sys/epoll.h>

#include "IServerNetModule.h"
#include "IClientNetActor.h"
#include "IClientNetModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "../Server/DefineHeader.h"

class ServerNetModule : public IServerNetModule
{
private:
	const int m_worker_max_events = WORKER_MAX_EVENTS;
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	std::array<epoll_event, WORKER_MAX_EVENTS> m_arr_events;
	// uint32_t timeout_secs_;
	// time_t last_socket_check_;
	// std::shared_ptr<IThreadPool> m_ptr_thread_pool;

	// other module
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;
protected:
	void CreateEpoll();
	bool SetNonblocking(int fd);
	void EventLoop();
	bool HandleAccept();
	bool HandleClient(epoll_event ev);
	void RemoveClient(std::shared_ptr<IClientNetActor> ptr_client);
public:
	ServerNetModule(std::shared_ptr<IPluginManager> ptr);
	~ServerNetModule();

	// life cycle function
	virtual void Init();
	virtual void AfterInit();

	// function
	virtual void StartNetwork(uint16_t listen_port, uint32_t timeout_secs);
	virtual bool AddFD(std::shared_ptr<IClientNetActor> ptr_client);
};

